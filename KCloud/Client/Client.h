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
						void							login() throw (Exception);							//fatto		ok
			virtual		void							logout();											//fatto		ok
			virtual		void							resourceUp();										//fatto		ok
			virtual		void							resourceMod();
			virtual		void							resourceDel();										//non utilizzata
			virtual		void							resourceTree() throw (KCloud::Exception); 			//fatto
			virtual		void							resourceDown();										//fatto
			virtual		void							userRegister();
			virtual		void							resourcePerm();
			virtual		void							resourceShare();
			virtual		void							passwordChange();

						void							setUserForLogin(const QString &email, const QString &pwd) throw (Exception);			//setta l'utente prima del login
						void							connectToHost(const QString &addr, const quint16 &port) throw (Exception);				//setta i parametri di connessione
						void							newUpload(const QString &localPath,
																	User *sessionUser,
																	const quint64 &parentId,
																	const QMap<QString, ResourceHeader::ResourcePerm> &permissionTable = QMap<QString, ResourceHeader::ResourcePerm>(),
																	ResourceHeader::ResourcePerm publicPerm = ResourceHeader::PermUndef) throw (Exception); //setta i parametri per l'upload
						void							newDownload(const quint64 &resourceId, const QString &savePath = "") throw (Exception);		//setta i parametri per il download
						void							newRemove(const quint64 &resourceId) throw (Exception);				//setta i parametri per la rimozione
						void							setSessionUser();								//setta l'utente di sessione dopo la risposta di login ok dal server
						void							saveResourcesTree();							//salva l'albero delle risorse
						void							removeTempFile() throw (Exception);															//rimuove il file temporaneo, chiamata dopo invio ok
						void							finalizeResource() throw (Exception);														//finalizza la risorsa dopo la ricezione
		protected:
						void							run();
		private slots:
						void							closeAll();
						void							execCommand(const QString &cmd);
						void							clog(const QString &log);
						bool							isLogged() throw(Exception);
		private:
						QCoreApplication *				m_coreApplication;
						Console	*						m_console;
						WorkMode						m_workMode;
						CommandPacket::ClientCommand	m_lastCommand;
						ResourceHeader					m_head;
						QList<ResourceHeader>			m_resourcesTree;


	};
}

#endif // CLIENT_H
