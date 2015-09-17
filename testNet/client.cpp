#include "client.h"
#include <QDebug>


WorkDispatcher::WorkDispatcher(QObject *parent) : QThread(parent), consoleOut(stdout), consoleIn(stdin){
	coreApplication = QCoreApplication::instance();
}

void WorkDispatcher::run(){

}

Server::Server(QObject *parent) : ClientServerApplication(parent){

	server = new QTcpServer(this);
}

void Server::work(){
	server->listen(QHostAddress::Any, 5000);
	if(server->isListening()){
		connect(server, SIGNAL(newConnection()), this, SLOT(newConnectionAccepted()));
		qDebug() << "In ascolto su porta 5000...";
	}else{
		qDebug() << "Errore nella socket...";
	}
}

void Server::newConnectionAccepted(){

	qDebug() << "Nuova connesione accettata!";
	qDebug() << "In attesa della risorsa!";
	obj = new KCloud::Resource(this);
	KCloud::Resource * tmp = reinterpret_cast<KCloud::Resource *>(obj);
	connect(tmp, SIGNAL(objectReceived()), this, SLOT(trasmissionEnd()), Qt::UniqueConnection);
	if(tmp){
		tmp->newFile("/home/giuseppe/Scrivania/ricevuto.txt");
	}
	tmp->receiveFrom(*(server->nextPendingConnection()));

}

void Server::trasmissionEnd()
{
	qDebug() << "Risorsa ricevuta!";
}

ClientServerApplication::ClientServerApplication(QObject *parent) : QThread(parent){

}

Client::Client(QObject *parent) : ClientServerApplication(parent){

	sock = new QTcpSocket(this);
}

void Client::work(){

	sock->connectToHost("127.0.0.1", 5000);
	if(sock->waitForConnected()){
		qDebug() << "Connesso a localhost...";
		obj = new KCloud::Resource(this);
		KCloud::Resource * res = reinterpret_cast<KCloud::Resource *>(obj);
		if(res){
			res->setFile("/home/giuseppe/Scrivania/prova.txt");
		}
		connect(res, SIGNAL(objectSended()), this, SLOT(trasmissionEnd()), Qt::UniqueConnection);
		res->sendThrough(*sock);
	}else{
		qDebug() << "Errore nella socket...";
	}
}

void Client::trasmissionEnd(){
	qDebug() << "Risorsa inviata...";
}
