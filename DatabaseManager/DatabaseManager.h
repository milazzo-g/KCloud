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
			explicit						DatabaseManager(QObject *parent = 0);

							bool			open();
							bool			isOpen() const;
							void			close();
							QString			lastDriverError() const;
							QString			lastSqlError() const;
		protected:
							QSqlQuery		m_query;

		private:
			const static	QString			m_username;
			const static	QString			m_password;
			const static	QString			m_hostname;
			const static	QString			m_database;
			const static	int				m_hostport;
							QSqlDatabase	m_db;
							bool			m_openFlag;
							QString			m_lastDriverError;
							QString			m_lastSqlError;

	};
}

#endif // DATABASEMANAGER_H
