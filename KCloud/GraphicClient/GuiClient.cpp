#include "GuiClient.h"
#include "LoginForm.h"
#include "ui_GuiClient.h"
#include "FirstConfigForm.h"


GuiClient::GuiClient(QWidget *parent) : QMainWindow(parent), ui(new Ui::GuiClient){

	QSettings appSettings;
	ui->setupUi(this);

	m_client			= new Client(Client::AsGuiThread, this);
	m_tree				= ui->mainTreeWidget;
	m_resourceInfoTable = ui->resourceInfoTable;
	m_permissionTable	= ui->permissionTable;
	m_scene				= new QGraphicsScene(this);
	m_loader			= new Loader(this);
	m_waiter			= new Waiter(this);
	m_player			= new QMediaPlayer(this);

	QFile::copy(":/sounds/sounds/bell.mp3", m_dir.path() + QString("/bell.mp3"));
	m_player->setMedia(QUrl::fromLocalFile(m_dir.path() + QString("/bell.mp3")));

	m_resourceInfoTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
	m_resourceInfoTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
	m_resourceInfoTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	m_permissionTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
	m_permissionTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

	ui->graphicsView->setScene(m_scene);
	m_client->start();

	if(!appSettings.contains(T_STARTED)){
		FirstConfigForm f(m_client, this);
		f.exec();
		if(!f.getResult()){
			QTimer::singleShot(0, this, SLOT(close()));
		}
	}else{
		qint64 i = appSettings.value(T_STARTED).toLongLong();
		appSettings.setValue(T_STARTED, ++i);
	}

	LoginForm login(m_client, this);
	login.exec();
	if(!login.getResult()){
		QTimer::singleShot(0, this, SLOT(close()));
	}else{
		m_user = m_client->getSessionUser();
		QTimer::singleShot(100, this, SLOT(requestTree()));
	}

	connect(m_client	, SIGNAL(finished()										),
			m_client	, SLOT	(deleteLater()									));
	connect(m_client	, SIGNAL(serverAnswer(CommandPacket::ServerAnswer)		),
			this		, SLOT	(onServerAnswer(CommandPacket::ServerAnswer)	));
	connect(m_client	, SIGNAL(commandSended()								),
			this		, SLOT	(waitForServer()								));
	connect(m_client	, SIGNAL(transmissionRate(qint64,qint64,Resource::Mode)	),
			m_loader	, SLOT	(updateStatus(qint64,qint64,Resource::Mode)		));
	connect(m_client	, SIGNAL(compressionEnd()								),
			this		, SLOT	(showLoader()									));
	connect(m_loader	, SIGNAL(trasmissionEnd()								),
			this		, SLOT	(restoreMainWithSound()							));

}

GuiClient::~GuiClient(){
	m_client->quit();
	delete ui;
}

void GuiClient::closeEvent(QCloseEvent *event){

	if(m_status){
		m_client->disconnectFromHost();
	}
	QMainWindow::closeEvent(event);
}

void GuiClient::waitForServer(){
	disableMain();
	m_waiter->setMessage("Attendo la risposta del server...");
	m_waiter->show();
}


void GuiClient::refreshTree(){

	quint64 publicId;
	bool	flag		= true;

	if(m_client->getResourceList().isEmpty()){
		return;
	}

	m_tree->clear();
	m_resourceMap.clear();

	foreach (ResourceHeader head, m_client->getResourceList()) {
		if(m_resourceMap.contains(head.getParentId())){
			if(!m_resourceMap.contains(head.getId())){
				m_resourceMap.insert(head.getId(), new GraphicResourceHeader(head, m_resourceMap[head.getParentId()]));
			}
		}else{
			if(head.getName() == m_user->getEmail()){
				m_resourceMap.insert(head.getId(), new GraphicResourceHeader(head, m_tree));
			}else{
				if(head.getPermission(m_user->getEmail()) != ResourceHeader::PermUndef){
					ResourceHeader tmp(head.getParentId(), this);
					tmp.setType(ResourceHeader::Dir);
					tmp.setName(head.getOwner());
					m_resourceMap.insert(tmp.getId(), new GraphicResourceHeader(tmp, m_tree, GraphicResourceHeader::OtherUser));
					m_resourceMap.insert(head.getId(), new GraphicResourceHeader(head, m_resourceMap[head.getParentId()]));
				}else{
					if(flag){
						ResourceHeader tmp(head.getParentId(), this);
						tmp.setType(ResourceHeader::Dir);
						tmp.setName("Pubblici");
						publicId = tmp.getId();
						flag = false;
						m_resourceMap.insert(tmp.getId(), new GraphicResourceHeader(tmp, m_tree, GraphicResourceHeader::Public));
					}
					m_resourceMap.insert(head.getId(), new GraphicResourceHeader(head, m_resourceMap[publicId]));
				}
			}
		}
	}
}

void GuiClient::requestTree(){

	m_waiter->setMessage("Richiedo l'albero delle risorse");

	QTimer::singleShot(500, m_client, SLOT(resourceTree()));

}

void GuiClient::disableMain(){

	this->setEnabled(false);
	m_waiter->setEnabled(true);
	m_loader->setEnabled(true);
}

void GuiClient::restoreMain(){
	trace;
	this->setEnabled(true);
	m_waiter->quit();
	m_loader->quit();
}

void GuiClient::restoreMainWithSound(){
	trace;
	m_player->setVolume(100);
	m_player->play();
	restoreMain();
}

void GuiClient::showLoader(){

	m_waiter->quit();
	m_loader->show();
}

void GuiClient::onServerAnswer(const CommandPacket::ServerAnswer serv){

	switch (serv){
		case CommandPacket::ResourceUpOk:
			m_waiter->setMessage("Comprimo i file...");
			QTimer::singleShot(1000, m_client, SLOT(resourceUp()));
			break;
		case CommandPacket::ResourceUpFail:
			restoreMain();
			QMessageBox::critical(this, "KCloudParser", "Il caricamento non può essere effettuato per motivi interni al server...");
			break;
		case CommandPacket::ResourceUpInvalidPerm:
			restoreMain();
			QMessageBox::information(this, "KCloudParser", "I tuoi permessi non sono sufficienti per caricare in questa cartella...");
			break;
		case CommandPacket::ResourceUpParentIsNotDir:
			restoreMain();
			QMessageBox::critical(this, "KCloudParser", "La risorsa selezionata è in realtà un file, non posso caricare...");
			break;
		case CommandPacket::ResourceUpAlreadyExists:
			restoreMain();
			QMessageBox::information(this, "KCloudParser", "Non puoi caricare due risorse con lo stesso nome nello stesso percorso...");
			break;
		case CommandPacket::ResourceUpSpaceExhausted:
			restoreMain();
			QMessageBox::information(this, "KCloudParser", "Non hai abbastanza spazio per caricare la risorsa, elimina qualche file...");
			break;
		case CommandPacket::ResourceUpFinalizeOk:
			restoreMain();
			QMessageBox::information(this, "KCloud::Parser", "Risorsa caricata correttamente!");
			requestTree();
			break;
		case CommandPacket::ResourceUpFinalizeFail:
			restoreMain();
			QMessageBox::critical(this, "KCloud::Parser", "Il server ha riscontrato un errore interno, il caricamento non è andato a buon fine!");
			break;
		case CommandPacket::ResourceDownOk:
			m_loader->show();
			break;
		case CommandPacket::ResourceDownInvalidId:

			break;
		case CommandPacket::ResourceDownInvalidPerm:

			break;
		case CommandPacket::ResourceDownFail:

			break;
		case CommandPacket::ResourceTreeOk:
			restoreMain();
			refreshTree();
			break;
		case CommandPacket::ResourceTreeError:

			break;
		case CommandPacket::ResourceDelOk:
			restoreMain();
			QMessageBox::information(this, "KCloud::Parser", "Risorsa eliminata correttamente!");
			requestTree();
			break;
		case CommandPacket::ResourceDelInvalidPerm:

			break;
		case CommandPacket::ResourceDelFail:

			break;
		case CommandPacket::UserRegisterOk:

			break;
		case CommandPacket::UsernameAlreadyInUse:

			break;
		case CommandPacket::UserRegisterFail:

			break;
		default:

			break;
	}

}

void GuiClient::on_mainTreeWidget_itemClicked(QTreeWidgetItem *item, int column){

	Q_UNUSED(column);
	int currentRow = 0;
	GraphicResourceHeader * tmp = reinterpret_cast<GraphicResourceHeader *>(item);
	m_scene->clear();
	m_scene->addPixmap(tmp->getImage());
	m_resourceInfoTable->item(0, 1)->setText(tmp->getName());
	m_resourceInfoTable->item(1, 1)->setText(QString::number(tmp->getSize()));
	m_resourceInfoTable->item(2, 1)->setText(tmp->getType() == ResourceHeader::Dir ? "Cartella" : "File");
	m_permissionTable->setRowCount(0);
	foreach (QString user, tmp->getPermissionTable().keys()){
		QTableWidgetItem * userItem = new QTableWidgetItem();
		QTableWidgetItem * permItem = new QTableWidgetItem();
		userItem->setText(user);
		if(tmp->getType() == GraphicResourceHeader::Dir){
			permItem->setIcon(tmp->getPermission(user) == GraphicResourceHeader::Read ? QIcon(":/icons/icons/read_dir.png") : QIcon(":/icons/icons/write_dir.png"));
		}else{
			permItem->setIcon(tmp->getPermission(user) == GraphicResourceHeader::Read ? QIcon(":/icons/icons/read_file.png") : QIcon(":/icons/icons/write_file.png"));
		}
		permItem->setText(tmp->getPermission(user) == GraphicResourceHeader::Read ? QString("Lettura") : QString("Scrittura"));

		userItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
		permItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		m_permissionTable->insertRow(currentRow);
		m_permissionTable->setItem(currentRow, 0, userItem);
		m_permissionTable->setItem(currentRow, 1, permItem);
		currentRow++;
	}
}

void GuiClient::on_mainTreeWidget_itemSelectionChanged(){
	on_mainTreeWidget_itemClicked(m_tree->currentItem(), 0);
}

void GuiClient::on_downloadButton_clicked(){

	if(m_tree->currentItem() == NULL){
		return;
	}
	quint64 id = reinterpret_cast<GraphicResourceHeader *>(m_tree->currentItem())->getId();
	m_client->newDownload(id);
}

void GuiClient::on_uploadButton_clicked(){

	if(m_tree->currentItem() == NULL){
		return;
	}

	GraphicResourceHeader * tmp = reinterpret_cast<GraphicResourceHeader *>(m_tree->currentItem());
	GraphicResourceHeader::ResourceType resourceType;

	if(tmp->getType() == GraphicResourceHeader::File){
		QMessageBox::information(this, "KCloud::Info", tmp->getName() + QString(" è un file, non posso inserire oggetti all'interno..."));
		return;
	}
	QString path;
	QMessageBox info;
	QPushButton *fileBtn = info.addButton("File", QMessageBox::AcceptRole);
	QPushButton *dirBtn = info.addButton("Cartella", QMessageBox::AcceptRole);
	QPushButton *cancelBtn = info.addButton("Annulla", QMessageBox::RejectRole);
	info.setText("KCloud::Upload");
	info.setIcon(QMessageBox::Information);
	info.setText("Vuoi caricare un file o una cartella?");
	info.exec();

	Q_UNUSED(cancelBtn);

	if(info.clickedButton() == fileBtn){
		resourceType	= GraphicResourceHeader::File;
		path			= QFileDialog::getOpenFileName(this, "KCloud::Upload", QDir::home().path());
	}else if(info.clickedButton() == dirBtn){
		resourceType = GraphicResourceHeader::Dir;
		path			= QFileDialog::getExistingDirectory(this, "KCloud::Upload", QDir::home().path());
	}else{
		return;
	}

	if(path.isEmpty()){
		return;
	}

	PermSettings settings(m_user, resourceType, this);

	settings.exec();

	if(settings.wasAccepted()){
		m_client->newUpload(path, m_user, tmp->getId(), settings.getPermMap(), settings.getPublicPerm());
	}
}

void GuiClient::on_deleteButton_clicked(){

	if(m_tree->currentItem() == NULL){
		return;
	}

	GraphicResourceHeader * tmp = reinterpret_cast<GraphicResourceHeader *> (m_tree->currentItem());

	QMessageBox msg(this);
	QPushButton *btnYes = msg.addButton("Si", QMessageBox::AcceptRole);
	QPushButton *btnNo	= msg.addButton("No", QMessageBox::RejectRole);
	msg.setText(QString("Eliminare veramente ") + tmp->getName());
	msg.setWindowTitle("KCloud::Eliminazione");
	msg.setIcon(QMessageBox::Information);
	msg.exec();
	Q_UNUSED(btnNo);
	if(msg.clickedButton() == btnYes){
		m_client->resourceDel(tmp->getId());
	}
}
