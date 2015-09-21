#include "ResourceHeader.h"

KCloud::ResourceHeader::ResourceHeader(QObject *parent) : QObject(parent){

}

void KCloud::ResourceHeader::clear(){

	m_size	= 0;
	m_id	= 0;
	m_owner = 0;
	m_parentId = 0;
	setPublicPermission();
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

bool KCloud::ResourceHeader::addPermission(const QString &mail, KCloud::ResourceHeader::ResourcePerm perm){

	if(User::checkMail(mail)){
		if(m_permissionTable.contains(mail)){

			return false;		//permesso già presente
		}else{
			m_permissionTable.insert(mail, perm);
			return true;
		}

	}else{
		//lanciare eccezione per email non valida
	}
}

bool KCloud::ResourceHeader::modPermission(const QString &mail, KCloud::ResourceHeader::ResourcePerm perm){

	if(User::checkMail(mail)){
		if(m_permissionTable.contains(mail)){

			m_permissionTable[mail] = perm;
			return true;		//permesso modificato
		}else{

			return false;		//utente non presente nella tabella dei permessi
		}

	}else{
		//lanciare eccezione per email non valida
	}
}

bool KCloud::ResourceHeader::delPermission(const QString &mail){

	if(User::checkMail(mail)){
		if(m_permissionTable.contains(mail)){

			m_permissionTable[mail] = perm;
			return true;		//permesso modificato
		}else{

			return false;		//utente non presente nella tabella dei permessi
		}

	}else{
		//lanciare eccezione per email non valida
	}
}
