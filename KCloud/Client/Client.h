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
		signals:
						void							newCommand();
						void							serverAnswer(CommandPacket::ServerAnswer answer);
						void							clientError(Exception::Type type);
						void							resourceReceived();
						void							resourceSended();
						void							finalizeOK();
						void							transmissionRate(const qint64 total, const qint64 transmitted, Resource::Mode mod);

		public slots:
			virtual		void							parse() throw (Exception);
						void							login(const QString &mail, const QString &pass, const User::PwdMode mode);
			virtual		void							logout();											//fatto		ok
			virtual		void							resourceUp();										//fatto		ok
			virtual		void							resourceMod(const ResourceHeader &modifiedHead);
			virtual		void							resourceDel(const quint64 &resourceId);				//fatto
			virtual		void							resourceTree();										//fatto
			virtual		void							resourceDown();										//fatto
			virtual		void							userRegister(const QString &email, const QString &password, const User::PwdMode mode);
			virtual		void							resourcePerm();
			virtual		void							resourceShare(const ResourceHeader &head);
			virtual		void							passwordChange(const QString &oldHash, const QString &newHash); // da provare
						void							disconnectFromHost();
						QStringList						lastErrors();

						void							connectToHost(const QString &addr, const quint16 &port);
						void							newUpload(const QString &localPath,
																	User *sessionUser,
																	const quint64 &parentId,
																	const QMap<QString, ResourceHeader::ResourcePerm> &permissionTable = QMap<QString, ResourceHeader::ResourcePerm>(),
																	ResourceHeader::ResourcePerm publicPerm = ResourceHeader::PermUndef) throw (Exception); //setta i parametri per l'upload
						void							newDownload(const quint64 &resourceId, const QString &savePath = "");		//setta i parametri per il download
						QList<ResourceHeader>			getResourceList() const;
						User *							getSessionUser() const;
						void							setSessionUser();								//setta l'utente di sessione dopo la risposta di login ok dal server
						void							saveResourcesTree();							//salva l'albero delle risorse
						void							removeTempFile() throw (Exception);															//rimuove il file temporaneo, chiamata dopo invio ok
						void							finalizeResource() throw (Exception);														//finalizza la risorsa dopo la ricezione
		protected:
						void							run();
		private slots:
						void							closeAll();
						void							execCommand(const QString &cmd);
						QString							clog(const QString &log);
						bool							isLogged() throw(Exception);
						void							notifyReceived();
						void							notifySended();
						void							notifyTransmissionRate(const qint64 total, const qint64 transmitted, Resource::Mode mod);

		private:
						QCoreApplication *				m_coreApplication;
						Console	*						m_console;
						WorkMode						m_workMode;
						CommandPacket::ClientCommand	m_lastCommand;
						ResourceHeader					m_head;
						QList<ResourceHeader>			m_resourcesTree;
						QStringList						m_errors;


	};
}

#endif // CLIENT_H
