#ifndef CLIENTSERVER_H
#define CLIENTSERVER_H

#include <QMap>
#include <QList>
#include <QObject>
#include <QThread>
#include <QString>
#include <QSettings>
#include <QTcpSocket>
#include <QTcpServer>
#include <QTextStream>
#include <QStringList>
#include <QCoreApplication>

#include "Console.h"
#include "../DatabaseManager/DatabaseManager.h"
#include "../NetObject_v_1_0/Commandpacket.h"
#include "../NetObject_v_1_0/Resource.h"

namespace KCloud{

	class MainServer : public QTcpServer{
		Q_OBJECT
		public:
			explicit						MainServer(QObject *parent = 0);
											~MainServer();

		public slots:
						void				execCommand(const QString &cmd);

		private:
						QCoreApplication *	m_coreApplication;
						Console	*			m_console;

	};

	class Engine : public QThread{
		Q_OBJECT
		public:
			explicit						Engine(QObject *parent = 0);

		signals:

		protected slots:
			virtual		void				parse() = 0;

		protected:
						QTcpSocket *		m_socket;
						Resource *			m_resource;
						CommandPacket *		m_packet;
	};

	class WorkerServer : public Engine{
		Q_OBJECT
		public:
											WorkerServer(int sd, QObject *parent = 0);
											~WorkerServer();
		private:
						UsersManager		m_usersManager;
						ResourcesManager	m_resourcesManager;


	};

	class Client : public Engine{
		Q_OBJECT
		public:
			enum WorkMode{
				Thread,
				Standalone
			};
											Client(WorkMode mode, QObject *parent = 0);
		private:
						QCoreApplication *	m_coreApplication;
						Console	*			m_console;
						WorkMode			m_workMode;

	};

}

#endif // CLIENTSERVER_H
