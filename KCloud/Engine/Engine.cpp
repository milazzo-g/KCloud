#include "Engine.h"

KCloud::Engine::Engine(QObject *parent) : QThread(parent){
	m_socket	= new QTcpSocket(this);
	m_resource	= new Resource(this);
	m_packet	= new CommandPacket(this);
}
