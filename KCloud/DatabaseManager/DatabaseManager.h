#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include "databasemanager_global.h"

#include <QObject>
#include <QString>
#include <QStringList>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlDatabase>

#include "../Exceptions/Exceptions.h"

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

			enum resourceTableValue{
				res_Id,
				res_Parent,
				res_Owner,
				res_Name,
				res_Type,
				res_Size
			};

			enum sharingTableValue{
				sha_User,
				sha_Resource,
				sha_Permission
			};

			const	static	QString			sqlEnumDir;
			const	static	QString			sqlEnumFile;
			const	static	QString			sqlEnumRead;
			const	static	QString			sqlEnumWrite;
			const	static	QString			sqlEnumLogged;
			const	static	QString			sqlEnumUnLogged;
			const	static	QString			placeHolder_mail;
			const	static	QString			placeHolder_hash;
			const	static	QString			placeHolder_space;
			const	static	QString			placeHolder_status;
			const	static	QString			placeHolder_permission;

			const	static	QString			placeHolder_id;
			const	static	QString			placeHolder_parent;
			const	static	QString			placeHolder_owner;
			const	static	QString			placeHolder_name;
			const	static	QString			placeHolder_type;
			const	static	QString			placeHolder_size;

							QSqlDatabase	m_db;
							QString			m_lastDriverError;
							QString			m_lastSqlError;

							void			tryExec(QSqlQuery &query) throw (Exception);
	};
}

#endif // DATABASEMANAGER_H
