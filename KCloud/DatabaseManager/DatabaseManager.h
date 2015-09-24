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

		private:
			const static	QString			m_username;
			const static	QString			m_password;
			const static	QString			m_hostname;
			const static	QString			m_database;
			const static	int				m_hostport;
							bool			m_openFlag;
		protected:
			enum userTableValue{
				Email,
				Hash,
				Space,
				Status
			};
			const	static	QString			queryUser_1;
			const	static	QString			queryUser_2;

			const	static	QString			placeHolder_mail;
			const	static	QString			placeHolder_hash;
			const	static	QString			placeHolder_space;
			const	static	QString			placeHolder_status;
			const	static	QString			sqlEnumLogged;
			const	static	QString			sqlEnumUnLogged;
							QSqlDatabase	m_db;
							QString			m_lastDriverError;
							QString			m_lastSqlError;

	};
}

#endif // DATABASEMANAGER_H
