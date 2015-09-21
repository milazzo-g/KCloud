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
		void	run();

};


class Client : public ClientServer{
	Q_OBJECT

	public:
		Client(const QString &host, const quint16 &port, QObject *parent = 0);

	protected:
		void	run();
	private:

};

class MainServer : public QTcpServer{
	Q_OBJECT

	public:
		explicit MainServer(QObject *parent = 0);

	protected:
		void incomingConnection(qintptr sock);


};


#endif // CLIENTSERVER_H
