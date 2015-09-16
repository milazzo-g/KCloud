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

class HashToLongException : public KCloud::Exceptions::UserException{
		virtual const char *	what()	const throw ()	{ return "UserException: The length of the hash must be 32!";				}
		virtual	Type			type()	const			{ return UserException::HashToLong;											}
};

class NegativeSpaceException : public KCloud::Exceptions::UserException{
		virtual const char *	what()	const throw ()	{ return "UserException: Occupied space can't be negative!";				}
		virtual	Type			type()	const			{ return UserException::NegativeSpace;										}
};

KCloud::User::User(const QString email, const QString pass, KCloud::User::Mode mode) throw (KCloud::Exceptions::UserException){

}

KCloud::User::User(const KCloud::User &cpy){

}

QString KCloud::User::getEmail() const{

}

QString KCloud::User::getHash() const{

}

qint64 KCloud::User::getFreeSpace() const{

}

qint64 KCloud::User::getNetworkSize() const{

}

qint64 KCloud::User::getOccupiedSpace() const{

}

bool KCloud::User::isLogged() const{

}

void KCloud::User::setHash(const QString pass, KCloud::User::Mode mode) throw (KCloud::Exceptions::UserException){

}

void KCloud::User::setEmail(const QString email) throw (KCloud::Exceptions::UserException){
	if(email.isEmpty()){
		throw EmptyEmailException();
	}
}

void KCloud::User::setLogged(){

}

void KCloud::User::setUnlogged(){

}

QDataStream &KCloud::operator <<(QDataStream &out, const KCloud::User &usr){

}

QDataStream &KCloud::operator >>(QDataStream &inp, KCloud::User &usr){

}

QDataStream &KCloud::operator <<(QDataStream &out, KCloud::User::Mode &mod){

}

QDataStream &KCloud::operator >>(QDataStream &inp, KCloud::User::Mode &mod){

}
