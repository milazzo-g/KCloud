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

namespace KCloud{

	class MainServer : public QTcpServer{
		Q_OBJECT
		public:
			explicit	MainServer(QObject *parent = 0);
						~MainServer();

		public slots:
			void		execCommand(const QString &cmd);

		private:
			QCoreApplication *	m_coreApplication;
			Console	*			m_console;

	};

	class Engine : public QThread{
		Q_OBJECT
		public:
			explicit				Engine(QObject *parent = 0);

		protected:
			QTcpSocket *			m_socket;


	};

	class WorkerServer : public Engine{
		Q_OBJECT
		public:
									WorkerServer(int sd, QObject *parent = 0);
									~WorkerServer();

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
			QCoreApplication *		m_coreApplication;
			Console	*				m_console;
			WorkMode				m_workMode;

	};

}

#endif // CLIENTSERVER_H
