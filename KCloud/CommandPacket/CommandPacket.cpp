#include "CommandPacket.h"

KCloud::CommandPacket::CommandPacket(QObject *parent) : NetObject(parent){

	clear();
}

void KCloud::CommandPacket::clear(){

	m_stringList.clear();
	m_currentUser.clear();
	m_headersList.clear();
	m_serverAnswer	=	UnsetAnswer;
	m_clientCommand =	UnsetCommand;
}

void KCloud::CommandPacket::prepareForSend() throw(Exception){

}

void KCloud::CommandPacket::prepareForRecv(){

}

void KCloud::CommandPacket::setForLogin(const KCloud::User &usr){

	clear();
	setUser(usr);
	setClientCommand(Login);

}

void KCloud::CommandPacket::setForLogout(const KCloud::User &usr){

	clear();
	setUser(usr);
	setClientCommand(Logout);
}

void KCloud::CommandPacket::setUser(const KCloud::User &usr){

	m_currentUser = usr;
}

void KCloud::CommandPacket::setClientCommand(KCloud::CommandPacket::ClientCommand cmd){

	m_clientCommand = cmd;
}

void KCloud::CommandPacket::setServerAnswer(KCloud::CommandPacket::ServerAnswer cmd){

	m_serverAnswer = cmd;
}

void KCloud::CommandPacket::setErrorString(const QString &err){

	m_stringList << err;
}

KCloud::User KCloud::CommandPacket::getUser() const{

	return m_currentUser;
}

void KCloud::CommandPacket::answerToLogin(const KCloud::User &usr, KCloud::CommandPacket::ServerAnswer answer, const QString &errorString){

	clear();
	setUser(usr);
	setServerAnswer(answer);
	setErrorString(errorString);
}

void KCloud::CommandPacket::send(const qint64 block){

}

void KCloud::CommandPacket::recv() throw(Exception){

}

void KCloud::CommandPacket::behaviorOnSend(const qint64 dim) throw(Exception){

}

qint64 KCloud::CommandPacket::calculateNetworkSize() throw(Exception){

}
