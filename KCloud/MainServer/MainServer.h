#ifndef MAINSERVER_H
#define MAINSERVER_H

/*
 *
 * GIUSEPPE
 *
 */

#include "mainserver_global.h"

#include <QDir>
#include <QMap>
#include <QList>
#include <QObject>
#include <QSettings>
#include <QCoreApplication>
#include <QtNetwork/QTcpServer>

#include "../Console/Console.h"
#include "../WorkerServer/WorkerServer.h"

namespace KCloud{

	class MAINSERVERSHARED_EXPORT MainServer : public QTcpServer{
		Q_OBJECT
		public:
			explicit									MainServer(QObject *parent = 0);
														~MainServer();
						void							start();
		protected:
						void							incomingConnection(qintptr handle);

		private slots:
						void							execCommand(const QString &cmd);
						void							removeHandler(const QString &add);
		private:
						void							clog(const QString &log);

						QMap<QString, WorkerServer *>	m_clientsHandlers;
						QCoreApplication *				m_coreApplication;
						Console	*						m_console;
	};
}

#endif // MAINSERVER_H
