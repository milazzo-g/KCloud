#include "User.h"
#define HASHLENGTH	32

bool KCloud::User::checkMail(const QString &mail){

	QString strPatt = "\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b";
	QRegExp control(strPatt, Qt::CaseInsensitive, QRegExp::RegExp);

	return control.exactMatch(mail);
}

KCloud::User::User(QObject *parent) : QObject(parent){

	clear();
}

KCloud::User::User(const QString &mail, const QString &password, KCloud::User::PwdMode mode, QObject *parent) : User(parent){

	setEmail(mail);
	setHash(password, mode);
}

bool KCloud::User::isLogged() const{

	return m_state;
}

void KCloud::User::clear(){

	m_state = false;
	m_space = 0;
	m_email.clear();
	m_hash.clear();
}

void KCloud::User::setLogged(){

	m_state = true;
}

void KCloud::User::setUnLogged(){

	m_state = false;
}

void KCloud::User::setEmail(const QString &email){

	if(User::checkMail(email)){
		m_email = email;
	}else{
		//lanciare eccezione
	}
}

void KCloud::User::setHash(const QString &password, KCloud::User::PwdMode mode){

	if(password.isEmpty()){
		//lanciare eccezione
	}
	if(mode == Encrypt){
		m_hash = QCryptographicHash::hash(password.toLocal8Bit(), QCryptographicHash::Md5).toHex();
	}else if(mode == NotEncrypt){
		if(password.length() != HASHLENGTH){
			//lanciare eccezione
		}
		m_hash = password;
	}else{
		//lancio eccezione enum
	}
}

qint64 KCloud::User::getSpace() const{

	return m_space;
}

QString KCloud::User::getEmail() const{

	return m_email;
}

QString KCloud::User::getHash() const{

	return m_hash;
}

QDataStream &KCloud::operator<<(QDataStream &out, const KCloud::User &tmp){

	out << tmp.m_state << tmp.m_space << tmp.m_email << tmp.m_hash;
	return out;
}

QDataStream &KCloud::operator>>(QDataStream &inp, KCloud::User &tmp){

	inp >> tmp.m_state >> tmp.m_space >> tmp.m_email >> tmp.m_hash;
	return inp;
}
