#include "clientserver.h"
#include <QDebug>

ClientServer::ClientServer(QObject *parent) : QThread(parent), channel(new QTcpSocket(this)), res(new KCloud::Resource(this)){

}


MainServer::MainServer(QObject *parent) : QTcpServer(parent){
}

void MainServer::incomingConnection(qintptr sock){

	qDebug() << "Nuova connessione accettata!";
	threads << new WorkerServer(fd, this);
	threads.last()->start();
}

WorkerServer::WorkerServer(int fd, QObject *parent) : ClientServer(parent){

	channel->setSocketDescriptor(fd);
	qDebug() << "Thread costruito!";
	KCloud::Resource * tmp = reinterpret_cast<KCloud::Resource *>(res);
	tmp->setZipDir("/home/giuseppe/Scrivania");
	tmp->setZipName("testTrasmissione1");
}
