#include "netobject.h"
#include <QFileInfo>
#include <QDebug>

KCloud::NetObject::NetObject(QObject *parent) : QObject(parent){

	clear();
}

qint64 KCloud::NetObject::getNetworkSize(){

	return calculateNetworkSize();
}

bool KCloud::NetObject::isReady(){
	return this->readyFlag;
}

void KCloud::NetObject::clear(){

	bytesToSend			= 0;
	bytesToRecv			= 0;
	spareBytesOnSend	= 0;
	spareBytesOnRecv	= 0;
	packetsOnSend		= 0;
	packetsOnRecv		= 0;
	readyFlag			= 0;
	currentBlock		= 0;
	packetSize			= 0;
	channel				= NULL;
}

void KCloud::NetObject::setReady(){
	this->readyFlag = true;
}

void KCloud::NetObject::setNotReady(){
	this->readyFlag = false;
}

/*
##########################################################################################
##########################################################################################
*/

KCloud::Resource::Resource(QObject *parent) : NetObject(parent), file(NULL){

}

void KCloud::Resource::sendThrough(QTcpSocket &sock){

	channel	= &sock;
	connect(channel, SIGNAL(bytesWritten(qint64)), this, SLOT(behaviorOnSend(qint64)), Qt::UniqueConnection);
	connect(this, SIGNAL(changeBlockOnSend(qint64)), this, SLOT(send(qint64)), Qt::UniqueConnection);
	send();

}

void KCloud::Resource::receiveFrom(QTcpSocket &sock){
	Q_UNUSED(sock);

}

bool KCloud::Resource::setFile(const QString path){

	QFileInfo info(path);
	if(!info.exists() || info.isDir()){
		return false;
	}
	clear();
	file = new QFile(path);
	return file->open(QIODevice::ReadWrite);
}

void KCloud::Resource::behaviorOnSend(const qint64 dim){
	packetSize -= dim;
	if(dim == 0){
		currentBlock++;

		if(currentBlock > packetsOnSend + 1){
			emit objectSended();
		}else{
			emit changeBlockOnSend(currentBlock);
		}
	}
}

void KCloud::Resource::behaviorOnRecv(const qint64 dim){
	Q_UNUSED(dim);
}

void KCloud::Resource::send(const qint64 block){

	currentBlock = block;
	if(currentBlock == 0){
		QDataStream	stream(channel);
		bytesToSend			= calculateNetworkSize();
		packetsOnSend		= bytesToSend / 4096;
		spareBytesOnSend	= bytesToSend % 4096;
		packetSize			= sizeof(qint64);
		stream << bytesToSend;
	}
	if(currentBlock <= packetsOnSend){
		packetSize = 4096;
		channel->write(file->read(4096));
	}else{
		packetSize = spareBytesOnSend;
		channel->write(file->read(spareBytesOnSend));
	}
	if(currentBlock == )

}

void KCloud::Resource::recv(const qint64 block){

}

qint64 KCloud::Resource::calculateNetworkSize(){

	if(!file){
		return -1;
	}
	return QFileInfo(*file).size();
}

void KCloud::Resource::clear(){

	NetObject::clear();
	if(file){
		file->close();
		delete file;
	}
	buff.clear();
}
