#include "GuiClient.h"
#include "ui_GuiClient.h"

const QString GuiClient::PARSER("KCloud::Parser");

GuiClient::GuiClient(QWidget *parent) : QMainWindow(parent), ui(new Ui::GuiClient){

	QSettings appSettings;
	ui->setupUi(this);

	m_client			= new Client(Client::AsGuiThread, this);
	m_tree				= ui->mainTreeWidget;
	m_resourceInfoTable = ui->resourceInfoTable;
	m_permissionTable	= ui->permissionTable;
	m_scene				= new QGraphicsScene(this);
	m_permScene			= new QGraphicsScene(this);
	m_loader			= new Loader(this);
	m_waiter			= new Waiter(this);
	m_player			= new QMediaPlayer(this);
	m_userSpaceBar		= ui->spaceBar;
	m_freeSpace			= ui->freeSpace;
	m_busySpace			= ui->busySpace;
	m_totalSpace		= ui->totalSpace;

	QFile::copy(":/sounds/sounds/bell.mp3", m_dir.path() + QString("/bell.mp3"));
	m_player->setMedia(QUrl::fromLocalFile(m_dir.path() + QString("/bell.mp3")));

	m_resourceInfoTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
	m_resourceInfoTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
	m_resourceInfoTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	m_permissionTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
	m_permissionTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

	ui->graphicsView->setScene(m_scene);
	ui->graphicsView_2->setScene(m_permScene);
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
	connect(m_waiter	, SIGNAL(serverTimeout()								),
			this		, SLOT	(close()										));

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

	qint64 free = ___4GB___ - m_user->getSpace();
	m_busySpace->setText(QString::number(m_user->getSpace()));
	m_freeSpace->setText(QString::number(free));
	m_totalSpace->setText(QString::number(___4GB___));
	double ratio = (static_cast<double>(m_user->getSpace()) / ___4GB___) * 100;
	m_userSpaceBar->setValue(ratio);

	m_tree->clear();
	m_resourceMap.clear();

	foreach (ResourceHeader head, m_client->getResourceList()) {
		if(m_resourceMap.contains(head.getParentId())){
			if(!m_resourceMap.contains(head.getId())){
				m_resourceMap.insert(head.getId(), new GraphicResourceHeader(head, m_resourceMap[head.getParentId()]));
			}
		}else{
			if(head.getName() == m_user->getEmail()){
				m_resourceMap.insert(head.getId(), new GraphicResourceHeader(head, m_tree, GraphicResourceHeader::SessionUser));
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
	m_tree->setCurrentItem(NULL);
}

void GuiClient::requestTree(){

	disableMain();
	m_waiter->setMessage("Richiedo l'albero delle risorse");
	m_waiter->show();
	QTimer::singleShot(500, m_client, SLOT(resourceTree()));

}

void GuiClient::disableMain(){

	this->setEnabled(false);
	m_waiter->setEnabled(true);
	m_loader->setEnabled(true);
}

void GuiClient::restoreMain(){
//	trace;
	this->setEnabled(true);
	m_waiter->quit();
	m_loader->quit();
}

void GuiClient::restoreMainWithSound(){
//	trace;
	m_player->setVolume(100);
	m_player->play();
	restoreMain();
}

void GuiClient::showLoader(){

	m_waiter->quit();
	m_loader->show();
}

void GuiClient::onServerAnswer(const CommandPacket::ServerAnswer serv){

	QString errors;

	for(int i = 0; i < m_client->lastErrors().size(); i++){
		errors += m_client->lastErrors()[i];
		if(i == m_client->lastErrors().size() - 1){
			errors += QString("."	);
		}else{
			errors += QString(", "	);
		}
	}

	switch (serv){
		case CommandPacket::UnsetAnswer					:
		case CommandPacket::LoginOk						:
		case CommandPacket::WrongEmail					:
		case CommandPacket::WrongPassword				:
		case CommandPacket::AlreadyLogged				:
		case CommandPacket::LogoutOk					:
		case CommandPacket::LogoutFail					:
				break;

		case CommandPacket::ResourceTreeOk				:
				restoreMain();
				refreshTree();
				break;
		case CommandPacket::ResourceTreeError			:
				restoreMain();
				QMessageBox::information	(this, PARSER, Client::MSG_09 + errors	);
				break;
		case CommandPacket::ResourceUpOk				:
				m_waiter->setMessage(Client::MSG_10						);
				QTimer::singleShot	(1000, m_client, SLOT(resourceUp())	);
				break;
		case CommandPacket::ResourceUpFail				:
				restoreMain();
				QMessageBox::critical		(this, PARSER, Client::MSG_11 + errors	);
				break;
		case CommandPacket::ResourceUpInvalidPerm		:
				restoreMain();
				QMessageBox::information	(this, PARSER, Client::MSG_12 + errors	);
				break;
		case CommandPacket::ResourceUpSpaceExhausted	:
				restoreMain();
				QMessageBox::information	(this, PARSER, Client::MSG_13 + errors	);
				break;
		case CommandPacket::ResourceUpParentIsNotDir	:
				restoreMain();
				QMessageBox::critical		(this, PARSER, Client::MSG_14 + errors	);
				break;
		case CommandPacket::ResourceUpAlreadyExists		:
				restoreMain();
				QMessageBox::information	(this, PARSER, Client::MSG_15 + errors	);
				break;
		case CommandPacket::ResourceUpFinalizeOk		:
				restoreMain();
				QMessageBox::information	(this, PARSER, Client::MSG_16 + errors	);
				requestTree();
				break;
		case CommandPacket::ResourceUpFinalizeFail		:
				restoreMain();
				QMessageBox::critical		(this, PARSER, Client::MSG_17 + errors	);
				break;
		case CommandPacket::ResourceDownOk				:
				m_loader->show();
				break;
		case CommandPacket::ResourceDownFail			:
				restoreMain();
				QMessageBox::information	(this, PARSER, Client::MSG_19 + errors	);
				break;
		case CommandPacket::ResourceDownInvalidId		:
				restoreMain();
				QMessageBox::information	(this, PARSER, Client::MSG_20 + errors	);
				break;
		case CommandPacket::ResourceDownInvalidPerm		:
				restoreMain();
				QMessageBox::information	(this, PARSER, Client::MSG_21 + errors	);
				break;
		case CommandPacket::ResourceDelOk				:
				restoreMain();
				QMessageBox::information	(this, PARSER, Client::MSG_22 + errors	);
				requestTree();
				break;
		case CommandPacket::ResourceDelFail				:
				restoreMain();
				QMessageBox::information	(this, PARSER, Client::MSG_23 + errors	);
				break;
		case CommandPacket::ResourceDelInvalidPerm		:
				restoreMain();
				QMessageBox::information	(this, PARSER, Client::MSG_24 + errors	);
				break;
		case CommandPacket::UserRegisterOk				:
				restoreMain();
				QMessageBox::information	(this, PARSER, Client::MSG_25 + errors	);
		case CommandPacket::UsernameAlreadyInUse		:
				restoreMain();
				QMessageBox::information	(this, PARSER, Client::MSG_26 + errors	);
				break;
		case CommandPacket::UserRegisterFail			:
				restoreMain();
				QMessageBox::critical		(this, PARSER, Client::MSG_27 + errors	);
				break;
		case CommandPacket::PasswordChangeOk			:
				restoreMain();
				QMessageBox::information	(this, PARSER, Client::MSG_28 + errors	);
		case CommandPacket::PasswordChangeFail			:
				restoreMain();
				QMessageBox::critical		(this, PARSER, Client::MSG_29 + errors	);
				break;
		case CommandPacket::ResourceModOk				:
				restoreMain();
				QMessageBox::information	(this, PARSER, Client::MSG_30 + errors	);
				break;
		case CommandPacket::ResourceModFail				:
				restoreMain();
				QMessageBox::critical		(this, PARSER, Client::MSG_31 + errors	);
				break;
		case CommandPacket::ResourceModInvalidId		:
				restoreMain();
				QMessageBox::critical		(this, PARSER, Client::MSG_32 + errors	);
				break;
		case CommandPacket::ResourceModInvalidPerm		:
				restoreMain();
				QMessageBox::information	(this, PARSER, Client::MSG_33 + errors	);
				break;
		case CommandPacket::ResourceSharingOk			:
				restoreMain();
				QMessageBox::information	(this, PARSER, Client::MSG_34 + errors	);
				break;
		case CommandPacket::ResourceSharingInvalidPerm	:
				restoreMain();
				QMessageBox::information	(this, PARSER, Client::MSG_35 + errors	);
				break;
		case CommandPacket::ResourceSharingInvalidId	:
				restoreMain();
				QMessageBox::information	(this, PARSER, Client::MSG_36 + errors	);
				break;
		case CommandPacket::ResourceSharingErrors		:
				restoreMain();
				QMessageBox::information	(this, PARSER, Client::MSG_37 + errors	);
				break;
		case CommandPacket::ResourceSharingFail			:
				restoreMain();
				QMessageBox::information	(this, PARSER, Client::MSG_38 + errors	);
				break;
		case CommandPacket::ServerInternalError			:
				restoreMain();
				QMessageBox::critical		(this, PARSER, Client::MSG_39 + errors	);
				break;
		default:
				restoreMain();
				QMessageBox::critical		(this, PARSER, Client::MSG_40 + QString::number((qint32)serv));
				break;
	}
}

void GuiClient::on_mainTreeWidget_itemClicked(QTreeWidgetItem *item, int column){

	Q_UNUSED(column);
	int currentRow = 0;
	GraphicResourceHeader * tmp = reinterpret_cast<GraphicResourceHeader *>(item);
	m_scene->clear();
	m_permScene->clear();
	ui->publicPerm->clear();
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
	if(tmp->getPublicPermission() != GraphicResourceHeader::PermUndef){
		if(tmp->getType() == GraphicResourceHeader::Dir){
			m_permScene->addPixmap(tmp->getPublicPermission() == GraphicResourceHeader::Read ? QIcon(":/icons/icons/read_dir.png").pixmap(64, 64) : QIcon(":/icons/icons/write_dir.png").pixmap(64, 64));
		}else{
			m_permScene->addPixmap(tmp->getPublicPermission() == GraphicResourceHeader::Read ? QIcon(":/icons/icons/read_file.png").pixmap(64, 64) : QIcon(":/icons/icons/write_file.png").pixmap(64, 64));
		}
		ui->publicPerm->setText(tmp->getPublicPermission() == GraphicResourceHeader::Read ? "Lettura" : "Scrittura");
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
//	GraphicResourceHeader::ResourceType resourceType;

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
//		resourceType	= GraphicResourceHeader::File;
		path			= QFileDialog::getOpenFileName(this, "KCloud::Upload", QDir::home().path());
	}else if(info.clickedButton() == dirBtn){
//		resourceType = GraphicResourceHeader::Dir;
		path			= QFileDialog::getExistingDirectory(this, "KCloud::Upload", QDir::home().path());
	}else{
		return;
	}

	if(path.isEmpty()){
		return;
	}
	GraphicResourceHeader fake(tmp->getHeader(), NULL);
	PermSettings settings(m_user, &fake, this);

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

void GuiClient::on_refreshBtn_clicked(){
	requestTree();
}

void GuiClient::on_modifyButton_clicked(){

	QMessageBox info;

	if(m_tree->currentItem() == NULL){
		return;
	}

	info.setText("Cosa vuoi modificare ?");
	info.setIcon(QMessageBox::Question);
	info.setWindowTitle("KCloud::Modifica");
	QPushButton *permBtn = info.addButton("Permessi", QMessageBox::AcceptRole);
	QPushButton *nameBtn = info.addButton("Rinomina", QMessageBox::AcceptRole);
	QPushButton *cancelBtn = info.addButton("Annulla", QMessageBox::RejectRole);

	Q_UNUSED(cancelBtn);

	info.exec();

	GraphicResourceHeader * tmp = reinterpret_cast<GraphicResourceHeader *>(m_tree->currentItem());

	if(info.clickedButton() == permBtn){
		PermSettings settings(m_user, tmp, this);

		settings.exec();

		if(settings.wasAccepted()){
			m_client->resourceShare(settings.getNewHeader());
		}else{
			return;
		}

	}else if(info.clickedButton() == nameBtn){
		RenameForm rename(tmp, this);
		rename.exec();
		if(rename.wasAccepted()){
			ResourceHeader newHeader(tmp->getHeader());
			newHeader.setName(rename.getNewName());
			m_client->resourceMod(newHeader);
		}else{
			return;
		}
	}else{
		return;
	}

}
