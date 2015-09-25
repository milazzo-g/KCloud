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
				UserOK,
				UserNotFound,
				UserWrongHash,
				PermError,
				SpaceFull,
				UploadOK,
				AlreadyExists
			};
			explicit										ResourcesManager(const QString &name, QObject *parent = 0);
															~ResourcesManager();
							ResourcesManagerAnswer			checkForUpload(const User &usr, ResourceHeader &head) throw (Exception);
							ResourcesManagerAnswer			aBadassFunction(const QString &path, const ResourceHeader &head, const User &usr, QStringList &errors) throw (Exception);
		private:

							bool							isOwner(const User &usr, const quint64 &id) throw (Exception);
							bool							exists(ResourceHeader &header) throw (Exception);
							qint64							userSpace(const User &usr) throw (Exception);
							ResourceHeader::ResourceType	getType(const quint64 &id) throw (Exception);
							ResourceHeader::ResourcePerm	publicPerm(const quint64 &id) throw (Exception);
							ResourceHeader::ResourcePerm	sharedPerm(const User &usr, const quint64 &id) throw (Exception);
							void							recursiveAdd(const QString &path, const ResourceHeader &head, const User &usr, QStringList &errors) throw (Exception);

			const	static	QString							queryResources_1;
			const	static	QString							queryResources_2;
			const	static	QString							queryResources_3;
			const	static	QString							queryResources_4;
			const	static	QString							queryResources_5;
			const	static	QString							queryResources_6;
			const	static	QString							queryResources_7;
			const	static	QString							queryResources_8;
			const	static	QString							queryResources_9;
	};
}

#endif // RESOURCESMANAGER_H
