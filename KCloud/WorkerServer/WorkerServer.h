#ifndef WORKERSERVER_H
#define WORKERSERVER_H

/*
 *
 * GIUSEPPE
 *
 */

#include "workerserver_global.h"

#include <QDir>
#include <QDateTime>
#include <QSettings>
#include <QHostAddress>

#include "../CommandPacket/CommandPacket.h"
#include "../ResourcesManager/ResourcesManager.h"
#include "../UsersManager/UsersManager.h"
#include "../Console/Console.h"
#include "../Engine/Engine.h"

namespace KCloud{

	class WORKERSERVERSHARED_EXPORT WorkerServer : public Engine{
		Q_OBJECT
		public:
											WorkerServer(int sd, QObject *parent = 0);
											~WorkerServer();
		signals:
						void				consoleOutRequest(QString msg);
						void				removeFromActiveHandlers(QString add);
		protected:
						void				run();
		public slots:
			virtual		void				parse();
			virtual		void				login();
			virtual		void				logout();
			virtual		void				resourceUp();
			virtual		void				resourceMod();
			virtual		void				resourceDel();
			virtual		void				resourceTree();
			virtual		void				resourceDown();
			virtual		void				userRegister();
			virtual		void				resourcePerm();
			virtual		void				resourceShare();
			virtual		void				passwordChange();

		private slots:
						void				forcedLogout();
						void				finalizeUpload();
		public:
						QString				address() const;
						QString				keyFirst() const;
						QString				keyLast() const;
		private:
						bool				userIsLogged() const;
						void				clog(const QString &log);
						void				removeDir(const QString &path);
						ResourceHeader		m_head;
						UsersManager *		m_usersManager;
						ResourcesManager *	m_resourcesManager;
						QDir				m_dir;
						QString				m_key1;
						QString				m_key2;


	};
}
#endif // WORKERSERVER_H
