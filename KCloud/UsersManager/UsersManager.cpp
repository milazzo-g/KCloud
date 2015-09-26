#include "UsersManager.h"
#include <QDebug>


const QString KCloud::UsersManager::queryUser_1(" SELECT * FROM users WHERE email = :email ");
const QString KCloud::UsersManager::queryUser_2(" UPDATE users SET status = :status WHERE email = :email ");

KCloud::UsersManager::UsersManager(const QString &name, QObject *parent) : DatabaseManager(name, parent){

}

KCloud::UsersManager::~UsersManager(){
	qDebug() << __FUNCTION__;
}

void KCloud::UsersManager::forceLogout(const KCloud::User &usr) throw (Exception){

	if(open()){
		QSqlQuery query(m_db);
		query.prepare(queryUser_2);
		query.bindValue(placeHolder_status, sqlEnumUnLogged);
		query.bindValue(placeHolder_mail, usr.getEmail());
		tryExec(query);
	}else{
		throw OpenFailure();
	}
}

KCloud::UsersManager::UsersManagerAnswer KCloud::UsersManager::checkLogin(const KCloud::User &usr) throw (Exception){

	if(open()){
		QSqlQuery query(m_db);
		query.prepare(queryUser_1);
		query.bindValue(placeHolder_mail, usr.getEmail());
		tryExec(query);
		switch (query.size()){
			case 0:
				return UserNotFound;
			case 1:
				query.seek(0);
				if(query.value(DatabaseManager::Hash).toString() == usr.getHash()){
					usrCopy(query);
					if(m_user.isLogged()){
						return UserAlreadyLogged;
					}
					query.clear();
					query.prepare(queryUser_2);
					query.bindValue(placeHolder_status, sqlEnumLogged);
					query.bindValue(placeHolder_mail, m_user.getEmail());
					tryExec(query);
					m_user.setLogged();
					return LoginOK;
				}
				return UserWrongHash;
			default:
				throw MultipleRowsForPrimaryKey();
				break;
		}
	}else{
		throw OpenFailure();
	}
	return UserNotFound;
}

KCloud::UsersManager::UsersManagerAnswer KCloud::UsersManager::checkLogout(const KCloud::User &usr) throw (Exception){

	if(m_db.open()){
		QSqlQuery query(m_db);
		query.prepare(queryUser_1);
		query.bindValue(placeHolder_mail, usr.getEmail());
		tryExec(query);
		switch (query.size()) {
			case 0:
				return UserNotFound;
			case 1:
				query.seek(0);
				if(query.value(DatabaseManager::Hash).toString() == usr.getHash()){
					usrCopy(query);
					if(!m_user.isLogged()){
						return UserAlreadyUnLogged;
					}
					query.clear();
					query.prepare(queryUser_2);
					query.bindValue(placeHolder_status, sqlEnumUnLogged);
					query.bindValue(placeHolder_mail, m_user.getEmail());
					tryExec(query);
					return LogoutOK;
				}
				return UserWrongHash;
			default:
				throw MultipleRowsForPrimaryKey();
				break;
		}
	}else{
		throw OpenFailure();
	}
	return UserNotFound;
}

KCloud::User KCloud::UsersManager::getUser() const{

	return m_user;
}

void KCloud::UsersManager::usrCopy(QSqlQuery &query){
	m_user.m_email	= query.value(DatabaseManager::Email).toString();
	m_user.m_hash	= query.value(DatabaseManager::Hash).toString();
	m_user.m_space	= query.value(DatabaseManager::Space).toLongLong();
	m_user.m_state	= query.value(DatabaseManager::Status).toString() == sqlEnumLogged ? true : false;
}
