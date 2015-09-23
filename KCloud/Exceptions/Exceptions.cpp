#include "Exceptions.h"

/*
 *		######### Eccezioni
 *
*/
const char *KCloud::Exception::what() const throw (){

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

/*
 *		######### Eccezioni utente
 *
*/

KCloud::Exception::ExceptionSubclass KCloud::UserException::subclass() const{

	return User;
}

/*
 *		Formato mail non valido
 *
*/
const char *KCloud::BadMailException::what() const throw (){

	return "UserException : Email address pattern mismatch!";
}

KCloud::Exception::Type KCloud::BadMailException::type() const{

	return UserBadMail;
}

/*
 *		Password vuota
 *
*/
const char *KCloud::EmptyPasswordException::what() const throw (){

	return "UserException : Empty password!";
}

KCloud::Exception::Type KCloud::EmptyPasswordException::type() const{

	return UserEmptyPassword;
}

/*
 *		Lunghezza hash errata
 *
*/
const char *KCloud::BadHashLengthException::what() const throw (){

	return "UserException : Hash length mismatch!";
}

KCloud::Exception::Type KCloud::BadHashLengthException::type() const{

	return UserBadHashLength;
}

/*
 *		######### Eccezioni risorse
 *
*/
KCloud::Exception::ExceptionSubclass KCloud::ResourceException::subclass() const{

	return Resource;
}

/*
 *		Percorso vuoto
 *
*/
const char *KCloud::EmptyPathException::what() const throw (){

	return "ResourceException : Path is empty!";
}

KCloud::Exception::Type KCloud::EmptyPathException::type() const{

	return ResourceEmptyPath;
}

/*
 *		Percorso non valido
 *
*/
const char *KCloud::BadPathException::what() const throw (){

	return "ResourceException : Bad path!";
}

KCloud::Exception::Type KCloud::BadPathException::type() const{

	return ResourceBadPath;
}

/*
 *		Nome file già in uso
 *
*/
const char *KCloud::AlreadyFileNameInUseException::what() const throw (){

	return "ResourceException : File name already in use!";
}

KCloud::Exception::Type KCloud::AlreadyFileNameInUseException::type() const{

	return ResourceAlreadyFileNameInUse;
}

/*
 *		Errore durante la compressione
 *
*/
const char *KCloud::ZippingErrorException::what() const throw (){

	return "ResourceException : Error during zipping file";
}

KCloud::Exception::Type KCloud::ZippingErrorException::type() const{

	return ResourceZippingError;
}

/*
 *		Errore durante la decompressione
 *
*/
const char *KCloud::UnZippingErrorException::what() const throw (){

	return "ResourceException : Error during unzipping file";
}

KCloud::Exception::Type KCloud::UnZippingErrorException::type() const{

	return ResourceUnZippingError;
}

/*
 *		Risorsa non pronta
 *
*/
const char *KCloud::NotReadyException::what() const throw (){

	return "ResourceException : Not ready to send";
}

KCloud::Exception::Type KCloud::NotReadyException::type() const{

	return ResourceNotReady;
}

/*
 *		######### Eccezioni NetObject
 *
*/
KCloud::Exception::ExceptionSubclass KCloud::NetObjectException::subclass() const{

	return NetObject;
}

/*
 *		Socket NULL o non aperta
 *
*/
const char *KCloud::InvalidSocket::what() const throw (){

	return "NetObjectException : Invalid socket";
}

KCloud::Exception::Type KCloud::InvalidSocket::type() const{

	return NetObjectInvalidSocket;
}

/*
 *		Contatore bytes non attendibile
 *
*/
const char *KCloud::UntrustedBytesCounter::what() const throw (){

	return "NetObjectException : Invalid bytes counter";
}

KCloud::Exception::Type KCloud::UntrustedBytesCounter::type() const{

	return NetObjectUntrustedBytesCounter;
}

/*
 *		######### Eccezioni base
 *
*/
KCloud::Exception::ExceptionSubclass KCloud::UnknownException::subclass() const{

	return Basic;
}

const char *KCloud::UnknownException::what() const throw (){

	return "Exception : UnknownException";
}

KCloud::Exception::Type KCloud::UnknownException::type() const{

	return Unknown;
}

/*
 *		Eccezioni Client
 *
 */

KCloud::Exception::ExceptionSubclass KCloud::ClientException::subclass() const
{
	return Client;
}

const char *KCloud::NullUserPointer::what() const throw (){
	return "ClientException : User not set or NULL!";
}

KCloud::Exception::Type KCloud::NullUserPointer::type() const{

	return ClientNullUserPointer;
}

const char *KCloud::UnreachableServer::what() const throw (){

	return "ClientException : Server unreachable!";
}

KCloud::Exception::Type KCloud::UnreachableServer::type() const{

	return ClientUnreachableServer;
}
