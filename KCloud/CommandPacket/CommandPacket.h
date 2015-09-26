#ifndef COMMANDPACKET_H
#define COMMANDPACKET_H
/*
 *			DANILO
 *
 * */

#include "commandpacket_global.h"

#include <QByteArray>
#include <QStringList>

#include "../User/User.h"
#include "../NetObject/NetObject.h"
#include "../Exceptions/Exceptions.h"
#include "../ResourceHeader/ResourceHeader.h"

namespace KCloud{

	class COMMANDPACKETSHARED_EXPORT CommandPacket : public NetObject{
		Q_OBJECT

		friend			QDataStream &		operator<<(QDataStream &out, const CommandPacket &tmp);
		friend			QDataStream &		operator>>(QDataStream &inp, CommandPacket &tmp);
		public:
					enum ServerAnswer{
						UnsetAnswer,
						NotLoggedUser,
						LoginOk,
                        WrongEmail,
                        WrongPassword,
						AlreadyLogged,
						LogoutOk,
						LogoutFail,
						ResourceTreeOk,
						ResourceTreeError,
						ResourceUpOk,
                        ResourceUpFail,
                        ResourceUpInvalidPerm,
						ResourceUpSpaceExhausted,
						ResourceUpFinalizeOk,
						ResourceUpFinalizeFail,
						ResourceDownOk,
                        ResourceDownFail,
                        ResourceDownInvalidId,
                        ResourceDelOk,
                        ResourceDelFail,
						ResourceDelInvalidPerm,
						ServerInternalError
					};

					enum ClientCommand{
						UnsetCommand,
						Login,
						Logout,
						ResourceUp,
                        ResourceDel,
                        ResourceDown,
                        ResourceTree

					};

		explicit							CommandPacket(QObject *parent = 0);
											~CommandPacket();

					void					clear();
		virtual		void					prepareForSend() throw(Exception);
		virtual		void					prepareForRecv();

                    //mancano modifica risorse, cambio permessi, condivisione, cambio password e altro....
                    void					setForLogin(const User &usr);
					void					setForLogout();
                    void                    setForResourceDel(const quint64 &id);
					void					setForResourceTree();
                    void					setForResourceDown(const quint64 &resourceId);
					void					setForResourceUp(const QString &localPath,
															 const User &sessionUser,
															 const quint64 &parentId,
															 const QMap<QString, ResourceHeader::ResourcePerm> &permissionTable = QMap<QString, ResourceHeader::ResourcePerm>(),
															 ResourceHeader::ResourcePerm publicPerm = ResourceHeader::PermUndef) throw (Exception);

					User					getUser() const;
					QStringList				getLastError() const;
                    ServerAnswer			getServerAnswer() const;
                    ClientCommand			getClientCommand() const;
					ResourceHeader			getFirstResourceHeader() const;
                    QList<ResourceHeader>	getResourceTree() const;

                    void                    answerToResourceDel(ServerAnswer answer, const QStringList &errorList = QStringList());
					void					answerToResourceUp(ServerAnswer answer, const QStringList &errorList = QStringList());
                    void					answerToLogout(ServerAnswer answer, const QStringList &errorStringList = QStringList());
					void					answerToResourceDown(ServerAnswer answer, const ResourceHeader &head = ResourceHeader(), const QStringList &errorList = QStringList());
					void					answerToLogin(ServerAnswer answer, const User &usr = User(), const QStringList &errorStringList = QStringList());
                    void					answerToResourceTree(ServerAnswer answer, const QList<ResourceHeader> res, const QStringList &errorList = QStringList());


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
					void					setServerAnswer(ServerAnswer cmd);
                    void					setClientCommand(ClientCommand cmd);
					void					setErrorStringList(const QStringList &err);
					void					setFirstResourceHeader(const ResourceHeader &res);
	};

    COMMANDPACKETSHARED_EXPORT QDataStream &operator<<(QDataStream &out, const CommandPacket &tmp);
	COMMANDPACKETSHARED_EXPORT QDataStream &operator>>(QDataStream &inp, CommandPacket &tmp);
	COMMANDPACKETSHARED_EXPORT QDataStream &operator<<(QDataStream &out, const CommandPacket::ClientCommand &tmp);
	COMMANDPACKETSHARED_EXPORT QDataStream &operator>>(QDataStream &inp, CommandPacket::ClientCommand &tmp);
	COMMANDPACKETSHARED_EXPORT QDataStream &operator<<(QDataStream &out, const CommandPacket::ServerAnswer &tmp);
	COMMANDPACKETSHARED_EXPORT QDataStream &operator>>(QDataStream &inp, CommandPacket::ServerAnswer &tmp);
}

#endif // COMMANDPACKET_H
