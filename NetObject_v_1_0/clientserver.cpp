#include "clientserver.h"
#include <QDebug>

ClientServer::ClientServer(QObject *parent) : QThread(parent), channel(new QTcpSocket(this)), res(new KCloud::Resource(this)){
}


MainServer::MainServer(QObject *parent) : QTcpServer(parent){
}

void MainServer::incomingConnection(qintptr sock){

	WorkerServer *thread = new WorkerServer(sock, this);
	qDebug() << "Nuova connessione accettata!";
}

WorkerServer::WorkerServer(int fd, QObject *parent) : ClientServer(parent){

	channel->setSocketDescriptor(fd);

}
