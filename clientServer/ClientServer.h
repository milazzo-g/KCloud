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
			QCoreApplication	*m_coreApplication;
			Console				*m_console;

	};
}

#endif // CLIENTSERVER_H
