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
	bytesCounter	= 0;
	packets			= 0;
	spareBytes		= 0;
	currentBlock	= 0;
	channel			= NULL;
	setNotReady();
}

void KCloud::NetObject::prepareForSend(){
	clear();
	packets		= getNetworkSize() / bytesPerPacket;
	spareBytes	= getNetworkSize() % bytesPerPacket;

	if(!packets){
		bytesCounter = spareBytes;
	}else{
		bytesCounter = getBytesPerPacket();
	}

	setReady();
}

void KCloud::NetObject::sendThrough(QTcpSocket *sock){

	if(isReady()){
		if(sock && sock->isOpen()){
			channel = sock;
			connect(channel, SIGNAL(bytesWritten(qint64)), this, SLOT(behaviorOnSend(qint64)), Qt::UniqueConnection);
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
		channel = sock;
		connect(channel,	SIGNAL(readyRead()),		this, SLOT(recv()),			Qt::UniqueConnection);
		connect(this,		SIGNAL(objectReceived()),	this, SLOT(leaveSocket()),	Qt::UniqueConnection);
	}else{
		//lanciare eccezione perchè la socket è NULL oppure perchè non ci si può leggere
	}

}

void KCloud::NetObject::setBytesPerPacket(KCloud::NetObject::Payload payload){
	switch (payload) {
		case Payload_128B:
			bytesPerPacket = 128;
			return;
		case Payload_256B:
			bytesPerPacket = 256;
			return;
		case Payload_512B:
			bytesPerPacket = 512;
			return;
		case Payload_1KB:
			bytesPerPacket = 1024;
			return;
		case Payload_2KB:
			bytesPerPacket = 2048;
			return;
		case Payload_5KB:
			bytesPerPacket = 5120;
			return;
		case Payload_10KB:
			bytesPerPacket = 10240;
			return;
		case Payload_20KB:
			bytesPerPacket = 20480;
			return;
		case Payload_50KB:
			bytesPerPacket = 51200;
			return;
		case Payload_128KB:
			bytesPerPacket = 131072;
			return;
		case Payload_256KB:
			bytesPerPacket = 262144;
			return;
		case Payload_512KB:
			bytesPerPacket = 524288;
			return;
		case Payload_1MB:
			bytesPerPacket = 1048576;
			return;
		default:
			//lanciare eccezione sconosciuta!
			break;
	}
}

qint64 KCloud::NetObject::getBytesPerPacket() const{

	return bytesPerPacket;
}

void KCloud::NetObject::leaveSocket(){

	disconnect(channel, SIGNAL(bytesWritten(qint64)),	this, SLOT(behaviorOnSend(qint64))	);
	disconnect(channel, SIGNAL(readyRead()),			this, SLOT(recv())					);
	disconnect(this,	SIGNAL(changeBlock(qint64)),	this, SLOT(send(qint64))			);
	clear();
}

void KCloud::NetObject::setReady(){

	readyFlag = true;
}

void KCloud::NetObject::setNotReady(){

	readyFlag = false;
}

bool KCloud::NetObject::isReady() const{

	return readyFlag;
}


