#ifndef CLIENTSERVER_H
#define CLIENTSERVER_H

#include "NetObject.h"
#include "Resource.h"

#include <QThread>
#include <QTcpServer>
#include <QTcpSocket>

class ClientServer : public QThread{
	Q_OBJECT

	public:
		explicit ClientServer(QObject *parent = 0);


	signals:

	public slots:


	protected:

		QTcpSocket *channel;
		KCloud::NetObject *res;

};

class WorkerServer : public ClientServer{
	Q_OBJECT

	public:
		explicit WorkerServer(int fd, QObject *parent = 0);

	protected:
		void run();


};


class Client : public ClientServer{
	Q_OBJECT

	public:
		explicit Client(QObject *parent = 0);
		QString address;
		quint16 port;

	protected:
		void run();

};

class MainServer : public QTcpServer{
	Q_OBJECT

	public:
		explicit MainServer(QObject *parent = 0);

	protected:
		void incomingConnection(qintptr sock);


};


#endif // CLIENTSERVER_H
