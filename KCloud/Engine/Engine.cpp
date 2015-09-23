#include "Engine.h"
#include <QDebug>

KCloud::Engine::Engine(QObject *parent) : QThread(parent){

	m_socket	= new QTcpSocket(this);
	m_resource	= new Resource(this);
    m_packet	= new CommandPacket(this);
	m_user		= NULL;
	connect(m_socket,	SIGNAL(stateChanged(QAbstractSocket::SocketState)),
			this,		SLOT(notifySocketState(QAbstractSocket::SocketState)));
}

KCloud::Engine::~Engine(){

	qDebug() << "Engine: stopped!";
}

void KCloud::Engine::sendCommand(){

	qDebug() << __FUNCTION__ << "invioOOOOO";
	m_packet->prepareForSend();
	m_packet->sendThrough(m_socket);
}

void KCloud::Engine::sendResource(){

	m_resource->prepareForSend();
	m_resource->sendThrough(m_socket);
}

void KCloud::Engine::receiveCommand(){

	m_packet->prepareForRecv();
	m_packet->receiveFrom(m_socket);
}

void KCloud::Engine::receiveResource(){

	m_resource->prepareForRecv();
	m_resource->receiveFrom(m_socket);
}

void KCloud::Engine::notifySocketState(QAbstractSocket::SocketState stat){

	emit socketStateChanged(stat);
}
