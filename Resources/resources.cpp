#include "resources.h"

KCloud::ResourceHeader::ResourceHeader(const KCloud::ResourceHeader &cpy){

	this->id = cpy.id;
	this->name = cpy.name;
	this->owner = cpy.owner;
	this->parent = cpy.parent;
	this->basicType = cpy.basicType;
	this->naturalSize = cpy.naturalSize;
	this->compressedSize = cpy.compressedSize;
	this->permissionTable = cpy.permissionTable;
}

qint64 KCloud::ResourceHeader::getCompressedSize() const{

	return compressedSize;
}

qint64 KCloud::ResourceHeader::getNetworkSize() const{

	QByteArray	buff;
	QDataStream test(&buff, QIODevice::ReadWrite);
	test << *this;
	return buff.size();
}

qint64 KCloud::ResourceHeader::getNaturalSize() const{

	return naturalSize;
}

quint64 KCloud::ResourceHeader::getParent() const{

	return parent;
}

quint64 KCloud::ResourceHeader::getId() const{

	return id;
}

QString KCloud::ResourceHeader::getName() const{

	return name;
}

QString KCloud::ResourceHeader::getOwner() const{

	return owner;
}

QString KCloud::ResourceHeader::getSuffix() const{

	return "Non implementata";
}

KCloud::ResourceHeader::ResourceType KCloud::ResourceHeader::getType() const{

	return basicType;
}

KCloud::ResourceHeader::ResourcePerm KCloud::ResourceHeader::getPermission(QString email) const{

	return permissionTable[email];
}

QMap<QString, KCloud::ResourceHeader::ResourcePerm> KCloud::ResourceHeader::getPermissionTable() const{

	return permissionTable;
}

void KCloud::ResourceHeader::setOwner(const QString owner){

	this->owner = owner;
}

bool KCloud::ResourceHeader::addPermission(const QString user, const KCloud::ResourceHeader::ResourcePerm perm){

	permissionTable.insert(user, perm);
	return true;
}

bool KCloud::ResourceHeader::delPermission(const QString user){

	if(permissionTable.contains(user)){
		permissionTable.remove(user);
		return true;
	}
	return false;
}

bool KCloud::ResourceHeader::modPermission(const QString user, const KCloud::ResourceHeader::ResourcePerm perm){

	if(permissionTable.contains(user)){
		permissionTable[user] = perm;
		return true;
	}
	return false;
}

QDataStream &KCloud::operator <<(QDataStream &out, const KCloud::ResourceHeader &res){

	out << res.naturalSize << res.compressedSize << res.id << res.parent << res.name << res.owner << res.basicType << res.permissionTable;
	return out;
}

QDataStream &KCloud::operator >>(QDataStream &inp, KCloud::ResourceHeader &res){

	inp >> res.naturalSize >> res.compressedSize >> res.id >> res.parent >> res.name >> res.owner >> res.basicType >> res.permissionTable;
	return inp;
}

QDataStream &KCloud::operator <<(QDataStream &out, KCloud::ResourceHeader::ResourceType &res){

	out << (qint32)res;
	return out;
}

QDataStream &KCloud::operator >>(QDataStream &inp, KCloud::ResourceHeader::ResourceType &res){

	inp >> (qint32 &)res;
	return inp;
}

QDataStream &KCloud::operator <<(QDataStream &out, KCloud::ResourceHeader::ResourcePerm &res){

	out << (qint32)res;
	return out;
}

QDataStream &KCloud::operator >>(QDataStream &inp, KCloud::ResourceHeader::ResourcePerm &res){

	inp >> (qint32 &)res;
	return inp;
}

KCloud::Resource::Resource(const QString path, const QString owner){

}

bool KCloud::Resource::compress(){

}

bool KCloud::Resource::unCompress(){

}

bool KCloud::Resource::deleteTempFile(){

}

QFile *KCloud::Resource::getFile(){

}

