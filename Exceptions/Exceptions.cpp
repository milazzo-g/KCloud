#include "Exceptions.h"

const char *KCloud::Exception::what() const{

	return "Exception : General message!";
}

KCloud::Exception::Type KCloud::Exception::type() const{

	return Core;
}

KCloud::Exception::ExceptionSubclass KCloud::Exception::subclass() const{

	return Basic;
}

KCloud::Exception *KCloud::Exception::clone() const{

	return new Exception(*this);
}

void KCloud::Exception::raise() const{

	throw *this;
}

KCloud::Exception::ExceptionSubclass KCloud::UserException::subclass() const{

	return User;
}

const char *KCloud::BadMailException::what() const{

	return "UserException : Email address pattern mismatch!";
}

KCloud::Exception::Type KCloud::BadMailException::type() const{

	return UserBadMail;
}

const char *KCloud::EmptyPasswordException::what() const{

	return "UserException : Empty password!";
}

KCloud::Exception::Type KCloud::EmptyPasswordException::type() const{

	return UserEmptyPassword;
}

const char *KCloud::BadHashLengthException::what() const{

	return "UserException : Hash length mismatch!";
}

KCloud::Exception::Type KCloud::BadHashLengthException::type() const{

	return UserBadHashLength;
}

const char *KCloud::UnknownException::what() const{

	return "UserException : Unknown!";
}

KCloud::Exception::Type KCloud::UnknownException::type() const{

	return Unknown;
}

KCloud::Exception::ExceptionSubclass KCloud::ResourceException::subclass() const{

	return Resource;
}

const char *KCloud::EmptyPathException::what() const{

	return "ResourceException : Path is empty!";
}

KCloud::Exception::Type KCloud::EmptyPathException::type() const{

	return ResourceEmptyPath;
}


const char *KCloud::BadPathException::what() const{

	return "ResourceException : Bad path!";
}

KCloud::Exception::Type KCloud::BadPathException::type() const{

	return ResourceBadPath;
}
