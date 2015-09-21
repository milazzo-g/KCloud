#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QString>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDriver>
#include <QStringList>
#include <QSqlDatabase>
namespace KCloud{

	class DatabaseManager : public QObject{
			Q_OBJECT
		public:
			explicit		DatabaseManager(QObject *parent = 0);
							DatabaseManager(const QString &userName,
											const QString &password,
											const QString &hostAddr,
											const quint16 &hostPort,
											const QString &dataName,
											QObject * parent = 0);

					void	setConnectionParameters(const QString &userName,
													const QString &password,
													const QString &hostAddr,
													const quint16 &hostPort,
													const QString &dataName);
					bool	testConnection();

		signals:

		public slots:

		private:
			QSqlDatabase	m_db;
			QString			m_lastSqlErrorString;
			QSqlError		m_lastSqlError;

	};
}

#endif // DATABASEMANAGER_H
