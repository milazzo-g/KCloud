#include "CommandPacket.h"

KCloud::CommandPacket::CommandPacket(QObject *parent) : NetObject(parent){

	clear();
}

void KCloud::CommandPacket::clear(){

    m_buffer.clear();
	m_stringList.clear();
	m_currentUser.clear();
	m_headersList.clear();
	m_serverAnswer	=	UnsetAnswer;
	m_clientCommand =	UnsetCommand;
}

void KCloud::CommandPacket::prepareForSend() throw(Exception){

    QDataStream tmp(&m_buffer, QIODevice::ReadWrite);       //controllare se funziona
    NetObject::clear();
    tmp << *this;
    m_bytesCounter = (qint64)sizeof(getNetworkSize());
    setReady();
}

void KCloud::CommandPacket::prepareForRecv(){

    CommandPacket::clear();
    setReady();
}

void KCloud::CommandPacket::setForLogin(const KCloud::User &usr){

	clear();
	setUser(usr);
	setClientCommand(Login);

}

void KCloud::CommandPacket::setForLogout(){

	clear();
    setClientCommand(Logout);
}

void KCloud::CommandPacket::setForResourceDel(const quint64 &id){

    clear();
    setFirstResourceHeader(ResourceHeader(id));
    setClientCommand(ResourceDel);
}

void KCloud::CommandPacket::setForResourceTree(){

	clear();
	setClientCommand(ResourceTree);
}

void KCloud::CommandPacket::setForResourceUp(const QString &localPath,
											 const KCloud::User &sessionUser,
											 const quint64 &parentId,
											 const QMap<QString, KCloud::ResourceHeader::ResourcePerm> &permissionTable,
											 KCloud::ResourceHeader::ResourcePerm publicPerm) throw(Exception){

	clear();
	setFirstResourceHeader(ResourceHeader(localPath, sessionUser, parentId, permissionTable, publicPerm));
	setClientCommand(ResourceUp);
}

void KCloud::CommandPacket::setForResourceDown(const quint64 &resourceId){

	clear();
	setFirstResourceHeader(ResourceHeader(resourceId));
	setClientCommand(ResourceDown);
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

void KCloud::CommandPacket::setErrorStringList(const QStringList &err){

	m_stringList = err;
}

void KCloud::CommandPacket::setFirstResourceHeader(const KCloud::ResourceHeader &res){

	m_headersList[0] = res;
}

KCloud::User KCloud::CommandPacket::getUser() const{

	return m_currentUser;
}

QList<KCloud::ResourceHeader> KCloud::CommandPacket::getResourceTree() const{

    return m_headersList;
}

void KCloud::CommandPacket::answerToResourceDel(KCloud::CommandPacket::ServerAnswer answer, const QStringList &errorList){

    clear();
    setServerAnswer(answer);
    setErrorStringList(errorList);
}

KCloud::CommandPacket::ClientCommand KCloud::CommandPacket::getClientCommand() const{

	return m_clientCommand;
}

KCloud::CommandPacket::ServerAnswer KCloud::CommandPacket::getServerAnswer() const{

	return m_serverAnswer;
}

QStringList KCloud::CommandPacket::getLastError() const{

	return m_stringList;
}

KCloud::ResourceHeader KCloud::CommandPacket::getFirstResourceHeader() const{

	return m_headersList[0];
}

void KCloud::CommandPacket::answerToLogin(KCloud::CommandPacket::ServerAnswer answer, const KCloud::User &usr, const QStringList &errorStringList){

	clear();
	setUser(usr);
	setServerAnswer(answer);
	setErrorStringList(errorStringList);
}

void KCloud::CommandPacket::answerToLogout(KCloud::CommandPacket::ServerAnswer answer, const QStringList &errorStringList){

	clear();
	setServerAnswer(answer);
	setErrorStringList(errorStringList);
}

void KCloud::CommandPacket::answerToResourceTree(KCloud::CommandPacket::ServerAnswer answer, const QList<KCloud::ResourceHeader> res, const QStringList &errorList){

	clear();
	setServerAnswer(answer);
	setErrorStringList(errorList);
	m_headersList = res;
}

void KCloud::CommandPacket::answerToResourceUp(KCloud::CommandPacket::ServerAnswer answer, const QStringList &errorList){

	clear();
	setServerAnswer(answer);
	setErrorStringList(errorList);
}

void KCloud::CommandPacket::answerToResourceDown(KCloud::CommandPacket::ServerAnswer &answer, const QStringList &errorList){

	clear();
	setServerAnswer(answer);
	setErrorStringList(errorList);
}

void KCloud::CommandPacket::send(const qint64 block){

    if(block == 0){
        QDataStream stream(m_channel);
        stream << getNetworkSize();
        return;
    }

    m_bytesCounter = getNetworkSize();
    QDataStream stream(m_channel);
    stream << m_buffer;
}

void KCloud::CommandPacket::recv() throw(Exception){

    QDataStream stream(m_channel);

    switch (m_currentBlock){
        case 0:
            if(m_channel->bytesAvailable() < (qint64)sizeof(m_bytesCounter)){

                return;
            }
            stream >> m_bytesCounter;
            m_currentBlock++;
            break;
        case 1:
            if(m_channel->bytesAvailable() < m_bytesCounter){

                return;
            }
            stream >> *this;
            emit objectReceived();
            break;
        default:
            throw UnknownException();
            break;
    }
}

void KCloud::CommandPacket::behaviorOnSend(const qint64 dim) throw(Exception){

    m_bytesCounter -= dim;
    if(m_bytesCounter == 0){

        m_currentBlock ++;
        if(m_currentBlock == 2){

            emit objectSended();
        }else{

            emit changeBlock(m_currentBlock);
        }
    }else if(m_bytesCounter < 0){

        throw UntrustedBytesCounter();
    }
}

qint64 KCloud::CommandPacket::calculateNetworkSize() throw(Exception){

    return m_buffer.size();
}

QDataStream &KCloud::operator<<(QDataStream &out, const KCloud::CommandPacket &tmp){

	out << tmp.m_currentUser << tmp.m_stringList << tmp.m_serverAnswer << tmp.m_clientCommand << tmp.m_headersList;
	return out;
}

QDataStream &KCloud::operator>>(QDataStream &inp, KCloud::CommandPacket &tmp){

	inp >> tmp.m_currentUser >> tmp.m_stringList >> tmp.m_serverAnswer >> tmp.m_clientCommand >> tmp.m_headersList;
	return inp;
}

QDataStream &KCloud::operator<<(QDataStream &out, const KCloud::CommandPacket::ClientCommand &tmp){

	out << (qint32)tmp;
	return out;
}

QDataStream &KCloud::operator>>(QDataStream &inp, KCloud::CommandPacket::ClientCommand &tmp){

	inp >> (qint32 &)tmp;
	return inp;
}

QDataStream &KCloud::operator<<(QDataStream &out, const KCloud::CommandPacket::ServerAnswer &tmp){

	out << (qint32)tmp;
	return out;
}

QDataStream &KCloud::operator>>(QDataStream &inp, KCloud::CommandPacket::ServerAnswer &tmp){

	inp >> (qint32 &)tmp;
	return inp;
}
