#include "WorkerServer.h"
#include "QDebug"

KCloud::WorkerServer::WorkerServer(int sd, QObject *parent) : Engine(parent){

	m_socket->setSocketDescriptor(sd);
	m_resourcesManager	= new ResourcesManager(keyFirst(), this);
	m_usersManager		= new UsersManager(keyLast(), this);
}

KCloud::WorkerServer::~WorkerServer(){
	qDebug() << "WorkerServer Stopped";
}

void KCloud::WorkerServer::run(){

	clog("Spawned");
	connect(m_packet, SIGNAL(objectReceived()), this, SLOT(parse()));
	receiveCommand();
	exec();
}

void KCloud::WorkerServer::parse(){

	clog("Command received!");

	switch (m_packet->getClientCommand()) {
		case CommandPacket::Login:
			login();
			break;
		default:
			break;
	}
}

void KCloud::WorkerServer::login(){

	clog("Login Request!");
	switch (m_usersManager->checkLogin(m_packet->getUser())){
		case UsersManager::UserOK:
			clog("Collegato");
			break;
		case UsersManager::UserAlreadyLogged:
			clog("Già loggato!");
			break;
		case UsersManager::UserNotFound:
			clog("User non trovato!");
			break;
		case UsersManager::UserWrongHash:
			clog("Password errata!");
			break;
		default:
			clog("Dio ci ha voluto male!");
			break;
	}
	receiveCommand();
}

void KCloud::WorkerServer::logout(){

}

void KCloud::WorkerServer::resourceUp(){

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
