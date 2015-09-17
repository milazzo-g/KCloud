#ifndef CLIENT_H
#define CLIENT_H

#include <QCoreApplication>
#include <QTextStream>
#include <QTcpServer>
#include <QAbstractSocket>
#include <QObject>
#include <QThread>
#include <QStringList>

#include "netobject.h"

class ClientServerApplication : public QThread{
		Q_OBJECT
	public:
		ClientServerApplication(QObject *parent);

	signals:

	public slots:
		virtual void work() = 0;
		virtual void trasmissionEnd() = 0;
	private:
};

class Client : public ClientServerApplication{
		Q_OBJECT
	public:
		Client(QObject *parent = 0);

	signals:

	public slots:
		virtual void work();
		virtual void trasmissionEnd();
	private:
		QTcpSocket *	sock;
		KCloud::NetObject *obj;

};

class Server : public ClientServerApplication{
		Q_OBJECT
	public:
		Server(QObject *parent = 0);

	signals:

	public slots:
		virtual void work();
		virtual void trasmissionEnd();
				void newConnectionAccepted();
	private:
		QTcpServer *		server;
		KCloud::NetObject *	obj;
};

class WorkDispatcher : public QThread{
	Q_OBJECT
	public:
		WorkDispatcher(QObject * parent = 0);
	protected:
		void run();
	private:
		enum Mode{
			Client,
			Server
		};
		Mode						workMode;
		QTextStream					consoleOut;
		QTextStream					consoleIn;
		QCoreApplication *			coreApplication;
		ClientServerApplication *	frontEndApplication;

};

#endif // CLIENT_H
