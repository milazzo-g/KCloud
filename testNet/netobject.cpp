#include "netobject.h"
#include <QFileInfo>
#include <QDebug>

#define __1MB__ 1048576

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

	channel = &sock;
	connect(channel, SIGNAL(readyRead()), this, SLOT(behaviorOnRecv()), Qt::UniqueConnection);
}

bool KCloud::Resource::setFile(const QString path){

	QFileInfo info(path);
	if(!info.exists() || info.isDir()){
		return false;
	}
	clear();
	file = new QFile(path);
	return true;
}

bool KCloud::Resource::newFile(const QString path){

	QFileInfo info(path);
	if(info.exists()){
		return false;
	}
	file = new QFile(path);
	return true;
}

void KCloud::Resource::behaviorOnSend(const qint64 dim){

	qDebug() << "Client: inviati " << dim << " bytes";
	packetSize -= dim;
	if(packetSize == 0){
		currentBlock++;

		if(currentBlock > packetsOnSend + 1){
			qDebug() << "Client: dentro if";
			disconnect(channel, SIGNAL(bytesWritten(qint64)), this, SLOT(behaviorOnSend(qint64)));
			disconnect(this, SIGNAL(changeBlockOnSend(qint64)), this, SLOT(send(qint64)));
			clear();
			emit objectSended();
		}else{
			qDebug() << "Client: dentro else";
			emit changeBlockOnSend(currentBlock);
		}
	}
}

void KCloud::Resource::behaviorOnRecv(){

	if(currentBlock == 0){
		if(channel->bytesAvailable() < (qint64)sizeof(packetSize)){
			return;
		}
		fileOpen();
		QDataStream stream(channel);
		stream >> packetSize;
		qDebug() << "Ricevuta dimensione packetSize " << packetSize;
		currentBlock++;
	}

	packetSize -= file->write(channel->readAll());
	qDebug() << "Rimanenti " << packetSize << " bytes";
	if(packetSize == 0){
		disconnect(channel, SIGNAL(readyRead()), this, SLOT(behaviorOnRecv()));
		clear();
		emit objectReceived();
	}
}

void KCloud::Resource::send(const qint64 block){

	currentBlock = block;
	if(currentBlock == 0){
		fileOpen();
		QDataStream	stream(channel);
		bytesToSend			= calculateNetworkSize();
		packetsOnSend		= bytesToSend / __1MB__;
		spareBytesOnSend	= bytesToSend % __1MB__;
		packetSize			= sizeof(bytesToSend);
		qDebug() << "---------------- CLIENT BLOCCO " << currentBlock << " -------------------";
		qDebug() << "Client: valore bytesToSend      = " << bytesToSend;
		qDebug() << "Client: valore packetsOnSend    = " << packetsOnSend;
		qDebug() << "Client: valore spareBytesOnSend = " << spareBytesOnSend;
		qDebug() << "Client: valore packetSize       = " << packetSize;
		stream << bytesToSend;
		return;
	}
	if(currentBlock <= packetsOnSend){
		packetSize = __1MB__;
		channel->write(file->read(packetSize));
		qDebug() << "---------------- CLIENT BLOCCO " << currentBlock << " -------------------";
		qDebug() << "Client: valore bytesToSend      = " << bytesToSend;
		qDebug() << "Client: valore packetsOnSend    = " << packetsOnSend;
		qDebug() << "Client: valore spareBytesOnSend = " << spareBytesOnSend;
		qDebug() << "Client: valore packetSize       = " << packetSize;
	}else{
		packetSize = spareBytesOnSend;
		channel->write(file->read(spareBytesOnSend));
		qDebug() << "---------------- CLIENT BLOCCO " << currentBlock << " -------------------";
		qDebug() << "Client: valore bytesToSend      = " << bytesToSend;
		qDebug() << "Client: valore packetsOnSend    = " << packetsOnSend;
		qDebug() << "Client: valore spareBytesOnSend = " << spareBytesOnSend;
		qDebug() << "Client: valore packetSize       = " << packetSize;
	}
}

void KCloud::Resource::recv(const qint64 block){
	Q_UNUSED(block)
}

bool KCloud::Resource::fileOpen(){

	if(file){
		return file->open(QIODevice::ReadWrite);
	}
	return false;
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
}
