#include "rootserver.h"
#include <QDebug>

#define out qDebug()

RootServer::RootServer(QObject *parent) : QTcpServer(parent){

}

void RootServer::incomingConnection(qintptr socketDescriptor){

	out << "Nuova Connessione Accettata!";
	WorkerServer * tmp = new WorkerServer(socketDescriptor, this);
	//connect(tmp, SIGNAL(finished()), this, SLOT(deleteLater()));
	tmp->start();
	//threads << tmp;
}


WorkerServer::WorkerServer(int socketDescriptor, QObject *parent) : QThread(parent), channel(NULL), packet(NULL){

	channel = new QTcpSocket(this);
	channel->setSocketDescriptor(socketDescriptor);
}

WorkerServer::~WorkerServer(){

	out << "Thread: morto";
	channel->close();
	delete channel;
	delete packet;
}

void WorkerServer::run(){

	out << "Nuovo Thread!";
	packet = new Resource();
	packet->moveToThread(this);
	Resource * tmp = reinterpret_cast<Resource *>(packet);
	tmp->newFile("/home/giuseppe/Scrivania/TestRicezione.pdf");
	tmp->receiveFrom(*channel);
	connect(packet, SIGNAL(objectReceived()), this, SLOT(end()));
	exec();
}

void WorkerServer::end(){

	out << "Thread: risorsa ricevuta";
	this->quit();
}
