#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include "databasemanager_global.h"

#include <QObject>
#include <QString>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDriver>
#include <QStringList>
#include <QtSql/QSqlDatabase>

namespace KCloud{

	class DATABASEMANAGERSHARED_EXPORT DatabaseManager : public QObject{
		Q_OBJECT
		public:
			explicit						DatabaseManager(const QString &name, QObject *parent = 0);
                                            ~DatabaseManager();
							bool			open();
							bool			isOpen() const;
							void			close();
							QString			lastDriverError() const;
							QString			lastSqlError() const;
		protected:
							QSqlQuery		m_query;
							QString			m_lastDriverError;
							QString			m_lastSqlError;

		private:
			const static	QString			m_username;
			const static	QString			m_password;
			const static	QString			m_hostname;
			const static	QString			m_database;
			const static	int				m_hostport;
							QSqlDatabase	m_db;
							bool			m_openFlag;

	};
}

#endif // DATABASEMANAGER_H
