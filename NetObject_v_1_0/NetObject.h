#ifndef NETOBJECT_H
#define NETOBJECT_H

#include <QObject>
#include <QTcpSocket>

namespace KCloud{
	class NetObject : public QObject{
		Q_OBJECT
	public:
		enum Payload{
			Payload_128B,
			Payload_256B,
			Payload_512B,
			Payload_1KB,
			Payload_2KB,
			Payload_5KB,
			Payload_10KB,
			Payload_20KB,
			Payload_50KB,
			Payload_128KB,
			Payload_256KB,
			Payload_512KB,
			Payload_1MB
		};

		explicit NetObject(QObject *parent = 0);

					qint64		getNetworkSize();
					void		clear();
					void		prepare();
					void		sendThrough(QTcpSocket * sock);
					void		receiveFrom(QTcpSocket * sock);
					void		setBytesPerPacket(Payload payload = Payload_512KB);
					qint64		getBytesPerPacket() const;
	signals:
					void		objectSended();
					void		objectReceived();
					void		changeBlock(qint64 block);
	public slots:

	protected slots:
		virtual		void		send(const qint64 dim) = 0;
		virtual		void		recv(const qint64 dim) = 0;

	protected:
					void		setReady();
					void		setNotReady();
					bool		isReady();
		virtual		qint64		calculateNetworkSize() = 0;

					qint64		bytesPerPacket;
					qint64		bytesCounter;
					qint64		packets;
					qint64		spareBytes;
					qint64		currentBlock;
					QTcpSocket *channel;
	};
}

#endif // NETOBJECT_H
