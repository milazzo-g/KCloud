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
	connect(m_resource	, SIGNAL(objectReceived()	), this, SLOT(finalizeUpload()), Qt::UniqueConnection	);
}

KCloud::WorkerServer::~WorkerServer(){

	if(m_user){
		forcedLogout();
	}
	removeDir(m_dir.path());
	qDebug() << address() << ": Job ended!";
	emit removeFromActiveHandlers(address());
}

void KCloud::WorkerServer::run(){

	clog("Spawned");
	connect(m_packet, SIGNAL(objectReceived()), this, SLOT(parse()));
	receiveCommand();
	exec();
}

void KCloud::WorkerServer::parse(){

	clog(QString("Command received from ") + m_socket->peerAddress().toString());

	switch (m_packet->getClientCommand()) {
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

	connect(m_packet, SIGNAL(objectSended()), this, SLOT(receiveCommand()), Qt::UniqueConnection);
	sendCommand();
}

void KCloud::WorkerServer::logout(){					////Completata ma meglio riguardare poi

	clog(QString("Logout request from ") + m_socket->peerAddress().toString());

	if(userIsLogged()){
		try {
			UsersManager::UsersManagerAnswer r = (m_user == NULL ? UsersManager::UserNotFound : m_usersManager->checkLogout(*m_user));
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

	}


	connect(m_packet, SIGNAL(objectSended()), this, SLOT(receiveCommand()), Qt::UniqueConnection);
	sendCommand();
}

void KCloud::WorkerServer::resourceUp(){				////Completata ma meglio riguardare poi

	clog(QString("Resource Upload request from ") + m_socket->peerAddress().toString());

	try{
		m_head = m_packet->getFirstResourceHeader();
		ResourcesManager::ResourcesManagerAnswer r = (m_user == NULL ? ResourcesManager::PermError : m_resourcesManager->checkForUpload(*m_user, m_head));
		switch (r) {
			case ResourcesManager::UploadOK:
				m_packet->answerToResourceUp(CommandPacket::ResourceUpOk);
				m_resource->setZipName(m_head.getName());
				m_resource->setZipDir(m_dir.path());

				disconnect	(m_packet, SIGNAL(objectSended()), this, SLOT(receiveCommand())							);
				connect		(m_packet, SIGNAL(objectSended()), this, SLOT(receiveResource()), Qt::UniqueConnection	);

				break;
			case ResourcesManager::PermError:
				m_packet->answerToResourceUp(CommandPacket::ResourceUpInvalidPerm);
				break;
			case ResourcesManager::SpaceFull:
				m_packet->answerToResourceUp(CommandPacket::ResourceUpSpaceExhausted);
				break;
			case ResourcesManager::AlreadyExists:
				m_packet->answerToResourceUp(CommandPacket::ResourceUpFail);
				break;
			default:
				clog("Generalmente non dovremmo essere qui!");
				clog(QString("m_resourcesManager->checkForUpload(*m_user, m_packet->getFirstResourceHeader())") + QString::number((qint32)r));
				break;
		}
	}catch(Exception &e){
		clog("Exception Occurred!");
		clog(e.what());
		QStringList errors;
		errors << QString(e.what()) << m_usersManager->lastSqlError() << m_usersManager->lastDriverError();
		m_packet->answerToLogin(CommandPacket::ServerInternalError, m_packet->getUser(), errors);
	}
	sendCommand();
}

void KCloud::WorkerServer::resourceMod(){

}

void KCloud::WorkerServer::resourceDel(){

}

void KCloud::WorkerServer::resourceTree(){

}

void KCloud::WorkerServer::resourceDown(){

	clog(QString("Resource Download request from ") + m_socket->peerAddress().toString());
	try{
		ResourcesManager::ResourcesManagerAnswer r = m_resourcesManager->aMoreBadassFunction()
	}catch(Exception &e){
		clog("Non credo funzioni!");
		clog(e.what());
	}
}

void KCloud::WorkerServer::userRegister(){

}

void KCloud::WorkerServer::resourcePerm(){

}

void KCloud::WorkerServer::resourceShare(){

}

void KCloud::WorkerServer::passwordChange(){

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

		m_packet->answerToResourceUp((	m_resourcesManager->aBadassFunction(m_dir.path(), m_head, errors) == ResourcesManager::FinalizeOK ?
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

	return !(m_user == NULL);
}

void KCloud::WorkerServer::clog(const QString &log){

	QString str(Console::Green + this->metaObject()->className() + Console::Reset + QString(" [") + address() + QString("] :"));
	str += " ";
	str += log;
	emit consoleOutRequest(str);
}

void KCloud::WorkerServer::removeDir(const QString &path){

	QDir dir(path);

	if(dir.exists()){
		foreach (QFileInfo item, dir.entryInfoList(QDir::NoDotAndDotDot)){
			if(item.isFile()){
				QFile::remove(item.absoluteFilePath());
			}else{
				removeDir(item.absoluteFilePath());
			}
		}
		dir.rmdir(dir.path());
	}else {
		return;
	}
}
