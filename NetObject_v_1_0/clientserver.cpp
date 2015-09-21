#include "clientserver.h"
#include <QDebug>

ClientServer::ClientServer(QObject *parent) : QThread(parent), channel(new QTcpSocket(this)), res(new KCloud::Resource(this)){

}


MainServer::MainServer(QObject *parent) : QTcpServer(parent){
}

void MainServer::incomingConnection(qintptr sock){

	qDebug() << "Nuova connessione accettata!";
	threads << new WorkerServer(sock, this);
	threads.last()->start();
}

WorkerServer::WorkerServer(int fd, QObject *parent) : ClientServer(parent){

	channel->setSocketDescriptor(fd);
	qDebug() << "Thread costruito!";
	res->setZipDir("/home/giuseppe/Scrivania");
	res->setZipName("testTrasmissione1");
	res->prepareForRecv();
	res->receiveFrom(channel);
	connect(channel, SIGNAL(readyRead()), this, SLOT(notificaBytes()));
	connect(res, SIGNAL(objectReceived()), this, SLOT(notificaRicezione()));
	qDebug() << "Connect eseguite!";
}

void WorkerServer::notificaBytes(){
	qDebug() << "Ricevuti " << channel->bytesAvailable();
}

void WorkerServer::notificaRicezione(){
	qDebug() << "******* RICEVUTO *******";
}


Client::Client(const QString &host, const quint16 &port, QObject *parent) : ClientServer(parent){

	this->host = host;
	this->port = port;
}

void Client::init(){

	channel->connectToHost(host, port);
	if(channel->isOpen()){
		qDebug() << "Connessione stabilita";
	}else{
		qDebug() << "Connessione fallita";
	}
}
