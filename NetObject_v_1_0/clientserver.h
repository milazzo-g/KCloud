#ifndef CLIENTSERVER_H
#define CLIENTSERVER_H

#include "NetObject.h"
#include "Resource.h"

#include <QList>
#include <QThread>
#include <QTcpServer>
#include <QTcpSocket>
#include <QAbstractSocket>


class ClientServer : public QThread{
	Q_OBJECT

	public:
		explicit ClientServer(QObject *parent = 0);

	signals:

	public slots:

	protected:
		QTcpSocket *channel;
		KCloud::Resource *res;
};

class WorkerServer : public ClientServer{
	Q_OBJECT

	public:
		explicit WorkerServer(int fd, QObject *parent = 0);

	protected slots:
		void notificaBytes();
		void notificaRicezione();

};


class Client : public ClientServer{
	Q_OBJECT

	public:
		Client(const QString &host, const quint16 &port, QObject *parent = 0);

	public slots:
		void	init();
		void	finish();
		void	sendData();
		void	handleStateChange(QAbstractSocket::SocketState state);


	protected:

	private:

		QString host;
		quint16 port;

};

class MainServer : public QTcpServer{
	Q_OBJECT
	public:
		explicit MainServer(QObject *parent = 0);

	protected:
		void incomingConnection(qintptr sock);
	private:
		QList<WorkerServer *> threads;
};


#endif // CLIENTSERVER_H
