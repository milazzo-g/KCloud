#ifndef ENGINE_H
#define ENGINE_H

/*
 *
 * GIUSEPPE
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
											~Engine();
		signals:
						void				userLogin();
						void				userLogout();
						void				socketStateChanged(QAbstractSocket::SocketState stat);

		public slots:
						void				sendCommand();
						void				sendResource();
						void				receiveCommand();
						void				receiveResource();
			virtual		void				parse()				= 0;
			virtual		void				login()				= 0;
			virtual		void				logout()			= 0;
			virtual		void				resourceUp()		= 0;
			virtual		void				resourceMod()		= 0;
			virtual		void				resourceDel()		= 0;
			virtual		void				resourceTree()		= 0;
			virtual		void				resourceDown()		= 0;
			virtual		void				userRegister()		= 0;
			virtual		void				resourcePerm()		= 0;
			virtual		void				resourceShare()		= 0;
			virtual		void				passwordChange()	= 0;

		protected:
						QTcpSocket *		m_socket;
						Resource *			m_resource;
						CommandPacket *		m_packet;
						User *				m_user;
		private slots:
						void				notifySocketState(QAbstractSocket::SocketState stat);
	};
}

#endif // ENGINE_H
