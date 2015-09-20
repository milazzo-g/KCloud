#include "NetObject.h"

KCloud::NetObject::NetObject(QObject *parent) : QObject(parent){

	clear();
	setBytesPerPacket();
	setNotReady();
}

qint64 KCloud::NetObject::getNetworkSize(){
	return calculateNetworkSize();
}

void KCloud::NetObject::clear(){
	m_bytesCounter	= 0;
	m_packets		= 0;
	m_spareBytes	= 0;
	m_currentBlock	= 0;
	m_channel		= NULL;
	setNotReady();
}

void KCloud::NetObject::prepareForSend(){
	clear();
	m_packets		= getNetworkSize() / getBytesPerPacket();
	m_spareBytes	= getNetworkSize() % getBytesPerPacket();
	m_bytesCounter  = (qint64)sizeof(getNetworkSize());
	setReady();
}

void KCloud::NetObject::sendThrough(QTcpSocket *sock){

	if(isReady()){
		if(sock && sock->isOpen()){
			m_channel = sock;
			connect(m_channel, SIGNAL(bytesWritten(qint64)), this, SLOT(behaviorOnSend(qint64)), Qt::UniqueConnection);
			connect(this, SIGNAL(changeBlock(qint64)), this, SLOT(send(qint64)), Qt::UniqueConnection);
			connect(this, SIGNAL(objectSended()), this, SLOT(leaveSocket()), Qt::UniqueConnection);
			send();
		}else{
			//lanciare eccezione perchè la socket è NULL oppure perchè non ci si può scrivere
		}
	}else{
		//lanciare eccezione perchè il pacchetto non è pronto
	}
}

void KCloud::NetObject::receiveFrom(QTcpSocket *sock){

	clear();
	if(sock && sock->isOpen()){
		m_channel = sock;
		connect(m_channel,	SIGNAL(readyRead()),		this, SLOT(recv()),			Qt::UniqueConnection);
		connect(this,		SIGNAL(objectReceived()),	this, SLOT(leaveSocket()),	Qt::UniqueConnection);
	}else{
		//lanciare eccezione perchè la socket è NULL oppure perchè non ci si può leggere
	}

}

void KCloud::NetObject::setBytesPerPacket(KCloud::NetObject::Payload payload){
	switch (payload) {
		case Payload_128B:
			m_bytesPerPacket = 128;
			return;
		case Payload_256B:
			m_bytesPerPacket = 256;
			return;
		case Payload_512B:
			m_bytesPerPacket = 512;
			return;
		case Payload_1KB:
			m_bytesPerPacket = 1024;
			return;
		case Payload_2KB:
			m_bytesPerPacket = 2048;
			return;
		case Payload_5KB:
			m_bytesPerPacket = 5120;
			return;
		case Payload_10KB:
			m_bytesPerPacket = 10240;
			return;
		case Payload_20KB:
			m_bytesPerPacket = 20480;
			return;
		case Payload_50KB:
			m_bytesPerPacket = 51200;
			return;
		case Payload_128KB:
			m_bytesPerPacket = 131072;
			return;
		case Payload_256KB:
			m_bytesPerPacket = 262144;
			return;
		case Payload_512KB:
			m_bytesPerPacket = 524288;
			return;
		case Payload_1MB:
			m_bytesPerPacket = 1048576;
			return;
		default:
			//lanciare eccezione sconosciuta!
			break;
	}
}

qint64 KCloud::NetObject::getBytesPerPacket() const{

	return m_bytesPerPacket;
}

void KCloud::NetObject::leaveSocket(){

	disconnect(m_channel,	SIGNAL(bytesWritten(qint64)),	this, SLOT(behaviorOnSend(qint64))	);
	disconnect(m_channel,	SIGNAL(readyRead()),			this, SLOT(recv())					);
	disconnect(this,		SIGNAL(changeBlock(qint64)),	this, SLOT(send(qint64))			);
	clear();
}

void KCloud::NetObject::setReady(){

	m_readyFlag = true;
}

void KCloud::NetObject::setNotReady(){

	m_readyFlag = false;
}

bool KCloud::NetObject::isReady() const{

	return m_readyFlag;
}


