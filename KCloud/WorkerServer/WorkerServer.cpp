#include "WorkerServer.h"
#include "QDebug"

KCloud::WorkerServer::WorkerServer(int sd, QObject *parent) : Engine(parent){

	m_socket->setSocketDescriptor(sd);
}

KCloud::WorkerServer::~WorkerServer(){
	qDebug() << "WorkerServer Stopped";
}

void KCloud::WorkerServer::run(){
	clog("Spawned!");
	//se non dovesse funzionare è perchè l'event loop non è ancora partito (almeno penso)
	//quindi possiamo provare con QTimer
	connect(m_packet, SIGNAL(objectReceived()), this, SLOT(parse()));
	receiveCommand();
	exec();
}

void KCloud::WorkerServer::parse(){

	switch (m_packet->getClientCommand()) {
		case CommandPacket::Login:
			login();
			break;
		default:
			break;
	}
}

void KCloud::WorkerServer::login(){
	//facciamo finta che è tutto ok...

	m_packet->answerToLogin(CommandPacket::LoginOk, m_packet->getUser());
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

void KCloud::WorkerServer::clog(const QString &log){

	QString str(Console::Green + this->metaObject()->className() + Console::Reset);
	str += " ";
	str += log;
	emit consoleOutRequest(str);
}
