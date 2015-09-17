#ifndef ROOTSERVER_H
#define ROOTSERVER_H

#include <QObject>
#include <QList>
#include <QThread>
#include <QTcpServer>
#include <QTcpSocket>
#include "netobject.h"

using namespace KCloud;
class WorkerServer;

class RootServer : public QTcpServer{
		Q_OBJECT
	public:
		explicit	RootServer(QObject *parent = 0);

	signals:

	public slots:

	protected:

		void	incomingConnection(qintptr socketDescriptor) Q_DECL_OVERRIDE;

	private:
		QList<WorkerServer *> threads;

};

class WorkerServer : public QThread{

	Q_OBJECT

	public:
		explicit	WorkerServer(int socketDescriptor, QObject *parent = 0);
					~WorkerServer();
	signals:

	public slots:

	protected:
		void run();

	private slots:
		void end();

	private:
		QTcpSocket	*	channel;
		NetObject *		packet;
};

#endif // ROOTSERVER_H
