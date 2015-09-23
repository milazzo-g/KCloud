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

void KCloud::WorkerServer::clog(const QString &log){

	QString str(Console::Green + this->metaObject()->className() + Console::Reset);
	str += " ";
	str += log;
	emit consoleOutRequest(str);
}
