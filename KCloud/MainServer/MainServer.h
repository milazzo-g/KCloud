#ifndef MAINSERVER_H
#define MAINSERVER_H

/*
 *
 * GIUSEPPE
 *
 */

#include "mainserver_global.h"

#include <QObject>
#include <QtNetwork/QTcpServer>
#include <QCoreApplication>
#include "../Console/Console.h"

namespace KCloud{

	class MAINSERVERSHARED_EXPORT MainServer : public QTcpServer{
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
}

#endif // MAINSERVER_H
