#include "NetObject.h"

NetObject::NetObject(QObject *parent) : QObject(parent)
{

}


qint64 KCloud::NetObject::getNetworkSize(){
	return calculateNetworkSize();
}

void KCloud::NetObject::clear(){
	bytesPerPacket	= 0;
	bytesCounter	= 0;
	packets			= 0;
	spareBytes		= 0;
	currentBlock	= 0;
	channel			= NULL;
}

void KCloud::NetObject::prepare(){
	clear();
	packets		= getNetworkSize() / bytesPerPacket;
	spareBytes	= getNetworkSize() % bytesPerPacket;
	setReady();
}

void KCloud::NetObject::sendThrough(QTcpSocket *sock){

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
