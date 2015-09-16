#include "user.h"
#include <QCryptographicHash>
#include <QRegExp>


const qint64	KCloud::User::totalSpace = 4294967296;
const QString	KCloud::User::emailRegEx = "\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b"
										   "";
const char *KCloud::Exceptions::UserException::what() const throw (){
	return "UserException: Unknown Exception Occured!";
}

KCloud::Exceptions::UserException::Type KCloud::Exceptions::UserException::type() const{
	return UnknownException;
}

KCloud::Exceptions::UserException *KCloud::Exceptions::UserException::clone() const{
	return new UserException(*this);
}

void KCloud::Exceptions::UserException::raise() const{
	throw *this;
}

class EmailPatternMismatchException : public KCloud::Exceptions::UserException{
		virtual const char *	what()	const throw ()	{ return "UserException: The value passed is not a valid email address!";	}
		virtual	Type			type()	const			{ return UserException::EmailPatternMismatch;								}
};

class EmptyEmailException : public KCloud::Exceptions::UserException{
		virtual const char *	what()	const throw ()	{ return "UserException: Email address can't be empty!";					}
		virtual	Type			type()	const			{ return UserException::EmptyEmail;											}
};

class EmptyPasswordException : public KCloud::Exceptions::UserException{
		virtual const char *	what()	const throw ()	{ return "UserException: Password or hash can't be empty!";					}
		virtual	Type			type()	const			{ return UserException::EmptyPassword;										}
};

class HashSizeException : public KCloud::Exceptions::UserException{
		virtual const char *	what()	const throw ()	{ return "UserException: The length of the hash must be 32!";				}
		virtual	Type			type()	const			{ return UserException::HashSize;											}
};

class NegativeSpaceException : public KCloud::Exceptions::UserException{
		virtual const char *	what()	const throw ()	{ return "UserException: Occupied space can't be negative!";				}
		virtual	Type			type()	const			{ return UserException::NegativeSpace;										}
};

KCloud::User::User(const QString email, const QString pass, KCloud::User::Mode mode) throw (KCloud::Exceptions::UserException){

	setEmail(email);
	setHash(pass, mode);
}

KCloud::User::User(const KCloud::User &cpy){

	m_email		= cpy.m_email;
	m_hash		= cpy.m_hash;
	m_space		= cpy.m_space;
	m_status	= cpy.m_status;
}

QString KCloud::User::getEmail() const{

	return m_email;
}

QString KCloud::User::getHash() const{

	return m_hash;
}

qint64 KCloud::User::getFreeSpace() const{

	return (User::totalSpace - m_space);
}

qint64 KCloud::User::getNetworkSize() const{

	QByteArray	buff;
	QDataStream test(buff);

	test << *this;
	return buff.size();
}

qint64 KCloud::User::getOccupiedSpace() const{

	return m_space;
}

bool KCloud::User::isLogged() const{

	return m_status;
}

void KCloud::User::setHash(const QString pass, KCloud::User::Mode mode) throw (KCloud::Exceptions::UserException){

	if(pass.isEmpty()){

		throw EmptyPasswordException();
	}

	if(mode == NotEncrypt){

		if(pass.length() != 32){

			throw HashSizeException();
		}else{

			m_hash = pass;
		}
	}else{

		m_hash = QCryptographicHash::hash(pass.toLocal8Bit(), QCryptographicHash::Md5).toHex();
	}
}

void KCloud::User::setEmail(const QString email) throw (KCloud::Exceptions::UserException){

	QRegExp validator(User::emailRegEx, Qt::CaseInsensitive, QRegExp::RegExp);

	if(email.isEmpty()){

		throw EmptyEmailException();
	}

	if(!validator.exactMatch(email)){

		throw EmailPatternMismatchException();
	}

	m_email = email;
}

void KCloud::User::setLogged(){

	m_status = true;
}

void KCloud::User::setUnlogged(){

	m_status = false;
}

QDataStream &KCloud::operator <<(QDataStream &out, const KCloud::User &usr){

	out << usr.m_email << usr.m_hash << usr.m_space << usr.m_status;
	return out;
}

QDataStream &KCloud::operator >>(QDataStream &inp, KCloud::User &usr){

	inp >> usr.m_email >> usr.m_hash >> usr.m_space >> usr.m_status;
	return inp;
}
