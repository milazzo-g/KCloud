#include "UsersManager.h"
#include <QDebug>


const QString KCloud::UsersManager::queryUser_1(" SELECT * FROM users WHERE email = :email ");
const QString KCloud::UsersManager::queryUser_2(" UPDATE users SET status = :status WHERE email = :email ");
const QString KCloud::UsersManager::queryUser_3(" SELECT * FROM users WHERE email = :email ");
const QString KCloud::UsersManager::queryUser_4(" INSERT INTO users VALUES ( :email, :hash, 0, 'UnLogged') ");
const QString KCloud::UsersManager::queryUser_5(" INSERT INTO resources (parent, owner, name, type, size) VALUES (1, :owner, :name, 'Dir', 0) ");
const QString KCloud::UsersManager::queryUser_6(" UPDATE users SET hash = :hash WHERE email = :email ");

KCloud::UsersManager::UsersManager(const QString &name, QObject *parent) : DatabaseManager(name, parent){

}

KCloud::UsersManager::~UsersManager(){
//	qDebug() << __FUNCTION__;
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

KCloud::UsersManager::UsersManagerAnswer KCloud::UsersManager::checkUserRegister(const KCloud::User &usr) throw(Exception){

	if(open()){

		if(!userExists(usr)){

			QSqlQuery query(m_db);
			query.prepare(queryUser_4);
			query.bindValue(placeHolder_mail, usr.getEmail());
			query.bindValue(placeHolder_hash, usr.getHash());
			tryExec(query);
			query.clear();
			query.prepare(queryUser_5);
			query.bindValue(placeHolder_owner, usr.getEmail());
			query.bindValue(placeHolder_name, usr.getEmail());
			tryExec(query);
			close();
			return UserRegisterOk;
		}else{

			close();
			return UsernameAlreadyInUse;
		}

	}else{

		throw OpenFailure();
	}
}

KCloud::UsersManager::UsersManagerAnswer KCloud::UsersManager::checkPasswordChange(const KCloud::User &usr) throw (Exception){

	if(open()){

		QSqlQuery query(m_db);
		query.prepare(queryUser_6);
		query.bindValue(placeHolder_hash, usr.getHash());
		query.bindValue(placeHolder_mail, usr.getEmail());
		tryExec(query);
		close();
		return UserPasswordChangeOk;
	}else{

		throw OpenFailure();
	}
}

KCloud::User KCloud::UsersManager::getUser(const QString &mail){

	if(open()){
		QSqlQuery query(m_db);
		query.prepare(queryUser_1);
		query.bindValue(placeHolder_mail, mail);
		tryExec(query);
		query.seek(0);
		usrCopy(query);
		close();
		return m_user;
	}else{

		throw OpenFailure();
	}
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

bool KCloud::UsersManager::userExists(const KCloud::User &usr) throw(Exception){

	return userExists(usr.getEmail());
}

bool KCloud::UsersManager::userExists(const QString &usr) throw(Exception){

	if(isOpen()){
		QSqlQuery query(m_db);
		query.prepare(queryUser_3);
		query.bindValue(placeHolder_mail, usr);
		tryExec(query);
		if(!query.size()){
			return false;
		}else if(query.size() == 1){
			return true;
		}else{
			throw MultipleRowsForPrimaryKey();
		}
	}else{
		throw OpenFailure();
	}
}
