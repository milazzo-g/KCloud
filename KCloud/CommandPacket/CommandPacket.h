#ifndef COMMANDPACKET_H
#define COMMANDPACKET_H
/*
 *			DANILO
 *
 * */

#include "commandpacket_global.h"

#include <QStringList>

#include "../User/User.h"
#include "../NetObject/NetObject.h"
#include "../ResourceHeader/ResourceHeader.h"

namespace KCloud{

	class COMMANDPACKETSHARED_EXPORT CommandPacket : public NetObject{
		Q_OBJECT

		public:
					enum ServerAnswer{
						UnsetAnswer,
						LoginOk,
						AlreadyLogged,
						WrongPassword,
						WrongEmail,
						LogoutOk,
						LogoutFail


					};

					enum ClientCommand{
						UnsetCommand,
						Login,
						Logout

					};

		explicit							CommandPacket(QObject *parent = 0);

					void					clear();
		virtual		void					prepareForSend() throw(Exception);
		virtual		void					prepareForRecv();

					void					setForLogin(const User &usr);
					void					setForLogout(const User &usr);

					User					getUser() const;
					void					answerToLogin(const User &usr, ServerAnswer answer, const QString &errorString = "");




		protected slots:

		virtual		void					send(const qint64 block = 0);
		virtual		void					recv() throw(Exception);
		virtual		void					behaviorOnSend(const qint64 dim) throw(Exception);

		protected:

		virtual		qint64					calculateNetworkSize() throw(Exception);

		private:

					User					m_currentUser;
					QStringList				m_stringList;
					ServerAnswer			m_serverAnswer;
					ClientCommand			m_clientCommand;
					QList<ResourceHeader>	m_headersList;

					void					setUser(const User &usr);
					void					setClientCommand(ClientCommand cmd);
					void					setServerAnswer(ServerAnswer cmd);
					void					setErrorString(const QString &err);

	};
}

#endif // COMMANDPACKET_H
