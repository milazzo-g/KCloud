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
