#ifndef ENGINE_H
#define ENGINE_H

/*
 *
 * DANILO
 *
 */

#include "engine_global.h"
#include <QObject>
#include <QThread>
#include <QCryptographicHash>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QAbstractSocket>

#include "../User/User.h"
#include "../Resource/Resource.h"
#include "../CommandPacket/CommandPacket.h"

namespace KCloud{

	class ENGINESHARED_EXPORT Engine : public QThread{
		Q_OBJECT
		public:
			explicit						Engine(QObject *parent = 0);
			virtual							~Engine() = 0;
		signals:
						void				commandSended();
						void				userLogin();
						void				userLogout();
						void				connected();
						void				disconnected();
						void				socketError(QAbstractSocket::SocketError error);
						void				compressionStart();
						void				compressionEnd();

		public slots:
						void				sendCommand();
						void				sendResource();
						void				receiveCommand();
						void				receiveResource();
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

		protected:
						QTcpSocket *		m_socket;
						Resource *			m_resource;
						CommandPacket *		m_packet;
						User *				m_user;
		private slots:
						void				notifyConnect();
						void				notifyDisconnect();
						void				notifySocketError(QAbstractSocket::SocketError error);
	};
}

#endif // ENGINE_H
