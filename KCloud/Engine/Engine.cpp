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

	qDebug() << __FUNCTION__;
	m_packet->prepareForSend();
	m_packet->sendThrough(m_socket);
}

void KCloud::Engine::sendResource(){
	qDebug() << __FUNCTION__;
	m_resource->prepareForSend();
	m_resource->sendThrough(m_socket);
}

void KCloud::Engine::receiveCommand(){
	qDebug() << __FUNCTION__;
	m_packet->prepareForRecv();
	m_packet->receiveFrom(m_socket);
}

void KCloud::Engine::receiveResource(){
	qDebug() << __FUNCTION__;
	m_resource->prepareForRecv();
	m_resource->receiveFrom(m_socket);
}

void KCloud::Engine::notifySocketState(QAbstractSocket::SocketState stat){

	emit socketStateChanged(stat);
}
