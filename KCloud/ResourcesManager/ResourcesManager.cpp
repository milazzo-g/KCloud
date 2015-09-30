#include "ResourcesManager.h"
#include <QThread>
#include "../MainServer/defines.h"

const QString KCloud::ResourcesManager::queryResources_1(" SELECT owner FROM resources WHERE id = :id ");
const QString KCloud::ResourcesManager::queryResources_2(" SELECT type FROM resources WHERE id = :id ");
const QString KCloud::ResourcesManager::queryResources_3(" SELECT permission FROM publicResources where resource = :id ");
const QString KCloud::ResourcesManager::queryResources_4(" SELECT permission FROM sharing where resource = :id AND  user = :email ");
const QString KCloud::ResourcesManager::queryResources_5(" SELECT * FROM resources where parent = :parent AND name = :name ");
const QString KCloud::ResourcesManager::queryResources_6(" SELECT space FROM users where email = :email ");
const QString KCloud::ResourcesManager::queryResources_7(" INSERT INTO resources (parent, owner, name, type, size) VALUES ( :parent , :owner , :name, :type, :size ) ");
const QString KCloud::ResourcesManager::queryResources_8(" INSERT INTO publicResources VALUES ( :id , :permission ) ");
const QString KCloud::ResourcesManager::queryResources_9(" INSERT INTO sharing VALUES ( :email , :id , :permission ) ");
const QString KCloud::ResourcesManager::queryResources_10(" UPDATE users SET space = :space WHERE email = :email ");
const QString KCloud::ResourcesManager::queryResources_11(" SELECT size FROM resources WHERE id = :id ");
const QString KCloud::ResourcesManager::queryResources_12(" SELECT * FROM resources WHERE id = :id ");
const QString KCloud::ResourcesManager::queryResources_13(" SELECT * FROM users WHERE email = :email ");
const QString KCloud::ResourcesManager::queryResources_14(" SELECT * FROM sharing WHERE resource = :id ");
const QString KCloud::ResourcesManager::queryResources_15(" SELECT permission FROM publicResources where resource = :id ");
const QString KCloud::ResourcesManager::queryResources_16(" SELECT * FROM resources where parent = :id and type = :type ");
const QString KCloud::ResourcesManager::queryResources_17(" DELETE FROM resources WHERE parent = :id AND type = :type ");
const QString KCloud::ResourcesManager::queryResources_18(" DELETE FROM resources WHERE id = :id ");
const QString KCloud::ResourcesManager::queryResources_19(" SELECT * FROM resources WHERE owner = :email ");
const QString KCloud::ResourcesManager::queryResources_20(" SELECT id, parent, owner, name, type, size FROM sharing JOIN resources ON sharing.resource = resources.id  WHERE sharing.user = :email ");
const QString KCloud::ResourcesManager::queryResources_21(" SELECT id, parent, owner, name, type, size FROM publicResources JOIN resources ON publicResources.resource = resources.id ");
const QString KCloud::ResourcesManager::queryResources_22(" UPDATE resources SET name = :name WHERE id = :id ");
const QString KCloud::ResourcesManager::queryResources_23(" UPDATE publicResources SET permission = :permission WHERE resource = :id ");
const QString KCloud::ResourcesManager::queryResources_24(" DELETE FROM sharing WHERE resource = :id AND user = :email");
const QString KCloud::ResourcesManager::queryResources_25(" UPDATE sharing SET permission = :permission WHERE resource = :id AND user = :email ");
const QString KCloud::ResourcesManager::queryResources_26(" DELETE FROM publicResources WHERE id = :id ");
const QString KCloud::ResourcesManager::queryResources_27(" DELETE FROM sharing WHERE id = :id ");

KCloud::ResourcesManager::ResourcesManager(const QString &name, QObject *parent) : DatabaseManager(name, parent){

}

KCloud::ResourcesManager::~ResourcesManager()
{

}

KCloud::ResourceHeader KCloud::ResourcesManager::headerForId(const quint64 &id) throw (Exception){

	if(open()){
		if(!resourceExists(id)){
			throw ResourceNotExists();
		}
		QSqlQuery query(m_db);
		query.prepare(queryResources_12);
		query.bindValue(placeHolder_id, id);
		tryExec(query);
		if(!query.size()){
			throw ResourceNotExists();
		}else if(query.size() == 1){
			query.seek(0);
			ResourceHeader result;
			result.setId(query.value(res_Id).toULongLong());
			result.setParentId(query.value(res_Parent).toULongLong());
			result.setOwner(query.value(res_Owner).toString());
			result.setName(query.value(res_Name).toString());
			result.m_type = (query.value(res_Type).toString() == sqlEnumDir ? ResourceHeader::Dir : ResourceHeader::File);
			result.m_size = query.value(res_Size).toLongLong();
			result.setPublicPermission(getPublicPermission(result.getId()));
			result.setPermissionTable(getSharedPermission(result.getId()));
			return result;
		}else{
			throw MultipleRowsForPrimaryKey();
		}
	}else{
		throw OpenFailure();
	}
}
/*
 * OK!!!!!!! -> Definitiva
 */
KCloud::ResourcesManager::ResourcesManagerAnswer KCloud::ResourcesManager::checkForUpload(const User &usr, KCloud::ResourceHeader &head) throw (Exception){

	if(open()){
		try{
			if(userSpace(usr.getEmail()) + head.getSize() > ___4GB___){
				return SpaceFull;
			}
			bool res1 = isOwner(usr, head.getParentId())	||
						sharedPerm(usr, head.getParentId()) == ResourceHeader::Write	||
						publicPerm(head.getParentId())		== ResourceHeader::Write;

			bool res2 = !resourceExists(head);
			bool res3 = resourceType(head.getParentId()) == ResourceHeader::Dir;
			close();
			if(res1){
				if(res2){
					if(res3){

						return UploadOK;
					}else{

						return NotADir;
					}
				}else{
					return AlreadyExists;
				}
			}else{
				return PermError;
			}
		}catch(Exception &e){
			close();
			switch (e.type()) {
				case Exception::DatabaseResourceNotFound:
					return ParentNotFound;
				default:
					throw e;
			}
		}
	}else{
		throw OpenFailure();
	}
}

KCloud::ResourcesManager::ResourcesManagerAnswer KCloud::ResourcesManager::addResources(const QString &path, const KCloud::ResourceHeader &incomplete, QStringList &errors) throw (Exception){

	if(open()){
		errors = recursiveAdd(path, incomplete);
		close();
	}else{
		throw OpenFailure();
	}
	if(errors.isEmpty()){
		return FinalizeOK;
	}else{
		return FinalizeIssues;
	}
}

KCloud::ResourcesManager::ResourcesManagerAnswer KCloud::ResourcesManager::getResources(const User &usr, const QString &path, const KCloud::ResourceHeader &resource, QStringList &filesMoved) throw (Exception){

	if(open()){
		if(!resourceExists(resource.getId())){
			close();
			return RecursiveGetFail;
		}
		ResourceHeader::ResourcePerm sharedPermission = sharedPerm(usr, resource.getId());
		ResourceHeader::ResourcePerm publicPermission = publicPerm(resource.getId());
		if(isOwner(usr.getEmail(), resource.getId()) || (sharedPermission != ResourceHeader::PermUndef || publicPermission != ResourceHeader::PermUndef)){
			ResourceHeader header = getHeader(resource.getId());
			filesMoved = recursiveGet(path, header);
			//close(); -------------------------------------------------
			return RecursiveGetOK;
		}else{
			close();
			return PermError;
		}
	}else{
		throw OpenFailure();
	}
}

KCloud::ResourcesManager::ResourcesManagerAnswer KCloud::ResourcesManager::delResources(const KCloud::User &usr, const KCloud::ResourceHeader &head) throw (Exception){

	if(open()){
		ResourceHeader toDel = getHeader(head.getId());
		if(!isOwner(usr, toDel.getId()) || toDel.getName() == usr.getEmail()){
			close();
			return PermError;
		}else{
			recursiveDel(toDel);
			close();
			return DeleteOK;
		}
	}else{
		throw OpenFailure();
	}
}

KCloud::ResourcesManager::ResourcesManagerAnswer KCloud::ResourcesManager::modResource(const KCloud::User &usr, const KCloud::ResourceHeader &head) throw (Exception){

	if(open()){
		ResourceHeader toMod = getHeader(head.getId());
		if(!resourceExists(head.getId())){
			close();
			return ResourceNotExist;
		}
		if(!isOwner(usr, toMod.getId()) || toMod.getName() == usr.getEmail()){
			close();
			return PermError;
		}else{
			updateResourceName(head.getId(), head.getName());
			close();
			return ResourceModOk;
		}
	}else{
		throw OpenFailure();
	}
}

KCloud::ResourcesManager::ResourcesManagerAnswer KCloud::ResourcesManager::shareResource(const KCloud::User &usr, const KCloud::ResourceHeader &head, QStringList &errorUsersShare, int i) throw (Exception){

	if(open()){
		if(!userExists(usr)){
			close();
			return UserNotExists;
		}
		ResourceHeader header(head);
		foreach (QString user, header.getPermissionTable().keys()) {
			if(!userExists(user)){
				errorUsersShare << user;
				header.delPermission(user);
			}
		}
		recursiveShare(header);
		return SharingOk;
	}else{
		throw OpenFailure();
	}

}

QList<KCloud::ResourceHeader> KCloud::ResourcesManager::resourceTree(const KCloud::User &usr){

	if(open()){
		QList<ResourceHeader> result = getOwned(usr) + getShared(usr) + getPublic();
		close();
		return result;
	}else{
		throw OpenFailure();
	}
}

/*
 * OK!!!!!!! -> Definitiva
 */
bool KCloud::ResourcesManager::isOwner(const KCloud::User &usr, const quint64 &id) throw (Exception){

	return isOwner(usr.getEmail(), id);
}
/*
 * OK!!!!!!! -> Definitiva
 */
bool KCloud::ResourcesManager::isOwner(const QString &usr, const quint64 &id) throw (Exception){

	if(isOpen()){
		QSqlQuery query(m_db);
		query.prepare(queryResources_1);
		query.bindValue(placeHolder_id, id);
		tryExec(query);
		if(!query.size()){
			throw ResourceNotExists();
		}else if(query.size() == 1){
			query.seek(0);
			return (query.value(0).toString() == usr ?  true : false);
		}else{
			throw MultipleRowsForPrimaryKey();
		}
	}else{
		throw OpenFailure();
	}
}
/*
 * OK!!!!!!! -> Definitiva
 */
bool KCloud::ResourcesManager::resourceExists(KCloud::ResourceHeader &header) throw (Exception){

	if(isOpen()){
		QSqlQuery query(m_db);
		query.prepare(queryResources_5);
		query.bindValue(placeHolder_parent, header.getParentId());
		query.bindValue(placeHolder_name, header.getName());
		tryExec(query);
		if(!query.size()){
			return false;
		}else if(query.size() == 1){
			query.seek(0);
			header.m_id		= query.value(res_Id).toULongLong();
			header.m_owner	= query.value(res_Owner).toString();
			header.m_type	= (query.value(res_Type).toString() == sqlEnumDir ? ResourceHeader::Dir : ResourceHeader::File);
			header.m_size	= query.value(res_Size).toLongLong();
			return true;
		}else{
			throw MultipleRowsForPrimaryKey();
		}
	}else{
		throw OpenFailure();
	}
}
/*
 * OK!!!!!!! -> Definitiva
 */
bool KCloud::ResourcesManager::resourceExists(const quint64 &id) throw (Exception){

	if(isOpen()){
		QSqlQuery query(m_db);
		query.prepare(queryResources_12);
		query.bindValue(placeHolder_id, id);
		tryExec(query);
		if(!query.size()){
			return false;
		}else if(query.size() == 1){
			return true;
		}else{
			throw MultipleRowsForPrimaryKey();
		}
	}else{
		throw OpenFailure();
	}
}
/*
 * OK!!!!!!! -> Definitiva
 */
bool KCloud::ResourcesManager::userExists(const KCloud::User &usr) throw (Exception){
	return userExists(usr.getEmail());
}
/*
 * OK!!!!!!! -> Definitiva
 */
bool KCloud::ResourcesManager::userExists(const QString &usr) throw (Exception){

	if(isOpen()){
		QSqlQuery query(m_db);
		query.prepare(queryResources_13);
		query.bindValue(placeHolder_mail, usr);
		tryExec(query);
		if(!query.size()){
			return false;
		}else if(query.size() == 1){
			return true;
		}else{
			throw MultipleRowsForPrimaryKey();
		}
	}else{
		throw OpenFailure();
	}
}

void KCloud::ResourcesManager::updateResourceName(const quint64 &id, const QString &name) throw (Exception){

	if(isOpen()){
		QSqlQuery query(m_db);
		query.prepare(queryResources_22);
		query.bindValue(placeHolder_id, id);
		query.bindValue(placeHolder_name, name);
		tryExec(query);
	}else{
		throw OpenFailure();
	}
}

void KCloud::ResourcesManager::updatePublicPermission(const KCloud::ResourceHeader &head) throw (Exception){

	if(isOpen()){
		QSqlQuery query(m_db);
		query.prepare(queryResources_23);
		query.bindValue(placeHolder_id, head.getId());
		query.bindValue(placeHolder_permission,	head.getPublicPermission() == ResourceHeader::Read ? sqlEnumRead : sqlEnumWrite);
		tryExec(query);
	}else{
		throw OpenFailure();
	}
}

void KCloud::ResourcesManager::deleteSharing(const quint64 &id, const QString &user) throw (Exception){

	if(isOpen()){
		QSqlQuery query(m_db);
		query.prepare(queryResources_24);
		query.bindValue(placeHolder_id, id);
		query.bindValue(placeHolder_mail, user);
		tryExec(query);
	}else{
		throw OpenFailure();
	}
}

void KCloud::ResourcesManager::updareSharing(const quint64 &id, const QString &user, KCloud::ResourceHeader::ResourcePerm perm) throw (Exception){

	if(isOpen()){
		QSqlQuery query(m_db);
		query.prepare(queryResources_25);
		query.bindValue(placeHolder_id, id);
		query.bindValue(placeHolder_mail, user);
		query.bindValue(placeHolder_permission, perm == ResourceHeader::Read ? sqlEnumRead : sqlEnumWrite);
		tryExec(query);
	}else{
		throw OpenFailure();
	}
}

void KCloud::ResourcesManager::addSharing(const quint64 &id, const QString &user, KCloud::ResourceHeader::ResourcePerm perm) throw (Exception){

	if(isOpen()){
		QSqlQuery query(m_db);
		query.prepare(queryResources_9);
		query.bindValue(placeHolder_id, id);
		query.bindValue(placeHolder_mail, user);
		query.bindValue(placeHolder_permission, perm == ResourceHeader::Read ? sqlEnumRead : sqlEnumWrite);
		tryExec(query);
	}else{
		throw OpenFailure();
	}

}

void KCloud::ResourcesManager::deletePublic(const quint64 &id) throw (Exception){

	if(isOpen()){
		QSqlQuery query(m_db);
		query.prepare(queryResources_26);
		query.bindValue(placeHolder_id, id);
		tryExec(query);
	}else{
		throw OpenFailure();
	}
}

void KCloud::ResourcesManager::deletePublic(const KCloud::ResourceHeader &head) throw (Exception){

	return deletePublic(head.getId());
}

void KCloud::ResourcesManager::deleteAllSharing(const KCloud::ResourceHeader &head){

	if(isOpen()){
		QSqlQuery query(m_db);
		query.prepare(queryResources_27);
		query.bindValue(placeHolder_id, head.getId());
		tryExec(query);
	}else{
		throw OpenFailure();
	}
}

/*
 * OK!!!!!!! -> Definitiva
 */
qint64 KCloud::ResourcesManager::userSpace(const QString &usr) throw (Exception){

	if(isOpen()){
		QSqlQuery query(m_db);
		query.prepare(queryResources_6);
		query.bindValue(placeHolder_mail, usr);
		tryExec(query);
		if(query.size() == 0){
			throw UserNotExists();
		}else if(query.size() == 1){
			query.seek(0);
			return query.value(0).toLongLong();
		}else{
			throw MultipleRowsForPrimaryKey();
		}
	}else{
		throw OpenFailure();
	}
}

/*
 * OK!!!!!!! -> Definitiva
 */
qint64 KCloud::ResourcesManager::resourceSize(const quint64 &id) throw (Exception){

	if(isOpen()){
		QSqlQuery query(m_db);
		query.prepare(queryResources_11);
		query.bindValue(placeHolder_id, id);
		tryExec(query);
		if(query.size() == 0){
			throw ResourceNotExists();
		}else if(query.size() == 1){
			query.seek(0);
			return query.value(0).toLongLong();
		}else{
			throw MultipleRowsForPrimaryKey();
		}
	}else{
		throw OpenFailure();
	}
}
/*
 * OK!!!!!!! -> Definitiva
 */

KCloud::ResourceHeader::ResourceType KCloud::ResourcesManager::resourceType(const quint64 &id) throw (Exception){

	if(isOpen()){
		QSqlQuery query(m_db);
		query.prepare(queryResources_2);
		query.bindValue(placeHolder_id, id);
		tryExec(query);
		if(!query.size()){
			throw ResourceNotExists();
		}else if(query.size() == 1){
			query.seek(0);
			return (query.value(0).toString() == sqlEnumDir ? ResourceHeader::Dir : ResourceHeader::File);
		}else{
			throw MultipleRowsForPrimaryKey();
		}
	}else{
		throw OpenFailure();
	}
}

/*
 * OK!!!!!!! -> Definitiva
 */

KCloud::ResourceHeader::ResourcePerm KCloud::ResourcesManager::publicPerm(const quint64 &id) throw (Exception){

	if(isOpen()){
		QSqlQuery query(m_db);
		query.prepare(queryResources_3);
		query.bindValue(placeHolder_id, id);
		tryExec(query);
		if(!query.size()){
			return ResourceHeader::PermUndef;
		}else if(query.size() == 1){
			query.seek(0);
			return (query.value(0).toString() == sqlEnumRead ? ResourceHeader::Read : ResourceHeader::Write);
		}else{
			throw MultipleRowsForPrimaryKey();
		}
	}else{
		throw OpenFailure();
	}
}

/*
 * OK!!!!!!! -> Definitiva
 */

KCloud::ResourceHeader::ResourcePerm KCloud::ResourcesManager::sharedPerm(const KCloud::User &usr, const quint64 &id) throw (Exception){

	if(isOpen()){
		QSqlQuery query(m_db);
		query.prepare(queryResources_4);
		query.bindValue(placeHolder_id, id);
		query.bindValue(placeHolder_mail, usr.getEmail());
		tryExec(query);
		if(!query.size()){
			return ResourceHeader::PermUndef;
		}else if(query.size() == 1){
			query.seek(0);
			return (query.value(0).toString() == sqlEnumRead ? ResourceHeader::Read : ResourceHeader::Write);
		}else{
			throw MultipleRowsForPrimaryKey();
		}
	}else{
		throw OpenFailure();
	}
}

QStringList KCloud::ResourcesManager::recursiveAdd(const QString &path, const KCloud::ResourceHeader &incomplete) throw (Exception){

	QStringList errors;
	QDir dir(path);
	QFileInfoList list = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);

	foreach (QFileInfo f, list){
		ResourceHeader newResource(f.absoluteFilePath(),
								   incomplete.getOwner(),
								   incomplete.getParentId(),
								   incomplete.getPermissionTable(),
								   incomplete.getPublicPermission());

		newResource = addResource(newResource);
		updateSpace(newResource.getOwner(), newResource.getId(), Increment);
		setPublicPermission(newResource);
		errors += setSharedPermission(newResource);
		newResource.setParentId(newResource.getId());
		if(f.isDir()){
			QDir old;
			recursiveAdd(f.absoluteFilePath(), newResource);
			old.rmdir(f.absoluteFilePath());
		}else{
			QSettings appSettings;
			QFile::rename(f.absoluteFilePath(),
						  appSettings.value(RESOURCES).toString() +
						  QString("/") +
						  QString::number(newResource.getId()));
		}
	}
	return errors;
}

QStringList KCloud::ResourcesManager::recursiveGet(const QString &path, const KCloud::ResourceHeader &item) throw (Exception){

	QStringList list;
	QSettings appSettings;
	if(item.getType() == ResourceHeader::File){
		list << (path + QString("/") + item.getName());
		QFile::copy(appSettings.value(RESOURCES).toString() +
					QString("/") + QString::number(item.getId()),
					list.last());
	}else{
		QDir current(path);
		QList<ResourceHeader> childFiles	= getChilds(item.getId(), OnlyFiles);
		QList<ResourceHeader> childDirs		= getChilds(item.getId(), OnlyDirs);

		current.mkdir(item.getName());
		current.cd(item.getName());

		list << current.path();

		foreach (ResourceHeader file, childFiles){
			list << (current.path() + QString("/") + file.getName());
			QFile::copy(appSettings.value(RESOURCES).toString() +
						QString("/") + QString::number(file.getId()),
						list.last());
		}

		foreach (ResourceHeader dir, childDirs) {
			list += recursiveGet(current.path(), dir);
		}
	}
	return list;
}

void KCloud::ResourcesManager::recursiveDel(const KCloud::ResourceHeader &head) throw (Exception){

	if(isOpen()){
		QSettings appSettings;
		if(head.getType() == ResourceHeader::Dir){

			QList<ResourceHeader> childFiles	= getChilds(head.getId(), OnlyFiles);
			QList<ResourceHeader> childDirs		= getChilds(head.getId(), OnlyDirs);

			foreach (ResourceHeader file, childFiles) {
				updateSpace(head.getOwner(), file.getId(), Decrement);
				QFile::remove(appSettings.value(RESOURCES).toString() + QString("/") + QString::number(file.getId()));
			}

			QSqlQuery query(m_db);
			query.prepare(queryResources_17);
			query.bindValue(placeHolder_id, head.getId());
			query.bindValue(placeHolder_type, sqlEnumFile);
			tryExec(query);

			foreach (ResourceHeader dir, childDirs) {
				recursiveDel(dir);
			}
			delResource(head);
		}else{
			QFile::remove(appSettings.value(RESOURCES).toString() + QString("/") + QString::number(head.getId()));
			updateSpace(head.getOwner(), head.getId(), Decrement);
			delResource(head);
		}
	}else{
		throw OpenFailure();
	}

}

/*
 * OK!!!!!!! -> Definitiva
 */

void KCloud::ResourcesManager::updateSpace(const QString &usr, const quint64 &id, const KCloud::ResourcesManager::SpaceUpdateMode mode) throw (Exception){

	if(isOpen()){
		QSqlQuery query(m_db);
		query.prepare(queryResources_10);
		query.bindValue(placeHolder_space, (mode == Increment ? userSpace(usr) + resourceSize(id) :
																userSpace(usr) - resourceSize(id)));
		query.bindValue(placeHolder_mail, usr);
		tryExec(query);
	}else{
		throw OpenFailure();
	}
}
/*
 * OK!!!!!!! -> Definitiva
 */

void KCloud::ResourcesManager::setPublicPermission(const KCloud::ResourceHeader &header) throw (Exception){

	if(isOpen()){
		if(header.getPublicPermission() != ResourceHeader::PermUndef && resourceExists(header.getId())){
			QSqlQuery query(m_db);
			query.prepare(queryResources_8);
			query.bindValue(placeHolder_id, header.getId());
			query.bindValue(placeHolder_permission,	header.getPublicPermission() == ResourceHeader::Read ? sqlEnumRead : sqlEnumWrite);
			tryExec(query);
		}
	}else{
		throw OpenFailure();
	}
}

KCloud::ResourceHeader::ResourcePerm KCloud::ResourcesManager::getPublicPermission(const quint64 &id) throw (Exception){

	if(isOpen()){
		if(!resourceExists(id)){
			throw ResourceNotExists();
		}
		QSqlQuery query(m_db);
		query.prepare(queryResources_15);
		query.bindValue(placeHolder_id, id);
		tryExec(query);

		if(!query.size()){
			return ResourceHeader::PermUndef;
		}else if(query.size() == 1){
			query.seek(0);
			return (query.value(0).toString() == sqlEnumRead ? ResourceHeader::Read : ResourceHeader::Write);
		}else{
			throw MultipleRowsForPrimaryKey();
		}
	}else{
		throw OpenFailure();
	}

}
/*
 * OK!!!!!!! -> Definitiva
 */

QStringList KCloud::ResourcesManager::setSharedPermission(const KCloud::ResourceHeader &header) throw (Exception){

	if(isOpen()){
		QStringList list;
		if(!header.getPermissionTable().isEmpty() && resourceExists(header.getId())){
			foreach (QString usr, header.getPermissionTable().keys()){
				if(!userExists(usr)){
					list << usr;
				}else{
					QSqlQuery query(m_db);
					query.prepare(queryResources_9);
					query.bindValue(placeHolder_mail, usr);
					query.bindValue(placeHolder_id, header.getId());
					query.bindValue(placeHolder_permission,
									header.getPermission(usr) == ResourceHeader::Read ?
									sqlEnumRead : sqlEnumWrite);
					tryExec(query);
				}
			}
		}
		return list;
	}else{
		throw OpenFailure();
	}
}

QMap<QString, KCloud::ResourceHeader::ResourcePerm> KCloud::ResourcesManager::getSharedPermission(const quint64 &id) throw (Exception){

	if(isOpen()){
		if(!resourceExists(id)){
			throw ResourceNotExists();
		}
		QMap<QString, ResourceHeader::ResourcePerm> map;
		QSqlQuery query(m_db);
		query.prepare(queryResources_14);
		query.bindValue(placeHolder_id, id);
		tryExec(query);
		while(query.next()){
			map.insert(query.value(sha_User).toString(),
					   query.value(sha_Permission).toString() == sqlEnumRead ?
						   ResourceHeader::Read : ResourceHeader::Write);
		}
		return map;
	}else{
		throw OpenFailure();
	}
}

KCloud::ResourceHeader KCloud::ResourcesManager::addResource(const KCloud::ResourceHeader &header) throw (Exception){

	if(isOpen()){
		if(!resourceExists(header.getParentId())){
			throw ResourceNotExists();
		}else if(!userExists(header.getOwner())){
			throw UserNotExists();
		}else{
			QSqlQuery query(m_db);
			query.prepare(queryResources_7);
			query.bindValue(placeHolder_parent, header.getParentId());
			query.bindValue(placeHolder_owner, header.getOwner());
			query.bindValue(placeHolder_name, header.getName());
			query.bindValue(placeHolder_type,
							header.getType() == ResourceHeader::Dir ?
							sqlEnumDir : sqlEnumFile);
			query.bindValue(placeHolder_size,
							header.getType() == ResourceHeader::Dir ?
							0 : header.getSize());
			tryExec(query);

			ResourceHeader newResource = header;
			newResource.setId(query.lastInsertId().toULongLong());

			return newResource;
		}
	}else{
		throw OpenFailure();
	}
}

void KCloud::ResourcesManager::delResource(const KCloud::ResourceHeader &header) throw (Exception){

	if(isOpen()){
		if(!resourceExists(header.getId())){
			throw ResourceNotExists();
		}
		QSqlQuery query(m_db);
		query.prepare(queryResources_18);
		query.bindValue(placeHolder_id, header.getId());
		tryExec(query);

	}else{
		throw OpenFailure();
	}
}

KCloud::ResourceHeader KCloud::ResourcesManager::getHeader(const quint64 &id) throw (Exception){

	if(isOpen()){
		if(!resourceExists(id)){
			throw ResourceNotExists();
		}
		QSqlQuery query(m_db);
		query.prepare(queryResources_12);
		query.bindValue(placeHolder_id, id);
		tryExec(query);
		if(!query.size()){
			throw ResourceNotExists();
		}else if(query.size() == 1){
			query.seek(0);
			ResourceHeader result;
			result.setId(query.value(res_Id).toULongLong());
			result.setParentId(query.value(res_Parent).toULongLong());
			result.setOwner(query.value(res_Owner).toString());
			result.setName(query.value(res_Name).toString());
			result.m_type = (query.value(res_Type).toString() == sqlEnumDir ? ResourceHeader::Dir : ResourceHeader::File);
			result.m_size = query.value(res_Size).toLongLong();
			result.setPublicPermission(getPublicPermission(result.getId()));
			result.setPermissionTable(getSharedPermission(result.getId()));
			return result;
		}else{
			throw MultipleRowsForPrimaryKey();
		}
	}else{
		throw OpenFailure();
	}
}

QList<KCloud::ResourceHeader> KCloud::ResourcesManager::getChilds(const quint64 &id, const KCloud::ResourcesManager::ChildGetMode &mode) throw (Exception){

	if(isOpen()){
		if(!resourceExists(id)){
			throw ResourceNotExists();
		}
		QList<ResourceHeader> result;
		QSqlQuery query(m_db);
		query.prepare(queryResources_16);
		query.bindValue(placeHolder_id, id);
		query.bindValue(placeHolder_type, mode == OnlyDirs ? sqlEnumDir : sqlEnumFile);
		tryExec(query);
		while(query.next()){
			result << getHeader(query.value(res_Id).toULongLong());
			trace << "result.size() = " << result.size();
		}
		return result;
	}else{
		throw OpenFailure();
	}
}

QList<KCloud::ResourceHeader> KCloud::ResourcesManager::getOwned(const KCloud::User &usr) throw (Exception){

	if(isOpen()){
		if(!userExists(usr)){
			throw UserNotExists();
		}
		QList<ResourceHeader> result;
		QSqlQuery query(m_db);
		query.prepare(queryResources_19);
		query.bindValue(placeHolder_mail, usr.getEmail());
		tryExec(query);
		while(query.next()){
			ResourceHeader item;
			item.setId(query.value(res_Id).toULongLong());
			item.setParentId(query.value(res_Parent).toULongLong());
			item.setOwner(query.value(res_Owner).toString());
			item.setName(query.value(res_Name).toString());
			item.m_type = (query.value(res_Type).toString() == sqlEnumDir ? ResourceHeader::Dir : ResourceHeader::File);
			item.m_size = query.value(res_Size).toLongLong();
			item.setPermissionTable(getSharedPermission(item.getId()));
			item.setPublicPermission(getPublicPermission(item.getId()));
			result << item;
		}
		return result;
	}else{
		throw OpenFailure();
	}
}

QList<KCloud::ResourceHeader> KCloud::ResourcesManager::getShared(const KCloud::User &usr) throw (Exception){

	if(isOpen()){
		if(!userExists(usr)){
			throw UserNotExists();
		}
		QList<ResourceHeader> result;
		QSqlQuery query(m_db);
		query.prepare(queryResources_20);
		query.bindValue(placeHolder_mail, usr.getEmail());
		tryExec(query);
		while(query.next()){
			ResourceHeader item;
			item.setId(query.value(res_Id).toULongLong());
			item.setParentId(query.value(res_Parent).toULongLong());
			item.setOwner(query.value(res_Owner).toString());
			item.setName(query.value(res_Name).toString());
			item.m_type = (query.value(res_Type).toString() == sqlEnumDir ? ResourceHeader::Dir : ResourceHeader::File);
			item.m_size = query.value(res_Size).toLongLong();
			if(sharedPerm(usr, item.getId()) != ResourceHeader::PermUndef){
				item.addPermission(usr.getEmail(), sharedPerm(usr, item.getId()));
			}
			item.setPublicPermission(getPublicPermission(item.getId()));
			result << item;
		}
		return result;
	}else{
		throw OpenFailure();
	}
}

QList<KCloud::ResourceHeader> KCloud::ResourcesManager::getPublic() throw (Exception){
	if(isOpen()){
		QList<ResourceHeader> result;
		QSqlQuery query(m_db);
		query.prepare(queryResources_21);
		tryExec(query);
		while(query.next()){
			ResourceHeader item;
			item.setId(query.value(res_Id).toULongLong());
			item.setParentId(query.value(res_Parent).toULongLong());
			item.setOwner(query.value(res_Owner).toString());
			item.setName(query.value(res_Name).toString());
			item.m_type = (query.value(res_Type).toString() == sqlEnumDir ? ResourceHeader::Dir : ResourceHeader::File);
			item.m_size = query.value(res_Size).toLongLong();
			item.setPublicPermission(getPublicPermission(item.getId()));
			result << item;
		}
		return result;
	}else{
		throw OpenFailure();
	}
}

void KCloud::ResourcesManager::recursiveShare(KCloud::ResourceHeader &head, int i){

	QList<ResourceHeader> childrens = getChilds(head.getId(), OnlyFiles) + getChilds(head.getId(), OnlyDirs);

	deletePublic(head);
	deleteAllSharing(head);

	switch (head.getPublicPermission()) {
		case ResourceHeader::Write:
			setPublicPermission(head);
			foreach (ResourceHeader child, childrens) {
				child.setPublicPermission(head.getPublicPermission());
				recursiveShare(child);
			}
			break;
		case ResourceHeader::Read:

			if(i == 0){
				QMap<QString, ResourceHeader::ResourcePerm> filtered;
				foreach (QString user, head.getPermissionTable().keys()) {
					if(head.getPermissionTable()[user] == ResourceHeader::Write){
						filtered.insert(user, ResourceHeader::Write);
					}
				}
				head.setPermissionTable(filtered);
			}
			setPublicPermission(head);
			foreach (QString user, head.getPermissionTable()) {
				addSharing(head.getId(), user, ResourceHeader::Write);
			}
			foreach (ResourceHeader child, childrens) {
				child.setPermissionTable(head.getPermissionTable());
				child.setPublicPermission(ResourceHeader::Read);
				recursiveShare(child, 1);
			}
			break;
		default:
			foreach (QString user, head.getPermissionTable().keys()) {
				addSharing(head.getId(), user, head.getPermission(user));
			}
			foreach (ResourceHeader child, childrens) {
				recursiveShare(child);
			}
			break;
	}
	return;
}


