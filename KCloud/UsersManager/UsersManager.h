#ifndef USERSMANAGER_H
#define USERSMANAGER_H

#include "usersmanager_global.h"

#include "../User/User.h"
#include "../DatabaseManager/DatabaseManager.h"

namespace KCloud{

	class USERSMANAGERSHARED_EXPORT UsersManager : public DatabaseManager{
		Q_OBJECT
		public:
			enum UsersManagerAnswer{
				UserOK,
				UserNotFound,
				UserWrongHash
			};
			explicit						UsersManager(QObject *parent = 0);
						UsersManagerAnswer	checkLogin(const User &usr);
						User				getUser();

	};
}

#endif // USERSMANAGER_H
