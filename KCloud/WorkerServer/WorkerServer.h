#ifndef WORKERSERVER_H
#define WORKERSERVER_H

#include "workerserver_global.h"

#include "../ResourcesManager/ResourcesManager.h"
#include "../UsersManager/UsersManager.h"
#include "../Engine/Engine.h"

namespace KCloud{

	class WORKERSERVERSHARED_EXPORT WorkerServer : public Engine{
		Q_OBJECT
		public:
											WorkerServer(int sd, QObject *parent = 0);
											~WorkerServer();
		private:
						UsersManager		m_usersManager;
						ResourcesManager	m_resourcesManager;


	};
}
#endif // WORKERSERVER_H
