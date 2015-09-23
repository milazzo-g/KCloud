#include "UsersManager.h"
#include <QDebug>

KCloud::UsersManager::UsersManager(const QString &name, QObject *parent) : DatabaseManager(name, parent){

}

KCloud::UsersManager::~UsersManager(){
	qDebug() << __FUNCTION__;
}

KCloud::UsersManager::UsersManagerAnswer KCloud::UsersManager::checkLogin(const KCloud::User &usr) throw (Exception){

	if(open()){
		QSqlQuery query(m_db);
		query.prepare(" SELECT * FROM users WHERE email = :email ");
		query.bindValue(":email", usr.getEmail());
		if(!query.exec()){
			m_lastSqlError		= query.lastError().databaseText();
			m_lastDriverError	= query.lastError().driverText();
			throw QueryFailure();
		}
		switch (query.size()) {
			case 0:
				return UserNotFound;
			case 1:
				query.seek(0);
				if(query.value(1).toString() == usr.getHash()){
					if(query.value(3).toString() == "Logged"){
						return UserAlreadyLogged;
					}
					m_user.m_email	= usr.getEmail();
					m_user.m_hash	= usr.getHash();
					m_user.m_space	= query.value(2).toLongLong();
					m_user.m_state	= true;
					query.clear();
					query.prepare("UPDATE users SET status = 'Logged' WHERE email = :email ");
					query.bindValue(":email", m_user.m_email);
					if(!query.exec()){
						m_lastSqlError		= query.lastError().databaseText();
						m_lastDriverError	= query.lastError().driverText();
						qDebug() << m_lastSqlError;
						qDebug() << m_lastDriverError;
						throw QueryFailure();
					}
					return UserOK;
				}else{
					return UserWrongHash;
				}
				break;
			default:
				//sono successe cose gravissime
				break;
		}
	}else{
		throw OpenFailure();
	}
	return UserAlreadyLogged;
}
