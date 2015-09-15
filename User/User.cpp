#include "User.h"
#include <QCryptographicHash>
#include <QRegExp>

#define __4GB__	4294967296

const char *KCloud::UserException::what() const throw (){

	return "UserException: basic exception!";
}

KCloud::UserException::Type KCloud::UserException::type() const{

	return UserBasicException;
}

void KCloud::UserException::raise() const{

	throw *this;
}

KCloud::UserException *KCloud::UserException::clone() const{

	return new UserException(*this);
}

/*
class  : public KCloud::UserException{

	virtual const char *					what() const throw ()	{ return ;}
	virtual KCloud::UserException::Type		type() const			{ return ;}
};
*/

class EmptyMailException : public KCloud::UserException{
		virtual const char *				what() const throw ()	{ return "UserException: Empty mail!";								}
		virtual KCloud::UserException::Type type() const			{ return KCloud::UserException::UserEmptyMailException;				}
};

class EmptyHashException : public KCloud::UserException{
		virtual const char *				what() const throw ()	{ return "UserException: Empty hash!";								}
		virtual KCloud::UserException::Type type() const			{ return KCloud::UserException::UserEmptyHashException;				}
};

class EmailPatternMismatchException : public KCloud::UserException{
		virtual const char *				what() const throw ()	{ return "UserException: Email pattern mismatch!";					}
		virtual KCloud::UserException::Type	type() const			{ return KCloud::UserException::UserEmailPatternMismatchException;	}
};

class KCloud::UserData{

	public:
	QString email;
	QString hash;
	qint64	space;
	bool	status;
};

KCloud::User::User(const QString &email, const QString &pass, KCloud::User::Mode mode) throw (UserException) : data(new UserData()){

	QRegExp validator("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b", Qt::CaseInsensitive);
	if(email.isEmpty()){
		throw EmptyMailException();
	}
	if(pass.isEmpty()){
		throw EmptyHashException();
	}
	if(!validator.exactMatch(email)){
		throw EmailPatternMismatchException();
	}
	if(mode == Password){
		data->hash = QCryptographicHash::hash(pass.toLocal8Bit(), QCryptographicHash::Md5).toHex();
	}else{
		data->hash = pass;
	}
	data->email		= email;
	data->space		= 0;
	data->status	= 0;
}

KCloud::User::~User(){
	delete data;
}

QString KCloud::User::getMail() const{
	return data->email;
}

QString KCloud::User::getHash() const{
	return data->hash;
}

qint64 KCloud::User::getOccupiedSpace() const{
	return data->space;
}

qint64 KCloud::User::getFreeSpace() const{
	return (__4GB__ - data->space);
}

bool KCloud::User::isLogged() const{
	return data->status;
}

void KCloud::User::setLogged(){
	data->status = true;
}

void KCloud::User::setUnlogged(){
	data->status = false;
}
