#include "WorkerServer.h"
#include "QDebug"

#include "../MainServer/defines.h"

KCloud::WorkerServer::WorkerServer(int sd, QObject *parent) : Engine(parent){

	QSettings appSettings;
	m_socket->setSocketDescriptor(sd);
	m_dir.cd(appSettings.value(INSTANCES).toString());
	m_dir.mkdir(keyFirst());
	m_dir.cd(keyFirst());
	m_resourcesManager	= new ResourcesManager(keyFirst(), this);
	m_usersManager		= new UsersManager(keyLast(), this);
	connect(m_socket, SIGNAL(disconnected()), this, SLOT(quit()));
}

KCloud::WorkerServer::~WorkerServer(){
	m_dir.rmdir(m_dir.path());
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
		default:
			break;
	}
}

void KCloud::WorkerServer::login(){					////Completata ma meglio riguardare poi

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

	try {
		UsersManager::UsersManagerAnswer r = (m_user == NULL ? UsersManager::UserNotFound : m_usersManager->checkLogout(*m_user));
		switch (r){
			case UsersManager::LogoutOK:
				clog("Logout Ok!");
				if(m_user){
					delete m_user;
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
		m_packet->answerToLogin(CommandPacket::ServerInternalError, m_packet->getUser(), errors);
	}

	connect(m_packet, SIGNAL(objectSended()), this, SLOT(receiveCommand()), Qt::UniqueConnection);
	sendCommand();
}

void KCloud::WorkerServer::resourceUp(){

	clog(QString("Resource Upload request from ") + m_socket->peerAddress().toString());

	try{

	}catch(Exception &e){

	}

}

void KCloud::WorkerServer::resourceMod(){

}

void KCloud::WorkerServer::resourceDel(){

}

void KCloud::WorkerServer::resourceTree(){

}

void KCloud::WorkerServer::resourceDown(){

}

void KCloud::WorkerServer::userRegister(){

}

void KCloud::WorkerServer::resourcePerm(){

}

void KCloud::WorkerServer::resourceShare(){

}

void KCloud::WorkerServer::passwordChange(){

}

QString KCloud::WorkerServer::address() const{

	return QString("0x%1").arg((quintptr)this, QT_POINTER_SIZE * 2, 16, QChar('0'));
}

QString KCloud::WorkerServer::keyFirst() const{


	return QCryptographicHash::hash(address().toLocal8Bit(), QCryptographicHash::Md5).toHex();
}

QString KCloud::WorkerServer::keyLast() const{
	return QCryptographicHash::hash(keyFirst().toLocal8Bit(), QCryptographicHash::Md5).toHex();
}

void KCloud::WorkerServer::clog(const QString &log){

	QString str(Console::Green + this->metaObject()->className() + Console::Reset + QString(" [") + address() + QString("] :"));
	str += " ";
	str += log;
	emit consoleOutRequest(str);
}
