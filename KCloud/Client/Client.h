#ifndef CLIENT_H
#define CLIENT_H

#include "client_global.h"

#include <QDir>
#include <QSettings>
#include <QCoreApplication>

#include "../Engine/Engine.h"
#include "../Console/Console.h"
#include "../Resource/Resource.h"
#include "../MainServer/defines.h"
#include "../Exceptions/Exceptions.h"
#include "../CommandPacket/CommandPacket.h"

namespace KCloud{

	class CLIENTSHARED_EXPORT Client : public Engine{
		Q_OBJECT
		public:
			enum WorkMode{
				AsGuiThread,
				AsConsoleThread
			};
														Client(const WorkMode mode, QObject *parent = 0);
														~Client();
		public slots:
			virtual		void							parse() throw (Exception);
					void							login() throw (Exception);
			virtual		void							logout();
			virtual		void							resourceUp();
			virtual		void							resourceMod();
			virtual		void							resourceDel();
			virtual		void							resourceTree();
			virtual		void							resourceDown();
			virtual		void							userRegister();
			virtual		void							resourcePerm();
			virtual		void							resourceShare();
			virtual		void							passwordChange();

						void							setUserForLogin(const QString &email, const QString &pwd) throw (Exception);
						void							connectToHost(const QString &addr, const quint16 &port) throw (Exception);
						void							newUpload(const QString &localPath,
																	User *sessionUser,
																	const quint64 &parentId,
																	const QMap<QString, ResourceHeader::ResourcePerm> &permissionTable = QMap<QString, ResourceHeader::ResourcePerm>(),
																	ResourceHeader::ResourcePerm publicPerm = ResourceHeader::PermUndef) throw (Exception);
						void							newDownload(const quint64 &resourceId, const QString &savePath = "") throw (Exception);

						void							removeTempFile() throw (Exception);
						void							finalizeResource() throw (Exception);
		protected:
						void							run();
		private slots:
						void							closeAll();
						void							execCommand(const QString &cmd);
						void							clog(const QString &log);
		private:
						QCoreApplication *				m_coreApplication;
						Console	*						m_console;
						WorkMode						m_workMode;
						CommandPacket::ClientCommand	m_lastCommand;
						ResourceHeader					m_head;


	};
}

#endif // CLIENT_H
