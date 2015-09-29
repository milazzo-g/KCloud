#ifndef RESOURCESMANAGER_H
#define RESOURCESMANAGER_H

#include "resourcesmanager_global.h"

#include <QFile>
#include <QSettings>

#include "../User/User.h"
#include "../Resource/Resource.h"
#include "../Exceptions/Exceptions.h"
#include "../ResourceHeader/ResourceHeader.h"
#include "../DatabaseManager/DatabaseManager.h"

namespace KCloud{
	class RESOURCESMANAGERSHARED_EXPORT ResourcesManager : public DatabaseManager{
		Q_OBJECT
		public:
			enum ResourcesManagerAnswer{
				UserOK,					//	0
				UserNotFound,			//	1
				UserWrongHash,			//	2
				PermError,				//	3
				SpaceFull,				//	4
				UploadOK,				//	5
				AlreadyExists,			//	6
				ParentNotFound,			//	7
				NotADir,				//	8
				NotAFile,				//	9
				FinalizeOK,				//	10
				FinalizeIssues,			//	11
				RecursiveGetOK,			//	12
				RecursiveGetFail,		//	13
				ResourceModOk,			//	14
				ResourceNotExist,		//	15
				DeleteOK,				//	16
				SharingOk				//	17
			};
			explicit										ResourcesManager(const QString &name, QObject *parent = 0);
															~ResourcesManager();
							ResourceHeader					headerForId(const quint64 &id) throw (Exception);
							ResourcesManagerAnswer			checkForUpload(const User &usr, ResourceHeader &head) throw (Exception);
							ResourcesManagerAnswer			addResources(const QString &path, const ResourceHeader &incomplete, QStringList &errors) throw (Exception);
							ResourcesManagerAnswer			getResources(const User &usr, const QString &path, const ResourceHeader &resource, QStringList &filesMoved) throw (Exception);
							ResourcesManagerAnswer			delResources(const User &usr, const ResourceHeader &head) throw (Exception);
							ResourcesManagerAnswer			modResource(const User &usr, const ResourceHeader &head) throw (Exception);
							ResourcesManagerAnswer			shareResource(const User &usr, const ResourceHeader &head, QStringList &errorUsersShare) throw (Exception);
							QList<ResourceHeader>			resourceTree(const User &usr);
		private:
			enum SpaceUpdateMode{
				Increment,
				Decrement
			};

			enum ChildGetMode{
				OnlyDirs,
				OnlyFiles
			};

							ResourceHeader								getHeader(const quint64 &id) throw (Exception);
							bool										isOwner(const User &usr, const quint64 &id) throw (Exception);
							bool										isOwner(const QString &usr, const quint64 &id) throw (Exception);
							bool										resourceExists(ResourceHeader &header) throw (Exception);
							bool										resourceExists(const quint64 &id) throw (Exception);
							bool										userExists(const User &usr) throw (Exception);
							bool										userExists(const QString &usr) throw (Exception);
							qint64										userSpace(const QString &usr) throw (Exception);
							qint64										resourceSize(const quint64 &id) throw (Exception);
							ResourceHeader::ResourceType				resourceType(const quint64 &id) throw (Exception);
							ResourceHeader::ResourcePerm				publicPerm(const quint64 &id) throw (Exception);
							ResourceHeader::ResourcePerm				sharedPerm(const User &usr, const quint64 &id) throw (Exception);
							QStringList									recursiveAdd(const QString &path, const ResourceHeader &incomplete) throw (Exception);
							QStringList									recursiveGet(const QString &path, const ResourceHeader &item) throw (Exception);
							void										recursiveDel(const ResourceHeader &head) throw (Exception);
							void										updateSpace(const QString &usr, const quint64 &id, const SpaceUpdateMode mode) throw (Exception);
							void										setPublicPermission(const ResourceHeader &header) throw (Exception);
							ResourceHeader::ResourcePerm				getPublicPermission(const quint64 &id) throw (Exception);
							QStringList									setSharedPermission(const ResourceHeader &header) throw (Exception);
							QMap<QString, ResourceHeader::ResourcePerm>	getSharedPermission(const quint64 &id) throw (Exception);
							ResourceHeader								addResource(const ResourceHeader &header) throw (Exception);
							void										delResource(const ResourceHeader &header) throw (Exception);
							QList<ResourceHeader>						getFileChild(const quint64 &id) throw (Exception);
							QList<ResourceHeader>						getChilds(const quint64 &id, const ChildGetMode &mode) throw (Exception);
							QList<ResourceHeader>						getOwned(const User &usr) throw (Exception);
							QList<ResourceHeader>						getShared(const User &usr) throw (Exception);
							QList<ResourceHeader>						getPublic() throw (Exception);

							void										updateResourceName(const quint64 &id, const QString &name) throw (Exception);
							void										updatePublicPermission(const ResourceHeader &head) throw (Exception);
							void										deleteSharing(const quint64 &id, const QString &user) throw (Exception);
							void										updareSharing(const quint64 &id, const QString &user, ResourceHeader::ResourcePerm perm) throw (Exception);
							void										addSharing(const quint64 &id, const QString &user, ResourceHeader::ResourcePerm perm) throw (Exception);

			const	static	QString							queryResources_1;
			const	static	QString							queryResources_2;
			const	static	QString							queryResources_3;
			const	static	QString							queryResources_4;
			const	static	QString							queryResources_5;
			const	static	QString							queryResources_6;
			const	static	QString							queryResources_7;
			const	static	QString							queryResources_8;
			const	static	QString							queryResources_9;
			const	static	QString							queryResources_10;
			const	static	QString							queryResources_11;
			const	static	QString							queryResources_12;
			const	static	QString							queryResources_13;
			const	static	QString							queryResources_14;
			const	static	QString							queryResources_15;
			const	static	QString							queryResources_16;
			const	static	QString							queryResources_17;
			const	static	QString							queryResources_18;
			const	static	QString							queryResources_19;
			const	static	QString							queryResources_20;
			const	static	QString							queryResources_21;
			const	static	QString							queryResources_22;
			const	static	QString							queryResources_23;
			const	static	QString							queryResources_24;
			const	static	QString							queryResources_25;
	};
}

#endif // RESOURCESMANAGER_H
