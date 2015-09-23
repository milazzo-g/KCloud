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
				UserOK,
				UserNotFound,
				UserWrongHash,
				UserAlreadyLogged
			};
			explicit						UsersManager(const QString &name, QObject *parent = 0);
                                            ~UsersManager();
						UsersManagerAnswer	checkLogin(const User &usr) throw (Exception);
						User				getUser();
		private:
						User				m_user;
	};
}

#endif // USERSMANAGER_H
