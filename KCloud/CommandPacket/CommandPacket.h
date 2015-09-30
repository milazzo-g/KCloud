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
						UnsetAnswer,				//0
						NotLoggedUser,				//1
						LoginOk,					//2
						WrongEmail,					//3
						WrongPassword,				//4
						AlreadyLogged,				//5
						LogoutOk,					//6
						LogoutFail,					//7
						ResourceTreeOk,				//8
						ResourceTreeError,			//9
						ResourceUpOk,				//10
						ResourceUpFail,				//11
						ResourceUpInvalidPerm,		//12
						ResourceUpSpaceExhausted,	//13
						ResourceUpParentIsNotDir,	//14
						ResourceUpAlreadyExists,	//15
						ResourceUpFinalizeOk,		//16
						ResourceUpFinalizeFail,		//17
						ResourceDownOk,				//18
						ResourceDownFail,			//19
						ResourceDownInvalidId,		//20
						ResourceDownInvalidPerm,	//21
						ResourceDelOk,				//22
						ResourceDelFail,			//23
						ResourceDelInvalidPerm,		//24
						UserRegisterOk,				//25
						UsernameAlreadyInUse,		//26
						UserRegisterFail,			//27
						PasswordChangeOk,			//28
						PasswordChangeFail,			//29
						ResourceModOk,				//30
						ResourceModFail,			//31
						ResourceModInvalidId,		//32
						ResourceModInvalidPerm,		//33
						ResourceSharingOk,			//34
						ResourceSharingInvalidPerm,	//35
						ResourceSharingInvalidId,	//36
						ResourceSharingErrors,		//37
						ResourceSharingFail,		//38
						ServerInternalError			//39
					};

					enum ClientCommand{
						UnsetCommand,
						Login,
						Logout,
						ResourceUp,
                        ResourceDel,
                        ResourceDown,
						ResourceTree,
						UserRegister,
						PasswordChange,
						ResourceMod,
						ResourceSharing
					};

		explicit							CommandPacket(QObject *parent = 0);
											~CommandPacket();

					void					clear();
		virtual		void					prepareForRecv();
		virtual		void					prepareForSend() throw(Exception);

					//mancano modifica risorse, cambio permessi, condivisione
					void					setForLogout();
					void					setForResourceTree();
					void					setForLogin(const User &usr);
					void					setForUserRegister(const User &usr);
					void                    setForResourceDel(const quint64 &id);
					void					setForPasswordChange(const User &usr);
                    void					setForResourceDown(const quint64 &resourceId);
					void					setForResourceUp(const QString &localPath,
															 const User &sessionUser,
															 const quint64 &parentId,
															 const QMap<QString, ResourceHeader::ResourcePerm> &permissionTable = QMap<QString, ResourceHeader::ResourcePerm>(),
															 ResourceHeader::ResourcePerm publicPerm = ResourceHeader::PermUndef) throw (Exception);
					void					setForResourceMod(const ResourceHeader &mod);
					void					setForResourceSharing(const ResourceHeader &head);

					User					getUser() const;
					QStringList				getLastError() const;
					QList<ResourceHeader>	getResourceTree() const;
                    ServerAnswer			getServerAnswer() const;
                    ClientCommand			getClientCommand() const;
					ResourceHeader			getFirstResourceHeader() const;

					void					answerToResourceUp(ServerAnswer answer, const QStringList &errorList = QStringList());
                    void                    answerToResourceDel(ServerAnswer answer, const QStringList &errorList = QStringList());
					void					answerToUserRegister(ServerAnswer answer, const QStringList &errorList = QStringList());
                    void					answerToLogout(ServerAnswer answer, const QStringList &errorStringList = QStringList());
					void					answerToPasswordChange(ServerAnswer answer, const QStringList &errorList = QStringList());
					void					answerToLogin(ServerAnswer answer, const User &usr = User(), const QStringList &errorStringList = QStringList());
                    void					answerToResourceTree(ServerAnswer answer, const QList<ResourceHeader> res, const QStringList &errorList = QStringList());
					void					answerToResourceDown(ServerAnswer answer, const ResourceHeader &head = ResourceHeader(), const QStringList &errorList = QStringList());
					void					answerToResourceMod(ServerAnswer answer, const QStringList &errorList = QStringList());
					void					answerToResourceSharing(ServerAnswer answer, const QStringList &errorList = QStringList());
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
