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

	connect(m_client	, SIGNAL(finished()										),
			m_client	, SLOT	(deleteLater()									));
	connect(m_client	, SIGNAL(serverAnswer(CommandPacket::ServerAnswer)		),
			this		, SLOT	(onServerAnswer(CommandPacket::ServerAnswer)	));
	connect(m_client	, SIGNAL(newCommand()									),
			m_waiter	, SLOT	(waitForServer()								));
	connect(m_waiter	, SIGNAL(waitComplete()									),
			this		, SLOT	(restoreMain()									));
	connect(m_client	, SIGNAL(transmissionRate(qint64,qint64,Resource::Mode)	),
			m_loader	, SLOT	(updateStatus(qint64,qint64,Resource::Mode)		));
	connect(m_loader	, SIGNAL(trasmissionEnd()								),
			this		, SLOT	(restoreMainWithSound()							));

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

void GuiClient::waitResponse(const QString &message){
	disableMain();
	m_waiter->setMessage(message);
	m_waiter->show();
	QThread::msleep(150);
}

void GuiClient::waitTransmission(){
	disableMain();
	m_loader->show();
}


void GuiClient::refreshTree(){

	quint64 publicId;
	bool	flag		= true;

	if(m_client->getResourceList().isEmpty()){
		return;
	}

	foreach (quint64 id, m_resourceMap.keys()) {
		delete m_resourceMap[id];
	}

	m_resourceMap.clear();
	foreach (ResourceHeader head, m_client->getResourceList()) {
		if(m_resourceMap.contains(head.getParentId())){
			m_resourceMap.insert(head.getId(), new GraphicResourceHeader(head, m_resourceMap[head.getParentId()]));
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

	waitResponse("Attendo l'albero delle risorse...");
	m_client->resourceTree();

}

void GuiClient::disableMain(){

	this->setEnabled(false);
	m_waiter->setEnabled(true);
	m_loader->setEnabled(true);
}

void GuiClient::restoreMain(){
	trace;
	this->setEnabled(true);
	m_waiter->hide();
	m_loader->hide();
}

void GuiClient::restoreMainWithSound(){
	trace;
	m_player->setVolume(100);
	m_player->play();
	restoreMain();
}

void GuiClient::onServerAnswer(const CommandPacket::ServerAnswer serv){

	switch (serv) {
		case CommandPacket::ResourceTreeOk:
			refreshTree();
			break;
		case CommandPacket::ResourceDownOk:
			waitTransmission();

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
	waitResponse("Controllo i permessi...");
	m_client->newDownload(id);
}

void GuiClient::on_uploadButton_clicked(){

	if(m_tree->currentItem() == NULL){
		return;
	}

	GraphicResourceHeader * tmp = reinterpret_cast<GraphicResourceHeader *>(m_tree->currentItem());

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
		path = QFileDialog::getOpenFileName(this, "KCloud::Upload", QDir::home().path());
	}else if(info.clickedButton() == dirBtn){
		path = QFileDialog::getExistingDirectory(this, "KCloud::Upload", QDir::home().path());
	}else{
		return;
	}

	PermSettings(m_user, GraphicResourceHeader::File, this).exec();

	return;

	waitResponse("Aspetto la risposta del server...");
	m_client->newUpload(path, m_user, tmp->getId());

}
