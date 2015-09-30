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
				UserAlreadyUnLogged,
				UsernameAlreadyInUse,
				UserRegisterOk,
				UserPasswordChangeOk

			};
			explicit							UsersManager(const QString &name, QObject *parent = 0);
												~UsersManager();
							void				forceLogout(const User &usr) throw (Exception);
							UsersManagerAnswer	checkLogin(const User &usr) throw (Exception);
							UsersManagerAnswer	checkLogout(const User &usr) throw (Exception);
							UsersManagerAnswer	checkUserRegister(const User &usr) throw(Exception);
							UsersManagerAnswer	checkPasswordChange(const User &usr) throw(Exception);
							User				getUser(const QString &mail);
							User				getUser() const;
		private:
							User				m_user;

							void				usrCopy(QSqlQuery &query);
							bool				userExists(const User &usr) throw (Exception);
							bool				userExists(const QString &usr) throw (Exception);

			const static	QString				queryUser_1;
			const static	QString				queryUser_2;
			const static	QString				queryUser_3;
			const static	QString				queryUser_4;
			const static	QString				queryUser_5;
			const static	QString				queryUser_6;
	};
}

#endif // USERSMANAGER_H
