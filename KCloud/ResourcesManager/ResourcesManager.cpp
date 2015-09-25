#include "ResourcesManager.h"
#include "../MainServer/defines.h"

const QString KCloud::ResourcesManager::queryResources_1(" SELECT * FROM resources WHERE owner = :email AND id = :id ");
const QString KCloud::ResourcesManager::queryResources_2(" SELECT type FROM resources WHERE id = :id ");
const QString KCloud::ResourcesManager::queryResources_3(" SELECT permission FROM publicResources where resource = :id ");
const QString KCloud::ResourcesManager::queryResources_4(" SELECT permission FROM sharing where resource = :id AND  user = :email ");
const QString KCloud::ResourcesManager::queryResources_5(" SELECT * FROM resources where parent = :parent AND name = :name ");
const QString KCloud::ResourcesManager::queryResources_6(" SELECT space FROM users where email = :email ");
const QString KCloud::ResourcesManager::queryResources_7(" INSERT INTO resources (parent, owner, name, type, size) VALUES ( :parent , :email, :name, :type, :size ) ");
const QString KCloud::ResourcesManager::queryResources_8(" INSERT INTO publicResources VALUES ( :id , :permission ) ");
const QString KCloud::ResourcesManager::queryResources_9(" INSERT INTO sharing VALUES ( :email , :id , :permission ) ");


KCloud::ResourcesManager::ResourcesManager(const QString &name, QObject *parent) : DatabaseManager(name, parent){

}

KCloud::ResourcesManager::~ResourcesManager()
{

}

KCloud::ResourcesManager::ResourcesManagerAnswer KCloud::ResourcesManager::checkForUpload(const User &usr, KCloud::ResourceHeader &head) throw (Exception){

	if(userSpace(usr) + head.getSize() > ___4GB___){
		return SpaceFull;
	}
	if(isOwner(usr, head.getParentId()) ||
			(sharedPerm(usr, head.getParentId()) == ResourceHeader::Write) ||
			(publicPerm(head.getParentId() == ResourceHeader::Write))){

		if(!exists(head)){
			return UploadOK;
		}else{
			return AlreadyExists;
		}
	}else{
		return PermError;
	}

}

KCloud::ResourcesManager::ResourcesManagerAnswer KCloud::ResourcesManager::aBadassFunction(const QString &path, const KCloud::ResourceHeader &head, const KCloud::User &usr, QStringList &errors) throw (Exception){

	if(open()){
		ResourceHeader h	= head;
		h.m_id				= h.m_parentId;
		h.m_parentId		= 0;
		recursiveAdd(path, h, usr, errors);
		close();
	}else{
		throw OpenFailure();
	}
	/**Ricordiamocelo**/
	return UploadOK;
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
			return false;
		}
		query.seek(0);
		header.m_id		= query.value(DatabaseManager::Id).toULongLong();
		header.m_owner	= query.value(DatabaseManager::Owner).toString();
		header.m_type	= (query.value(DatabaseManager::Type).toString() == sqlEnumDir ? ResourceHeader::Dir : ResourceHeader::File);
		header.m_size	= query.value(DatabaseManager::Size).toLongLong();
		return true;
	}else{
		throw OpenFailure();
	}
	return false;
}

qint64 KCloud::ResourcesManager::userSpace(const KCloud::User &usr) throw (Exception){

	if(open()){
		QSqlQuery query(m_db);
		query.prepare(queryResources_6);
		query.bindValue(placeHolder_mail, usr.getEmail());
		tryExec(query);
		close();
		if(query.size() == 0){
			return -1;
		}
		query.seek(0);
		return query.value(0).toLongLong();
	}else{
		throw OpenFailure();
	}
	return -1;
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

void KCloud::ResourcesManager::recursiveAdd(const QString &path, const ResourceHeader &head, const User &usr, QStringList &errors) throw (Exception){

	QDir dir(path);
	QFileInfoList list = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);

	foreach (QFileInfo f, list){
		QSqlQuery query(m_db);
		query.prepare(queryResources_7);
		query.bindValue(placeHolder_parent, head.getId());
		query.bindValue(placeHolder_mail, usr.getEmail());
		query.bindValue(placeHolder_name, f.fileName());

		if(f.isDir()){
			ResourceHeader n_head(f.absoluteFilePath(),
								  usr, head.getId(),
								  head.getPermissionTable(),
								  head.getPublicPermission());
			n_head.m_size = 0;
			query.bindValue(placeHolder_type, sqlEnumDir);
			query.bindValue(placeHolder_size, n_head.getSize());
			tryExec(query);
			n_head.m_id = query.lastInsertId().toULongLong();

			query.clear();
			if(head.getPublicPermission() != ResourceHeader::PermUndef){
				query.prepare(queryResources_8);
				query.bindValue(placeHolder_id, n_head.getId());
				query.bindValue(placeHolder_permission,
								head.getPublicPermission() == ResourceHeader::Read ? sqlEnumRead : sqlEnumWrite);
				tryExec(query);
				query.clear();
			}
			if(!head.getPermissionTable().empty()){
				foreach(QString usr, head.getPermissionTable().keys()){
					query.prepare(queryResources_9);
					query.bindValue(placeHolder_mail, usr);
					query.bindValue(placeHolder_id, n_head.getId());
					query.bindValue(placeHolder_permission,
									head.getPermission(usr) == ResourceHeader::Read ? sqlEnumRead : sqlEnumWrite);
					try{
						tryExec(query);
					}catch(Exception &e){
						errors << usr;
					}
				}
			}
			recursiveAdd(f.absoluteFilePath(), n_head, usr, errors);
		}else{

			query.bindValue(placeHolder_type, sqlEnumFile);
			query.bindValue(placeHolder_size, f.size());
			tryExec(query);
			QString lastIdStr = query.lastInsertId().toString();
			quint64 lastIdInt = query.lastInsertId().toULongLong();

			query.clear();
			if(head.getPublicPermission() != ResourceHeader::PermUndef){
				query.prepare(queryResources_8);
				query.bindValue(placeHolder_id, lastIdInt);
				query.bindValue(placeHolder_permission,
								head.getPublicPermission() == ResourceHeader::Read ? sqlEnumRead : sqlEnumWrite);
				tryExec(query);
				query.clear();
			}
			if(!head.getPermissionTable().empty()){
				foreach(QString user, head.getPermissionTable().keys()){
					query.prepare(queryResources_9);
					query.bindValue(placeHolder_mail, user);
					query.bindValue(placeHolder_id, lastIdInt);
					query.bindValue(placeHolder_permission,
									head.getPermission(user) == ResourceHeader::Read ? sqlEnumRead : sqlEnumWrite);
					try{
						tryExec(query);
					}catch(Exception &e){
						if(!errors.contains(user)){
							errors << user;
						}
					}
				}
			}
			QSettings appSettings;
			QFile::rename(f.absoluteFilePath(), appSettings.value(RESOURCES).toString() + QString("/") + lastIdStr);
		}
	}
}


