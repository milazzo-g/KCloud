#include "Exceptions.h"

KCloud::Exception *KCloud::Exception::clone() const{

	return new Exception(*this);
}

void KCloud::Exception::raise() const{

	const throw *this;
}

KCloud::Exception::ExceptionSubclass KCloud::UserException::subclass() const{

	return UserException;
}

const char *KCloud::BadMailException::what() const{

	return "UserException : email address pattern mismatch!";
}

KCloud::Exception::Type KCloud::BadMailException::type() const{

	return UserBadMail;
}
