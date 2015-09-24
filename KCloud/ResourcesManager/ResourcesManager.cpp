#include "ResourcesManager.h"

const QString KCloud::ResourcesManager::queryResources_1(" SELECT * FROM resources WHERE owner = :email AND id = :id ");
const QString KCloud::ResourcesManager::queryResources_2(" SELECT type FROM resources WHERE id = :id ");
const QString KCloud::ResourcesManager::queryResources_3(" SELECT permission FROM publicResources where resource = :id ");
const QString KCloud::ResourcesManager::queryResources_4(" SELECT permission FROM sharing where resource = :id AND  user = :email ");
const QString KCloud::ResourcesManager::queryResources_5(" SELECT * FROM resources where parent = :parent AND name = :name ");


KCloud::ResourcesManager::ResourcesManager(const QString &name, QObject *parent) : DatabaseManager(name, parent){

}

KCloud::ResourcesManager::~ResourcesManager()
{

}

bool KCloud::ResourcesManager::isOwner(const KCloud::User &usr, const quint64 &id) throw (Exception){

	if(open()){
		QSqlQuery query(m_db);
		query.prepare(queryResources_1);
		query.bindValue(placeHolder_mail, usr.getEmail());
		query.bindValue(placeHolder_id, id);
		tryExec(query);
		close();
		return (query.size() == 1 ? true : false);
	}else{
		throw OpenFailure();
	}
}

bool KCloud::ResourcesManager::exists(KCloud::ResourceHeader &header) throw (Exception){

	if(open()){
		QSqlQuery query(m_db);
		query.prepare(queryResources_5);
		query.bindValue(placeHolder_parent, header.getParentId());
		query.bindValue(placeHolder_name, header.getName());
		tryExec(query);
		close();
		if(query.size() == 0){
			return ResourceHeader::PermUndef;
		}
		query.seek(0);
		return (query.value(0) == sqlEnumRead ? ResourceHeader::Read : ResourceHeader::Write);
	}else{
		throw OpenFailure();
	}
}

KCloud::ResourceHeader::ResourceType KCloud::ResourcesManager::getType(const quint64 &id) throw (Exception){

	if(open()){
		QSqlQuery query(m_db);
		query.prepare(queryResources_2);
		query.bindValue(placeHolder_id, id);
		tryExec(query);
		close();
		if(query.size() == 0){
			return ResourceHeader::TypeUndef;
		}
		query.seek(0);
		return (query.value(0) == sqlEnumDir ? ResourceHeader::Dir : ResourceHeader::File);
	}else{
		throw OpenFailure();
	}
	return ResourceHeader::TypeUndef;
}

KCloud::ResourceHeader::ResourcePerm KCloud::ResourcesManager::publicPerm(const quint64 &id) throw (Exception){

	if(open()){
		QSqlQuery query(m_db);
		query.prepare(queryResources_3);
		query.bindValue(placeHolder_id, id);
		tryExec(query);
		close();
		if(query.size() == 0){
			return ResourceHeader::PermUndef;
		}
		query.seek(0);
		return (query.value(0) == sqlEnumRead ? ResourceHeader::Read : ResourceHeader::Write);
	}else{
		throw OpenFailure();
	}
	return ResourceHeader::PermUndef;
}

KCloud::ResourceHeader::ResourcePerm KCloud::ResourcesManager::sharedPerm(const KCloud::User &usr, const quint64 &id) throw (Exception){

	if(open()){
		QSqlQuery query(m_db);
		query.prepare(queryResources_4);
		query.bindValue(placeHolder_id, id);
		query.bindValue(placeHolder_mail, usr.getEmail());
		tryExec(query);
		close();
		if(query.size() == 0){
			return ResourceHeader::PermUndef;
		}
		query.seek(0);
		return (query.value(0) == sqlEnumRead ? ResourceHeader::Read : ResourceHeader::Write);
	}else{
		throw OpenFailure();
	}
	return ResourceHeader::PermUndef;
}



//controllo spazio utente
if(select * from users where email = usr.email + resourceHeader.size > dimensione da stabilire){
	non posso caricare perchè lo spazio non ci basta;
}
//controllo cartella
if(select * from resources where id = resourceHeader.parent == CARTELLA){
	//controllo che non esistano file con lo stesso nome di quello che voglio caricare con lo stesso padre:
	if(select * from resources where parent = resourceHeader.parent and name = resourceHeader.name == EMPTY SET){
		if(select * from resources where id = resourceHeader.parent and email = usr.email != EMPTY SET){
			//sono il proprietario posso caricare;
		}else if(select * from publicResources where id = resourceHeader.parent and permission = "WRITE" != EMPTY SET){
			//ho i permessi posso caricare;
		}else if(select * from sharing where user = usr.email and resource = resourceHeader.parent and permission = "WRITE" != EMPTY SET){
			//ho i permessi posso caricare;
		}else{
			//mi dispiace;
		}
	}else{
		mi salvo l'id della risorsa e faccio questa select

		if(select from sharing where user = usr.email and reurce)
	}
}else{
	Non  posso caricare perchè il padre è un file
}
