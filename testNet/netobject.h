#ifndef NETOBJECT_H
#define NETOBJECT_H

#include <QObject>
#include <QFile>
#include <QTcpSocket>
#include <QDataStream>

			//virtual		void	writeOn(QDataStream &stream)	const	= 0;
			//virtual		void	readFrom(QDataStream &stream)			= 0;

namespace KCloud{

	class NetObject : public QObject{
			Q_OBJECT
		public:

			explicit			NetObject(QObject *parent = 0);

			virtual		void	sendThrough(QTcpSocket &sock)	= 0;
			virtual		void	receiveFrom(QTcpSocket &sock)	= 0;

						qint64	getNetworkSize();
						bool	isReady();

		signals:
			void objectSended();
			void objectReceived();
			void changeBlockOnSend(qint64 block);
			void changeBlockOnRecv(qint64 block);

		protected slots:
			virtual		void	behaviorOnSend(const qint64 dim)= 0;
			virtual		void	behaviorOnRecv(const qint64 dim)= 0;
			virtual		void	send(const qint64 block = 0) = 0;
			virtual		void	recv(const qint64 block = 0) = 0;

		protected:
			virtual		qint64	calculateNetworkSize() = 0;
			virtual		void	clear();
						void	setReady();
						void	setNotReady();

						qint64			bytesToSend;
						qint64			bytesToRecv;
						qint64			spareBytesOnSend;
						qint64			spareBytesOnRecv;
						qint64			packetsOnSend;
						qint64			packetsOnRecv;
						qint64			currentBlock;
						qint64			packetSize;
						QTcpSocket *	channel;
		private:
						bool	readyFlag;
	};

	class Resource : public NetObject{
			Q_OBJECT
		public:

			explicit			Resource(QObject *parent = 0);

			virtual		void	sendThrough(QTcpSocket &sock);
			virtual		void	receiveFrom(QTcpSocket &sock);

		public slots:
						bool	setFile(const QString path);
		protected slots:
			virtual		void	behaviorOnSend(const qint64 dim);
			virtual		void	behaviorOnRecv(const qint64 dim);
			virtual		void	send(const qint64 block = 0);
			virtual		void	recv(const qint64 block = 0);
		protected:
			virtual		qint64	calculateNetworkSize();
			virtual		void	clear();
		private:
						QFile *		file;
						QByteArray	buff;

	};

//	QTcpSocket	&operator <<(QTcpSocket		&out, const NetObject &obj);
//	QTcpSocket	&operator >>(QTcpSocket		&inp,		NetObject &obj);

//	QDataStream &operator <<(QDataStream	&out, const NetObject &obj);
//	QDataStream &operator >>(QDataStream	&inp,		NetObject &obj);

}

#endif // NETOBJECT_H
