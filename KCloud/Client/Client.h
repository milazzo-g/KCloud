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
			static	const	QString							MSG_00;
			static	const	QString							MSG_01;
			static	const	QString							MSG_02;
			static	const	QString							MSG_03;
			static	const	QString							MSG_04;
			static	const	QString							MSG_05;
			static	const	QString							MSG_06;
			static	const	QString							MSG_07;
			static	const	QString							MSG_08;
			static	const	QString							MSG_09;
			static	const	QString							MSG_10;
			static	const	QString							MSG_11;
			static	const	QString							MSG_12;
			static	const	QString							MSG_13;
			static	const	QString							MSG_14;
			static	const	QString							MSG_15;
			static	const	QString							MSG_16;
			static	const	QString							MSG_17;
			static	const	QString							MSG_18;
			static	const	QString							MSG_19;
			static	const	QString							MSG_20;
			static	const	QString							MSG_21;
			static	const	QString							MSG_22;
			static	const	QString							MSG_23;
			static	const	QString							MSG_24;
			static	const	QString							MSG_25;
			static	const	QString							MSG_26;
			static	const	QString							MSG_27;
			static	const	QString							MSG_28;
			static	const	QString							MSG_29;
			static	const	QString							MSG_30;
			static	const	QString							MSG_31;
			static	const	QString							MSG_32;
			static	const	QString							MSG_33;
			static	const	QString							MSG_34;
			static	const	QString							MSG_35;
			static	const	QString							MSG_36;
			static	const	QString							MSG_37;
			static	const	QString							MSG_38;
			static	const	QString							MSG_39;
			static	const	QString							MSG_40;

		signals:
							void							serverAnswer(CommandPacket::ServerAnswer answer);
							void							clientError(Exception::Type type);


							void							commandReceived	();
							void							resourceReceived();
							void							resourceSended();
							void							decompressionStart();
							void							decompressionEnd();
							void							serverFinalized();

							void							transmissionRate(const qint64 total, const qint64 transmitted, Resource::Mode mod);

		public slots:

							void							parse() throw (Exception);
							void							login(const QString &mail, const QString &pass, const User::PwdMode mode);
							void							logout();
							void							resourceUp();
							void							resourceMod(const ResourceHeader &modifiedHead);
							void							resourceDel(const quint64 &resourceId);
							void							resourceTree();
							void							resourceDown();
							void							userRegister(const QString &email, const QString &password, const User::PwdMode mode);
							void							resourcePerm();
							void							resourceShare(const ResourceHeader &head);
							void							passwordChange(const QString &oldHash, const QString &newHash); // da provare
							void							disconnectFromHost();
							QStringList						lastErrors();

							void							connectToHost(const QString &addr, const quint16 &port);
							void							newUpload(const		QString &localPath,
																				User *	sessionUser,
																	  const		quint64 &parentId,
																	  const		QMap<QString, ResourceHeader::ResourcePerm> &permissionTable = QMap<QString, ResourceHeader::ResourcePerm>(),
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
