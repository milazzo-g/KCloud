#include "UsersManager.h"
#include <QDebug>

KCloud::UsersManager::UsersManager(const QString &name, QObject *parent) : DatabaseManager(name, parent){

}

KCloud::UsersManager::~UsersManager(){
	qDebug() << __FUNCTION__;
}

void KCloud::UsersManager::createTestUser(){

	if(open()){
		User u("test@test.it", "test");
		m_query.prepare("INSERT INTO users VALUES (':email', ':hash', 100000, 'UnLogged'");
		m_query.bindValue(":email", u.getEmail());
		m_query.bindValue(":hash", u.getHash());
		m_query.exec();
	}
}

KCloud::UsersManager::UsersManagerAnswer KCloud::UsersManager::checkLogin(const KCloud::User &usr) throw (Exception){

	if(open()){
		m_query.prepare("SELECT * FROM users where email = :email");
		m_query.bindValue(":email", usr.getEmail());
		if(!m_query.exec()){
			m_lastSqlError		= m_query.lastError().databaseText();
			m_lastDriverError	= m_query.lastError().driverText();
			m_query.clear();
			throw QueryFailure();
		}
		switch (m_query.size()) {
			case 0:
				return UserNotFound;
			case 1:
				if(m_query.value("hash").toString() == usr.getHash()){
					if(m_query.value("status").toInt() == 0){
						return UserAlreadyLogged;
					}
					m_user.m_email	= usr.getEmail();
					m_user.m_hash	= usr.getHash();
					m_user.m_space	= m_query.value("space").toLongLong();
					m_user.m_state	= true;
					m_query.clear();
					m_query.prepare("UPDATE users SET status = 'Logged' WHERE email = :email");
					m_query.bindValue(":email", m_user.m_email);
					m_query.exec();
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
