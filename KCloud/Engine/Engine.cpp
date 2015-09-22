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

void KCloud::Engine::notifySocketState(QAbstractSocket::SocketState stat){

	emit socketStateChanged(stat);
}
