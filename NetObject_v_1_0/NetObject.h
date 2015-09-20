#ifndef NETOBJECT_H
#define NETOBJECT_H

#include <QObject>
#include <QTcpSocket>

/*!
 *		Namespace delle librerie.
 */
namespace KCloud{
	/*!
	 * \brief	Classe base per oggetti che possono inviati e ricevuti tramite socket TCP.
	 *
	 */
	class NetObject : public QObject{

		Q_OBJECT

	public:
		/*!
		 * \brief	Dimensione dei pacchetti per l'invio e la ricezione.
		 */
		enum Payload{
			Payload_128B,	/**< Setta il pacchetto a 128 byte. */
			Payload_256B,	/**< Setta il pacchetto a 256 byte. */
			Payload_512B,	/**< Setta il pacchetto a 512 byte. */
			Payload_1KB,	/**< Setta il pacchetto a 1 Kbyte (1024 byte). */
			Payload_2KB,	/**< Setta il pacchetto a 2 Kbyte (2048 byte). */
			Payload_5KB,	/**< Setta il pacchetto a 5 Kbyte (5120 byte). */
			Payload_10KB,	/**< Setta il pacchetto a 10 Kbyte (10240 byte). */
			Payload_20KB,	/**< Setta il pacchetto a 20 Kbyte (20480 byte). */
			Payload_50KB,	/**< Setta il pacchetto a 50 Kbyte (51200 byte). */
			Payload_128KB,	/**< Setta il pacchetto a 128 Kbyte (131072 byte). */
			Payload_256KB,	/**< Setta il pacchetto a 256 Kbyte (262144 byte). */
			Payload_512KB,	/**< Setta il pacchetto a 512 Kbyte (524288 byte). */
			Payload_1MB		/**< Setta il pacchetto a 1 Mbyte (1048576 byte). */
		};

		/*!
		 * \brief	Costruttore di default
		 * \param	parent Oggetto padre (vedi <a href="http://doc.qt.io/qt-5/qobject.html">QObject</a>).
		 * \details	Costruisce l'oggetto richiamando {@link clear()} e inizializzando i bytes per pacchetto a {@link Payload_512KB}
		 */
		explicit				NetObject(QObject *parent = 0);

		/*!
		* \brief	Calcola la dimensione di rete dell'oggetto intesa come bytes da inviare sulla socket.
		* \return	Il numero di bytes calcolati tramite {@link calculateNetworkSize()}.
		*/
					qint64		getNetworkSize();
		/*!
		 * \brief	Azzera i valori dell'oggetto.
		 * \details	Setta a zero {@link m_bytesCounter}, {@link m_packets}, {@link m_spareBytes}, {@link m_currentBlock}, mette
		 *			il puntatore alla socket {@link m_channel} a NULL, ed infine setta l'oggetto stesso come non pronto {@link setNotReady()}
		 */
		virtual		void		clear();
		/*!
		 * \brief	Inizializza i dati per poter avviare la trasmissione.
		 * \details	Dopo aver chiamato {@link clear()}, setta il numero dei pacchetti {@link m_packets} facendo la divisione tra la dimensione
		 *			totale e quella di un pacchetto	(si vedano {@link getNetworkSize()} e {@link getBytesPerPacket()}), il numero di bytes
		 *			rimanenti {@link m_spareBytes} con il resto della divisione tra i due precedenti, ed infine, inizializza anche il contatore
		 *			dei bytes {@link m_bytesCounter} con il valore oppportuno in base al numero dei pacchetti.
		 */
		virtual		void		prepareForSend();
		virtual		void		prepareForRecv() = 0;
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
		virtual		void		send(const qint64 block = 0) = 0;
		virtual		void		recv() = 0;
		virtual		void		behaviorOnSend(const qint64 dim) = 0;
		virtual		void		leaveSocket();


	protected:
					void		setReady();
					void		setNotReady();
					bool		isReady() const;
		virtual		qint64		calculateNetworkSize() = 0;

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
