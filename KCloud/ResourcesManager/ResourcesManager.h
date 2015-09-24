#ifndef RESOURCESMANAGER_H
#define RESOURCESMANAGER_H

#include "resourcesmanager_global.h"

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
				UserWrongHash
			};
			explicit										ResourcesManager(const QString &name, QObject *parent = 0);
															~ResourcesManager();
							ResourcesManagerAnswer			checkForUpload(ResourceHeader &r);
		private:
							bool							isOwner(const User &usr, const quint64 &id) throw (Exception);
							bool							exists(ResourceHeader &header) throw (Exception);
							ResourceHeader::ResourceType	getType(const quint64 &id) throw (Exception);
							ResourceHeader::ResourcePerm	publicPerm(const quint64 &id) throw (Exception);
							ResourceHeader::ResourcePerm	sharedPerm(const User &usr, const quint64 &id) throw (Exception);


			const	static	QString							queryResources_1;
			const	static	QString							queryResources_2;
			const	static	QString							queryResources_3;
			const	static	QString							queryResources_4;
			const	static	QString							queryResources_5;
	};
}

#endif // RESOURCESMANAGER_H
