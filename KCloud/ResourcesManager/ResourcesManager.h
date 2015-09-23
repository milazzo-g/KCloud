#ifndef RESOURCESMANAGER_H
#define RESOURCESMANAGER_H

#include "resourcesmanager_global.h"

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
			explicit						ResourcesManager(const QString &name, QObject *parent = 0);
                                            ~ResourcesManager();
	};
}

#endif // RESOURCESMANAGER_H
