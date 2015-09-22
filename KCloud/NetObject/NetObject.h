#ifndef NETOBJECT_H
#define NETOBJECT_H

#include "netobject_global.h"

#include <QObject>
#include <QTcpSocket>
#include "../Exceptions/Exceptions.h"

namespace KCloud{

	class NETOBJECTSHARED_EXPORT NetObject : public QObject{

		Q_OBJECT

	public:
		enum Payload{
			Payload_128B,	/**< Dimensione pacchetto: 128 byte. */
			Payload_256B,	/**< Dimensione pacchetto: 256 byte. */
			Payload_512B,	/**< Dimensione pacchetto: 512 byte. */
			Payload_1KB,	/**< Dimensione pacchetto: 1 Kbyte (1024 byte). */
			Payload_2KB,	/**< Dimensione pacchetto: 2 Kbyte (2048 byte). */
			Payload_5KB,	/**< Dimensione pacchetto: 5 Kbyte (5120 byte). */
			Payload_10KB,	/**< Dimensione pacchetto: 10 Kbyte (10240 byte). */
			Payload_20KB,	/**< Dimensione pacchetto: 20 Kbyte (20480 byte). */
			Payload_50KB,	/**< Dimensione pacchetto: 50 Kbyte (51200 byte). */
			Payload_128KB,	/**< Dimensione pacchetto: 128 Kbyte (131072 byte). */
			Payload_256KB,	/**< Dimensione pacchetto: 256 Kbyte (262144 byte). */
			Payload_512KB,	/**< Dimensione pacchetto: 512 Kbyte (524288 byte). */
			Payload_1MB		/**< Dimensione pacchetto: 1 Mbyte (1048576 byte). */
		};

		explicit				NetObject(QObject *parent = 0);
					qint64		getNetworkSize();
					void		clear();
		virtual		void		prepareForSend() throw(Exception);
		virtual		void		prepareForRecv() = 0;
					void		sendThrough(QTcpSocket * sock) throw(Exception);
					void		receiveFrom(QTcpSocket * sock) throw(Exception);
					void		setBytesPerPacket(Payload payload = Payload_512KB) throw(Exception);
					qint64		getBytesPerPacket() const;
	signals:
					void		objectSended();
					void		objectReceived();
					void		changeBlock(qint64 block);
	public slots:

	protected slots:
		virtual		void		send(const qint64 block = 0) = 0;
		virtual		void		recv() throw(Exception) = 0;
		virtual		void		behaviorOnSend(const qint64 dim) throw(Exception) = 0;
		virtual		void		leaveSocket();


	protected:
					void		setReady();
					void		setNotReady();
					bool		isReady() const;
		virtual		qint64		calculateNetworkSize() throw(Exception) = 0;

					bool		m_readyFlag;
					qint64		m_bytesPerPacket;
					qint64		m_bytesCounter;
					qint64		m_packets;
					qint64		m_spareBytes;
					qint64		m_currentBlock;
					QTcpSocket *m_channel;
	};
}

#endif // NETOBJECT_H


