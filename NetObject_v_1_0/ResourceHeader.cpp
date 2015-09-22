#include "ResourceHeader.h"

KCloud::ResourceHeader::ResourceHeader(QObject *parent) : QObject(parent){

	ResourceHeader::clear();
}

KCloud::ResourceHeader::ResourceHeader(const QString &path,
									   const KCloud::User &sessionUser,
									   const quint64 &parentId,
									   const QMap<QString, KCloud::ResourceHeader::ResourcePerm> &permissionTable,
									   KCloud::ResourceHeader::ResourcePerm publicPerm,
									   QObject *parent) throw (Exception) : ResourceHeader(parent){

	if(path.isEmpty()){

		throw EmptyPathException();
	}
	if(!QFileInfo(path).exists()){

		throw BadPathException();
	}
	if(QFileInfo(path).isDir()){

		m_size = calculateDirSize(path);
		m_type = Dir;
	}else{

		m_size = QFileInfo(path).size();
		m_type = File;
	}
	m_parentId = parentId;
	setOwner(sessionUser);
	m_permissionTable = permissionTable;
	m_publicPerm = publicPerm;
}

void KCloud::ResourceHeader::clear(){

	m_size		= 0;
	m_id		= 0;
	m_parentId	= 0;
	setPublicPermission();
	m_owner.clear();
	m_permissionTable.clear();
}

void KCloud::ResourceHeader::setParentId(const quint64 &id){

	m_parentId = id;
}

void KCloud::ResourceHeader::setOwner(const User &sessionUser){

	m_owner = sessionUser.getEmail();
}

void KCloud::ResourceHeader::setPermissionTable(const QMap<QString, KCloud::ResourceHeader::ResourcePerm> &permissioTable){

	m_permissionTable = permissioTable;
}

bool KCloud::ResourceHeader::addPermission(const QString &mail, KCloud::ResourceHeader::ResourcePerm perm) throw(Exception){

	User::checkMail(mail);
	if(m_permissionTable.contains(mail)){

		return false;		//permesso già presente
	}else{

		m_permissionTable.insert(mail, perm);
		return true;
	}
}

bool KCloud::ResourceHeader::modPermission(const QString &mail, KCloud::ResourceHeader::ResourcePerm perm) throw(Exception){

	User::checkMail(mail);
	if(m_permissionTable.contains(mail)){

		m_permissionTable[mail] = perm;
		return true;		//permesso modificato
	}else{

		return false;		//utente non presente nella tabella dei permessi
	}
}

bool KCloud::ResourceHeader::delPermission(const QString &mail) throw(Exception){

	User::checkMail(mail);
	if(m_permissionTable.contains(mail)){

		m_permissionTable.remove(mail);
		return true;		//permesso modificato
	}else{

		return false;		//utente non presente nella tabella dei permessi
	}
}

void KCloud::ResourceHeader::setPublicPermission(KCloud::ResourceHeader::ResourcePerm perm){

	m_publicPerm = perm;
}

qint64 KCloud::ResourceHeader::getSize() const{

	return m_size;
}

quint64 KCloud::ResourceHeader::getId() const{

	return m_id;
}

quint64 KCloud::ResourceHeader::getParentId() const{

	return m_parentId;
}

QString KCloud::ResourceHeader::getOwner() const{

	return m_owner;
}

KCloud::ResourceHeader::ResourceType KCloud::ResourceHeader::getType() const{

	return m_type;
}

KCloud::ResourceHeader::ResourcePerm KCloud::ResourceHeader::getPublicPermission(){

	return m_publicPerm;
}

KCloud::ResourceHeader::ResourcePerm KCloud::ResourceHeader::getPermission(const QString &mail) throw(Exception){

	User::checkMail(mail);
	if(m_permissionTable.contains(mail)){

		return m_permissionTable[mail];
	}else{

		return PermUndef;
	}
}

QMap<QString, KCloud::ResourceHeader::ResourcePerm> KCloud::ResourceHeader::getPermissionTable() const{

	return m_permissionTable;
}

qint64 KCloud::ResourceHeader::calculateDirSize(const QString &path){

	QDir dir(path);
	qint64 size = 0;

	QFileInfoList list = dir.entryInfoList(QDir::Files | QDir::Dirs |  QDir::Hidden | QDir::NoSymLinks | QDir::NoDotAndDotDot);

	foreach (QFileInfo info, list) {

		if(info.isDir()){

			size += calculateDirSize(info.absoluteFilePath());
		}else{

			size += info.size();
		}
	}
	return size;
}

QDataStream &KCloud::operator<<(QDataStream &out, const KCloud::ResourceHeader &tmp){

	out << tmp.m_id << tmp.m_owner << tmp.m_parentId << tmp.m_permissionTable << tmp.m_publicPerm << tmp.m_size << tmp.m_type;
	return out;
}

QDataStream &KCloud::operator>>(QDataStream &inp, KCloud::ResourceHeader &tmp){

	inp >> tmp.m_id >> tmp.m_owner >> tmp.m_parentId >> tmp.m_permissionTable >> tmp.m_publicPerm >> tmp.m_size >> tmp.m_type;
	return inp;
}

QDataStream &KCloud::operator<<(QDataStream &out, const KCloud::ResourceHeader::ResourcePerm &tmp){

	out << (qint32)tmp;
	return out;
}

QDataStream &KCloud::operator>>(QDataStream &inp, KCloud::ResourceHeader::ResourcePerm &tmp){

	inp >> (qint32 &)tmp;
	return inp;
}

QDataStream &KCloud::operator<<(QDataStream &out, const KCloud::ResourceHeader::ResourceType &tmp){

	out << (qint32)tmp;
	return out;
}

QDataStream &KCloud::operator>>(QDataStream &inp, KCloud::ResourceHeader::ResourceType &tmp){

	inp >> (qint32 &)tmp;
	return inp;
}
