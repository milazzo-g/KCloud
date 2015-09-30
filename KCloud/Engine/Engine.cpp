#include "Engine.h"
#include <QDebug>
#include "../MainServer/defines.h"

KCloud::Engine::Engine(QObject *parent) : QThread(parent){

	m_socket	= new QTcpSocket(this);
	m_resource	= new Resource(this);
    m_packet	= new CommandPacket(this);
	m_user		= NULL;
	connect(m_socket,	SIGNAL(connected()), this, SLOT(notifyConnect()));
	connect(m_socket,	SIGNAL(disconnected()), this, SLOT(notifyDisconnect()));
	connect(m_socket,	SIGNAL(error(QAbstractSocket::SocketError)),
			this,		SLOT(notifySocketError(QAbstractSocket::SocketError)));
}

KCloud::Engine::~Engine(){

//	qDebug() << "Engine: stopped!";
}

void KCloud::Engine::sendCommand(){

//	trace;
	QThread::msleep(500);
	m_packet->prepareForSend();
	m_packet->sendThrough(m_socket);
}

void KCloud::Engine::sendResource(){

//	trace;
	QThread::msleep(500);
	m_resource->prepareForSend();
	QThread::msleep(500);
	emit compressionEnd();
	m_resource->sendThrough(m_socket);
}

void KCloud::Engine::receiveCommand(){
//	trace;
	emit commandSended();
	m_packet->prepareForRecv();
	m_packet->receiveFrom(m_socket);
}

void KCloud::Engine::receiveResource(){
//	trace;
	m_resource->prepareForRecv();
	m_resource->receiveFrom(m_socket);
}

void KCloud::Engine::parse(){

}

void KCloud::Engine::login(){

}

void KCloud::Engine::logout(){

}

void KCloud::Engine::resourceUp(){

}

void KCloud::Engine::resourceMod(){

}

void KCloud::Engine::resourceDel(){

}

void KCloud::Engine::resourceTree(){

}

void KCloud::Engine::resourceDown(){

}

void KCloud::Engine::userRegister(){

}

void KCloud::Engine::resourcePerm(){

}

void KCloud::Engine::resourceShare(){

}

void KCloud::Engine::passwordChange(){

}

void KCloud::Engine::notifyConnect(){

	emit connected();
}

void KCloud::Engine::notifyDisconnect(){

	emit disconnected();
}

void KCloud::Engine::notifySocketError(QAbstractSocket::SocketError error){

	emit socketError(error);
}
