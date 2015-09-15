#include "User.h"

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
	virtual const char * what() const throw () {}
	virtual KCloud::UserException::Type type() const {}

};
*/
class EmptyMailException : public KCloud::UserException{
		virtual const char *				what() const throw ()	{ return "UserException: Empty mail!";					}
		virtual KCloud::UserException::Type type() const			{ return KCloud::UserException::UserEmptyMailException;	}
};

class EmptyHashException : public KCloud::UserException{
		virtual const char *				what() const throw ()	{ return "UserException: Empty hash!";					}
		virtual KCloud::UserException::Type type() const			{ return KCloud::UserException::UserEmptyHashException; }
};
