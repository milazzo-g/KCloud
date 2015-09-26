#include "ResourceHeader.h"

KCloud::ResourceHeader::ResourceHeader(QObject *parent) : QObject(parent){

	ResourceHeader::clear();
}

KCloud::ResourceHeader::ResourceHeader(const QString &path,
									   const User &sessionUser,
									   const quint64 &parentId,
									   const QMap<QString, KCloud::ResourceHeader::ResourcePerm> &permissionTable,
									   KCloud::ResourceHeader::ResourcePerm publicPerm,
									   QObject *parent) throw (Exception) :
	ResourceHeader(path, sessionUser.getEmail(), parentId, permissionTable, publicPerm, parent){


}

KCloud::ResourceHeader::ResourceHeader(const QString &path, const QString &sessionUser, const quint64 &parentId, const QMap<QString, KCloud::ResourceHeader::ResourcePerm> &permissionTable, KCloud::ResourceHeader::ResourcePerm publicPerm, QObject *parent) throw (Exception) : ResourceHeader(parent){

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
	setName(QFileInfo(path).fileName());
	m_permissionTable = permissionTable;
	m_publicPerm = publicPerm;
}

KCloud::ResourceHeader::ResourceHeader(const quint64 &id, QObject *parent) : KCloud::ResourceHeader(parent){

	setId(id);
}

KCloud::ResourceHeader::ResourceHeader(const KCloud::ResourceHeader &cpy) : QObject(cpy.parent()){

	*this = cpy;
}

void KCloud::ResourceHeader::clear(){

	m_size		= 0;
	m_id		= 0;
	m_parentId	= 0;
	setPublicPermission();
	m_name.clear();
	m_owner.clear();
	m_permissionTable.clear();
}

void KCloud::ResourceHeader::setName(const QString &name){

	m_name = name;
}

void KCloud::ResourceHeader::setParentId(const quint64 &id){

	m_parentId = id;
}

void KCloud::ResourceHeader::setOwner(const User &sessionUser){

	m_owner = sessionUser.getEmail();
}

void KCloud::ResourceHeader::setOwner(const QString &sessionUser){

	User::checkMail(sessionUser);
	m_owner = sessionUser;
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

QString KCloud::ResourceHeader::getName() const{

	return m_name;
}

QString KCloud::ResourceHeader::toString() const{

	QString out;
	out +=	"\nSize \t=\t";
	out	+=	QString::number(m_size);
	out	+=	"\t\n";
	out +=	"Id	\t=\t";
	out +=	QString::number(m_id);
	out +=	"\t\n";
	out +=	"ParentId \t=\t ";
	out +=	QString::number(m_parentId);
	out +=	"\t\n";
	out +=	"Owner \t=\t ";
	out +=	m_owner;
	out +=	"\t\n";
	out +=	"Name \t=\t";
	out +=	m_name;
	out +=	"\t\n";
	out +=	"Type \t=\t ";
	out +=	(int)m_type;
	return out;
}

KCloud::ResourceHeader::ResourceType KCloud::ResourceHeader::getType() const{

	return m_type;
}

KCloud::ResourceHeader::ResourcePerm KCloud::ResourceHeader::getPublicPermission() const{

	return m_publicPerm;
}

KCloud::ResourceHeader::ResourcePerm KCloud::ResourceHeader::getPermission(const QString &mail) const throw(Exception){

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

KCloud::ResourceHeader &KCloud::ResourceHeader::operator=(const KCloud::ResourceHeader &cpy){

	m_size				= cpy.m_size;
	m_name				= cpy.m_name;
	m_id				= cpy.m_id;
	m_parentId			= cpy.m_parentId;
	m_owner				= cpy.m_owner;
	m_publicPerm		= cpy.m_publicPerm;
	m_type				= cpy.m_type;
	m_permissionTable	= cpy.m_permissionTable;

	return *this;
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

void KCloud::ResourceHeader::setId(const quint64 &id){

	m_id = id;
}

QDataStream &KCloud::operator<<(QDataStream &out, const KCloud::ResourceHeader &tmp){

	out << tmp.m_id << tmp.m_owner << tmp.m_parentId << tmp.m_permissionTable << tmp.m_publicPerm << tmp.m_size << tmp.m_type << tmp.m_name;
	return out;
}

QDataStream &KCloud::operator>>(QDataStream &inp, KCloud::ResourceHeader &tmp){

	inp >> tmp.m_id >> tmp.m_owner >> tmp.m_parentId >> tmp.m_permissionTable >> tmp.m_publicPerm >> tmp.m_size >> tmp.m_type >> tmp.m_name;
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
