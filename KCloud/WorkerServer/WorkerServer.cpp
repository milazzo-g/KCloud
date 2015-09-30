#include "WorkerServer.h"
#include "QDebug"

#include "../MainServer/defines.h"

KCloud::WorkerServer::WorkerServer(int sd, QObject *parent) : Engine(parent){

	QSettings appSettings;
	QString tmp = address() + QString::number(QDateTime::currentMSecsSinceEpoch());
	m_key1 = QCryptographicHash::hash(tmp.toLocal8Bit(), QCryptographicHash::Md5).toHex();
	tmp = keyFirst() + QString::number(QDateTime::currentMSecsSinceEpoch());
	m_key2 = QCryptographicHash::hash(tmp.toLocal8Bit(), QCryptographicHash::Md5).toHex();
	m_socket->setSocketDescriptor(sd);
	m_dir.cd(appSettings.value(INSTANCES).toString());
	m_dir.mkdir(keyFirst());
	m_dir.cd(keyFirst());
	m_resourcesManager	= new ResourcesManager(keyFirst(), this);
	m_usersManager		= new UsersManager(keyLast(), this);

	connect(m_socket	, SIGNAL(disconnected()		), this, SLOT(quit())									);
	connect(m_resource	, SIGNAL(objectReceived()	), this, SLOT(finalizeUpload()	), Qt::UniqueConnection	);
	connect(m_resource	, SIGNAL(objectSended()		), this, SLOT(finalizeDownload()), Qt::UniqueConnection	);
	connect(m_packet	, SIGNAL(objectSended()		), this, SLOT(receiveCommand()	), Qt::UniqueConnection	);
}

KCloud::WorkerServer::~WorkerServer(){

	if(m_user){
		forcedLogout();
	}
	recursiveRemove(m_dir.path());
	qDebug() << address() << ": Job ended!";
//	emit removeFromActiveHandlers(address());
}

void KCloud::WorkerServer::run(){

	clog("Spawned");
	connect(m_packet, SIGNAL(objectReceived()), this, SLOT(parse()));
	receiveCommand();
	exec();
}

void KCloud::WorkerServer::parse(){

	clog(QString("Command received from ") + m_socket->peerAddress().toString());

	switch (m_packet->getClientCommand()){
		case CommandPacket::Login:
			login();
			break;
		case CommandPacket::Logout:
			logout();
			break;
		case CommandPacket::ResourceUp:
			resourceUp();
			break;
		case CommandPacket::ResourceDown:
			resourceDown();
			break;
		case CommandPacket::ResourceDel:
			resourceDel();
			break;
		case CommandPacket::ResourceTree:
			resourceTree();
			break;
		case CommandPacket::UserRegister:
			userRegister();
			break;
		case CommandPacket::PasswordChange:
			passwordChange();
			break;
		case CommandPacket::ResourceMod:
			resourceMod();
			break;
		case CommandPacket::ResourceSharing:
			resourceShare();
			break;
		default:
			break;
	}
}

void KCloud::WorkerServer::login(){						////Completata ma meglio riguardare poi

	clog(QString("Login request from ") + m_socket->peerAddress().toString());

	try {
		UsersManager::UsersManagerAnswer r = m_usersManager->checkLogin(m_packet->getUser());
		switch (r){
			case UsersManager::LoginOK:
				clog("Login Ok!");
				m_user = new User(m_usersManager->getUser());
				m_packet->answerToLogin(CommandPacket::LoginOk, *m_user);
				break;
			case UsersManager::UserNotFound:
				clog("User Not Found!");
				m_packet->answerToLogin(CommandPacket::WrongEmail);
				break;
			case UsersManager::UserWrongHash:
				clog("User Wrong Hash!");
				m_packet->answerToLogin(CommandPacket::WrongPassword);
				break;
			case UsersManager::UserAlreadyLogged:
				clog("User Already Logged!");
				m_packet->answerToLogin(CommandPacket::AlreadyLogged);
				break;
			default:
				clog("Generalmente non dovremmo essere qui!");
				clog(QString("m_usersManager->checkLogin(m_packet->getUser())") + QString::number((qint32)r));
				break;
		}
	} catch (Exception &e) {
		clog("Exception Occurred!");
		clog(e.what());
		QStringList errors;
		errors << QString(e.what()) << m_usersManager->lastSqlError() << m_usersManager->lastDriverError();
		m_packet->answerToLogin(CommandPacket::ServerInternalError, m_packet->getUser(), errors);
	}
	sendCommand();
}

void KCloud::WorkerServer::logout(){					////Completata ma meglio riguardare poi

	clog(QString("Logout request from ") + m_socket->peerAddress().toString());

	if(userIsLogged()){
		try {
			UsersManager::UsersManagerAnswer r = m_usersManager->checkLogout(*m_user);
			switch (r){
				case UsersManager::LogoutOK:
					clog("Logout Ok!");
					if(m_user){
						delete m_user;
						m_user = NULL;
					}
					m_packet->answerToLogout(CommandPacket::LogoutOk);
					break;
				case UsersManager::UserNotFound:
				case UsersManager::UserWrongHash:
				case UsersManager::UserAlreadyUnLogged:
					clog("Logout Fail!");
					m_packet->answerToLogout(CommandPacket::LogoutFail);
					break;
				default:
					clog("Generalmente non dovremmo essere qui!");
					clog(QString("m_usersManager->checkLogout(m_packet->getUser())") + QString::number((qint32)r));
					break;
			}
		} catch (Exception &e) {
			clog("Exception Occurred!");
			clog(e.what());
			QStringList errors;
			errors << QString(e.what()) << m_usersManager->lastSqlError() << m_usersManager->lastDriverError();
			m_packet->answerToLogout(CommandPacket::ServerInternalError, errors);
		}
	}else{
		m_packet->answerToLogout(CommandPacket::NotLoggedUser);
	}

	sendCommand();
}

void KCloud::WorkerServer::resourceUp(){				////Riguardare per forza!

	clog(QString("Resource Upload request from ") + m_socket->peerAddress().toString());

	if(userIsLogged()){
		try{
			m_head = m_packet->getFirstResourceHeader();
			ResourcesManager::ResourcesManagerAnswer r = m_resourcesManager->checkForUpload(*m_user, m_head);
			switch (r) {
				case ResourcesManager::UploadOK:
					clog(m_socket->peerAddress().toString() + QString(" can upload"));
					m_packet->answerToResourceUp(CommandPacket::ResourceUpOk);
					m_resource->setZipName(m_head.getName());
					m_resource->setZipDir(m_dir.path());

					disconnect	(m_packet, SIGNAL(objectSended()), this, SLOT(receiveCommand())							);
					connect		(m_packet, SIGNAL(objectSended()), this, SLOT(receiveResource()), Qt::UniqueConnection	);

					break;
				case ResourcesManager::PermError:
					clog(m_socket->peerAddress().toString() + QString(" not have permission"));
					m_packet->answerToResourceUp(CommandPacket::ResourceUpInvalidPerm);
					break;
				case ResourcesManager::SpaceFull:
					clog(m_socket->peerAddress().toString() + QString(" not have space"));
					m_packet->answerToResourceUp(CommandPacket::ResourceUpSpaceExhausted);
					break;
				case ResourcesManager::AlreadyExists:
					clog(m_socket->peerAddress().toString() + QString(" can't overwrite"));
					m_packet->answerToResourceUp(CommandPacket::ResourceUpAlreadyExists);
					break;
				case ResourcesManager::NotADir:
					clog(m_socket->peerAddress().toString() + QString(" wrong parent"));
					m_packet->answerToResourceUp(CommandPacket::ResourceUpParentIsNotDir);
					break;
				case ResourcesManager::ParentNotFound:
					clog(m_socket->peerAddress().toString() + QString(" invalid parent"));
					m_packet->answerToResourceUp(CommandPacket::ResourceUpFail);
					break;
				default:
					clog("Generalmente non dovremmo essere qui!");
					clog(QString("m_resourcesManager->checkForUpload(*m_user, m_packet->getFirstResourceHeader()) ") + QString::number((qint32)r));
					m_packet->answerToResourceUp(CommandPacket::ResourceUpFail);
					break;
			}
		}catch(Exception &e){
			clog("Exception Occurred!");
			clog(e.what());
			QStringList errors;
			errors << QString(e.what()) << m_resourcesManager->lastSqlError() << m_resourcesManager->lastDriverError();
			m_packet->answerToResourceUp(CommandPacket::ServerInternalError, errors);
		}
	}else{
		m_packet->answerToResourceUp(CommandPacket::NotLoggedUser);
	}
	sendCommand();
}

void KCloud::WorkerServer::resourceMod(){			//nuovo arrivo

	clog(QString("Resource modification request from ") + m_socket->peerAddress().toString());

	if(userIsLogged()){
		try{
			ResourcesManager::ResourcesManagerAnswer r = m_resourcesManager->modResource(*m_user, m_packet->getFirstResourceHeader());

			switch (r){
				case ResourcesManager::ResourceNotExist:
					clog("Resource modify, resource not exist");
					m_packet->answerToResourceMod(CommandPacket::ResourceModInvalidId);
					break;
				case ResourcesManager::PermError:
					clog("Resource modify, not having permission");
					m_packet->answerToResourceMod(CommandPacket::ResourceModInvalidPerm);
					break;
				case ResourcesManager::ResourceModOk:
					clog("Resource modified");
					m_packet->answerToResourceMod(CommandPacket::ResourceModOk);
					break;
				default:
					clog("Generalmente non dovremmo essere qui!");
					clog(QString("m_resourcesManager->modResource(*m_user, m_packet->getFirstResourceHeader()) = ") + QString::number((qint32)r));
					m_packet->answerToResourceMod(CommandPacket::ResourceModFail);
					break;
			}
		}catch(Exception &e){
			clog("Exception Occurred!");
			clog(e.what());
			QStringList errors;
			errors << QString(e.what()) << m_resourcesManager->lastSqlError() << m_resourcesManager->lastDriverError();
			m_packet->answerToResourceMod(CommandPacket::ResourceModFail, errors);
		}
	}else{
		m_packet->answerToResourceMod(CommandPacket::NotLoggedUser);
	}
	sendCommand();
}

void KCloud::WorkerServer::resourceDel(){

	clog(QString("Resource Delete request from ") + m_socket->peerAddress().toString());

	if(userIsLogged()){
		try{
			ResourcesManager::ResourcesManagerAnswer r = m_resourcesManager->delResources(*m_user, m_packet->getFirstResourceHeader());

			switch (r) {
				case ResourcesManager::PermError:
					clog(m_socket->peerAddress().toString() + " not have permissions");
					m_packet->answerToResourceDel(CommandPacket::ResourceDelInvalidPerm);
					break;
				case ResourcesManager::DeleteOK:
					clog(m_socket->peerAddress().toString() + " delete ok");
					m_packet->answerToResourceDel(CommandPacket::ResourceDelOk);
					break;
				default:
					clog("Generalmente non dovremmo essere qui!");
					clog(QString("m_resourcesManager->delResources(*m_user, m_packet->getFirstResourceHeader()) ") + QString::number((qint32)r));
					m_packet->answerToResourceDel(CommandPacket::ResourceDelFail);
					break;
			}

		}catch(Exception &e){
			clog("Exception Occurred!");
			clog(e.what());
			QStringList errors;
			errors << QString(e.what()) << m_resourcesManager->lastSqlError() << m_resourcesManager->lastDriverError();
			m_packet->answerToResourceDel(CommandPacket::ServerInternalError, errors);
		}
	}else{
		m_packet->answerToResourceDel(CommandPacket::NotLoggedUser);
	}
	sendCommand();
}

void KCloud::WorkerServer::resourceTree(){

	clog(QString("Resource Tree request from ") + m_socket->peerAddress().toString());

	if(userIsLogged()){
		try{
			QList<ResourceHeader> list = m_resourcesManager->resourceTree(*m_user);

			if(list.isEmpty()){
				clog(QString("Resource Tree failed for ") + m_socket->peerAddress().toString());
				m_packet->answerToResourceTree(CommandPacket::ResourceTreeError, list);
			}else{
				clog(QString("Resource Tree OK for ") + m_socket->peerAddress().toString());
				m_packet->answerToResourceTree(CommandPacket::ResourceTreeOk, list);
			}

		}catch(Exception &e){
			clog("Exception Occurred!");
			clog(e.what());
			QStringList errors;
			errors << QString(e.what()) << m_resourcesManager->lastSqlError() << m_resourcesManager->lastDriverError();
			m_packet->answerToResourceTree(CommandPacket::ServerInternalError, QList<ResourceHeader>(), errors);
		}
	}else{
		m_packet->answerToResourceTree(CommandPacket::NotLoggedUser, QList<ResourceHeader>());
	}
	sendCommand();
}

void KCloud::WorkerServer::resourceDown(){				////Permessi OK

	clog(QString("Resource Download request from ") + m_socket->peerAddress().toString());
	if(userIsLogged()){
		try{
			QStringList filesMoved;
			quint64		resourceId = m_packet->getFirstResourceHeader().getId();
			ResourcesManager::ResourcesManagerAnswer r =	m_resourcesManager->getResources(*m_user,
															m_dir.path(),
															m_packet->getFirstResourceHeader(),
															filesMoved);
			switch (r) {
				case ResourcesManager::RecursiveGetOK:
					if(filesMoved.isEmpty()){
						throw RecursionError();
					}
					m_resource->setResourcePath(filesMoved[0]);
					m_resource->setZipDir(m_dir.path());
					m_resource->setZipName(QFileInfo(filesMoved[0]).fileName());
					m_packet->answerToResourceDown(CommandPacket::ResourceDownOk, m_resourcesManager->headerForId(resourceId));

					disconnect	(m_packet	, SIGNAL(objectSended()), this, SLOT(receiveCommand()	)						);
					connect		(m_packet	, SIGNAL(objectSended()), this, SLOT(sendResource()		), Qt::UniqueConnection	);

					break;
				case ResourcesManager::RecursiveGetFail:
					m_packet->answerToResourceDown(CommandPacket::ResourceDownInvalidId);
					break;
				case ResourcesManager::PermError:
					clog("siamo qua!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1");
					m_packet->answerToResourceDown(CommandPacket::ResourceDownInvalidPerm);
					break;
				default:
					clog("Generalmente non dovremmo essere qui!");
					clog(QString("m_resourcesManager->getResources(m_dir.path(), m_packet->getFirstResourceHeader());") + QString::number((qint32)r));
					m_packet->answerToResourceDown(CommandPacket::ResourceDownFail);
					break;
			}
		}catch(Exception &e){
			clog("Exception Occurred!");
			clog(e.what());
			QStringList errors;
			errors << QString(e.what()) << m_resourcesManager->lastSqlError() << m_resourcesManager->lastDriverError();
			m_packet->answerToResourceDown(CommandPacket::ServerInternalError, ResourceHeader(), errors);
		}
	}else{
		m_packet->answerToResourceDown(CommandPacket::NotLoggedUser);
	}
	sendCommand();
}

void KCloud::WorkerServer::userRegister(){

	clog(QString("UserRegister request from ") + m_socket->peerAddress().toString());

	try {
		UsersManager::UsersManagerAnswer r =  m_usersManager->checkUserRegister(m_packet->getUser());
		switch (r){
			case UsersManager::UserRegisterOk:
				clog("New UserRegister Ok!");
				m_packet->answerToUserRegister(CommandPacket::UserRegisterOk);
				break;
			case UsersManager::UsernameAlreadyInUse:
				clog("Username already in use!");
				m_packet->answerToUserRegister(CommandPacket::UsernameAlreadyInUse);
				break;
			default:
				clog("Generalmente non dovremmo essere qui!");
				clog(QString("m_usersManager->checkUserRegister(m_packet->getUser())") + QString::number((qint32)r));
				break;
		}
	} catch (Exception &e) {
		clog("Exception Occurred!");
		clog(e.what());
		QStringList errors;
		errors << QString(e.what()) << m_usersManager->lastSqlError() << m_usersManager->lastDriverError();
		m_packet->answerToUserRegister(CommandPacket::ServerInternalError, errors);
	}
	sendCommand();
}

void KCloud::WorkerServer::resourcePerm(){

}

void KCloud::WorkerServer::resourceShare(){

	clog(QString("Resource sharing request from ") + m_socket->peerAddress().toString());
	if(userIsLogged()){
		try{
			QStringList failsSharing;
			ResourcesManager::ResourcesManagerAnswer r = m_resourcesManager->shareResource(*m_user, m_packet->getFirstResourceHeader(), failsSharing);

			switch (r) {
				case ResourcesManager::ResourceNotExist:
					clog("Resource sharing, invalid id");
					m_packet->answerToResourceSharing(CommandPacket::ResourceSharingInvalidId);
					break;
				case ResourcesManager::PermError:
					clog("Resource sharing, invalid perm");
					m_packet->answerToResourceSharing(CommandPacket::ResourceSharingInvalidPerm);
					break;
				case ResourcesManager::SharingOk:
					if(failsSharing.isEmpty()){
						clog("Resource sharing ok");
						m_packet->answerToResourceSharing(CommandPacket::ResourceSharingOk);
					}else{
						clog("Resource sharing, Errors.....");
						m_packet->answerToResourceSharing(CommandPacket::ResourceSharingErrors, failsSharing);
					}
					break;
				default:
					clog("Generalmente non dovremmo essere qui!");
					clog(QString("m_resourcesManager->shareResource(*m_user, m_packet->getFirstResourceHeader(), failsSharing));") + QString::number((qint32)r));
					m_packet->answerToResourceSharing(CommandPacket::ResourceSharingFail);
					break;
			}
		}catch(Exception &e){
			clog("Exception Occurred!");
			clog(e.what());
			QStringList errors;
			errors << QString(e.what()) << m_resourcesManager->lastSqlError() << m_resourcesManager->lastDriverError();
			m_packet->answerToResourceSharing(CommandPacket::ServerInternalError, errors);
		}
	}else{
		m_packet->answerToResourceSharing(CommandPacket::NotLoggedUser);
	}
	sendCommand();
}

void KCloud::WorkerServer::passwordChange(){			// nuovo arrivo

	clog(QString("Password change request from ") + m_socket->peerAddress().toString());

	if(userIsLogged() && (m_packet->getUser().getEmail() == m_user->getEmail())){
		try{
			UsersManager::UsersManagerAnswer r = m_usersManager->checkPasswordChange(m_packet->getUser());

			switch (r){
				case UsersManager::UserPasswordChangeOk:
					clog("Password change Ok!");
					m_packet->answerToPasswordChange(CommandPacket::PasswordChangeOk);
					break;
				default:
					clog("Generalmente non dovremmo essere qui!");
					clog(QString("m_usersManager->checkPasswordChange(m_packet->getUser())") + QString::number((qint32)r));
					m_packet->answerToPasswordChange(CommandPacket::PasswordChangeFail);
					break;
			}
		}catch(Exception &e){
			clog("Exception Occurred!");
			clog(e.what());
			QStringList errors;
			errors << QString(e.what()) << m_resourcesManager->lastSqlError() << m_resourcesManager->lastDriverError();
			m_packet->answerToPasswordChange(CommandPacket::PasswordChangeFail, errors);
		}
	}else{
		m_packet->answerToPasswordChange(CommandPacket::NotLoggedUser);
	}
	sendCommand();
}

void KCloud::WorkerServer::forcedLogout(){			////Completata <Sistemare Stringhe errori>

	if(m_user == NULL){
		return;
	}
	try{
		m_usersManager->forceLogout(*m_user);
	}catch(Exception &e){
		clog("Exception Occurred!");
		clog(e.what());
		clog(m_usersManager->lastDriverError());
		clog(m_usersManager->lastSqlError());
	}
}

void KCloud::WorkerServer::finalizeUpload(){		////Completata ma meglio riguardare poi

	clog(QString("Resource received from ") + m_socket->peerAddress().toString());
	clog(QString("Finalizing for ") + m_socket->peerAddress().toString());
	m_resource->decompress(m_head);
	m_resource->clear();
	try{
		QStringList errors;

		m_packet->answerToResourceUp((	m_resourcesManager->addResources(m_dir.path(), m_head, errors) == ResourcesManager::FinalizeOK ?
										CommandPacket::ResourceUpFinalizeOk : CommandPacket::ResourceUpFinalizeFail),
										errors);
		foreach(QString err, errors){
			clog(QString("User not founded: ") + err);

		}
	}catch(Exception &e){
		clog("Exception Occurred!");
		clog(e.what());
		clog(m_resourcesManager->lastDriverError());
		clog(m_resourcesManager->lastSqlError());
	}

	disconnect	(m_packet, SIGNAL(objectSended()), this, SLOT(receiveResource()	)						);
	connect		(m_packet, SIGNAL(objectSended()), this, SLOT(receiveCommand()	), Qt::UniqueConnection	);

	sendCommand();
}

void KCloud::WorkerServer::finalizeDownload(){

	clog(QString("Resource sended to ") + m_socket->peerAddress().toString());
	clog(QString("Finalizing for ") + m_socket->peerAddress().toString());
	clog(QString("Removing ") + m_resource->getResourcePath());
	clog((recursiveRemove(m_resource->getResourcePath()) ? QString("OK") : (Console::Red + QString("FAIL") + Console::Reset)));
	clog(QString("Removing ") + m_resource->getZipPath());
	clog((m_resource->removeZipFile() ? QString("OK") : (Console::Red + QString("FAIL") + Console::Reset)));
	clog("Download request processed.");

	disconnect	(m_packet	, SIGNAL(objectSended()), this, SLOT(sendResource()		)						);
	connect		(m_packet	, SIGNAL(objectSended()), this, SLOT(receiveCommand()	), Qt::UniqueConnection	);

	receiveCommand();
}

QString KCloud::WorkerServer::address() const{

	return QString("0x%1").arg((quintptr)this, QT_POINTER_SIZE * 2, 16, QChar('0'));
}

QString KCloud::WorkerServer::keyFirst() const{

	return m_key1;
}

QString KCloud::WorkerServer::keyLast() const{
	return m_key2;
}

bool KCloud::WorkerServer::userIsLogged() const{

	return (!(m_user == NULL) && m_user->isLogged());
}

void KCloud::WorkerServer::clog(const QString &log){

	QString str(Console::Green + this->metaObject()->className() + Console::Reset + QString(" [") + address() + QString("] :"));
	str += " ";
	str += log;
//	emit consoleOutRequest(str);
}

bool KCloud::WorkerServer::recursiveRemove(const QString &path){


	QFileInfo info(path);

	if(info.exists()){
		if(info.isFile()){

			if(!QFile::remove(info.absoluteFilePath())){
				clog(QString("[F] ") + Console::Red + QString("Error while removing : ") + Console::Reset + info.absoluteDir().relativeFilePath(info.fileName()));
				return false;
			}else{
				clog(QString("[F] Removed : ") + info.absoluteDir().relativeFilePath(info.fileName()));
				return true;
			}
		}else{
			QDir dir(path);
			bool res;
			foreach (QFileInfo item, dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::Hidden | QDir::NoDotAndDotDot)) {

				res  = recursiveRemove(item.absoluteFilePath());
			}
			if(!dir.rmdir(dir.path())){

				clog(QString("[D] ") + Console::Red + QString("Error while removing : ") + Console::Reset + info.absoluteDir().relativeFilePath(info.fileName()));
				return false;
			}else{

				clog(QString("[D] Removed : ") + info.absoluteDir().relativeFilePath(info.fileName()));
				return (true && res);
			}
		}
	}else{
		return false;
	}
}
