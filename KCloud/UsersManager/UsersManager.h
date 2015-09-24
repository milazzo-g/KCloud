#ifndef USERSMANAGER_H
#define USERSMANAGER_H

#include "usersmanager_global.h"

#include <QList>

#include "../User/User.h"
#include "../Exceptions/Exceptions.h"
#include "../DatabaseManager/DatabaseManager.h"

namespace KCloud{

	class USERSMANAGERSHARED_EXPORT UsersManager : public DatabaseManager{
		Q_OBJECT
		public:
			enum UsersManagerAnswer{
				LoginOK,
				LogoutOK,
				UserNotFound,
				UserWrongHash,
				UserAlreadyLogged,
				UserAlreadyUnLogged
			};
			explicit							UsersManager(const QString &name, QObject *parent = 0);
												~UsersManager();
							UsersManagerAnswer	checkLogin(const User &usr) throw (Exception);
							UsersManagerAnswer	checkLogout(const User &usr) throw (Exception);
							User				getUser() const;
		private:

			const static	QString				queryUser_1;
			const static	QString				queryUser_2;
							void				tryExec(QSqlQuery &query) throw (Exception);
							void				usrCopy(QSqlQuery &query);
							User				m_user;
	};
}

#endif // USERSMANAGER_H
