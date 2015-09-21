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
	 */
	class NetObject : public QObject{

		Q_OBJECT

	public:
		/*!
		 * \brief	Dimensione dei pacchetti per l'invio e la ricezione.
		 */
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
		 *			il puntatore alla socket {@link m_channel} a NULL, ed infine setta l'oggetto stesso come non pronto (vedi {@link setNotReady()})
		 */
					void		clear();
		/*!
		 * \brief	Inizializza i dati per poter avviare la trasmissione.
		 * \details	Dopo aver chiamato {@link clear()}, setta il numero dei pacchetti {@link m_packets} facendo la divisione tra la dimensione
		 *			totale e quella di un pacchetto	(si vedano {@link getNetworkSize()} e {@link getBytesPerPacket()}), il numero di bytes
		 *			rimanenti {@link m_spareBytes} con il resto della divisione tra i due precedenti, ed infine, inizializza anche il contatore
		 *			dei bytes {@link m_bytesCounter} con il valore oppportuno in base al numero dei pacchetti.
		 */
		virtual		void		prepareForSend();
		/*!
		 * \brief	Non implementata.
		 */
		virtual		void		prepareForRecv() = 0;
		/*!
		* \brief	Manda l'oggetto attraverso una socket TCP.
		* \param	sock socket su cui l'oggetto deve essere inviato
		* \details	Dopo aver controllato che l'oggetto sia pronto per essere inviato (vedi {@link isReady()}) e la socket non punti a NULL
		*			o sia chiusa, connette i segnali e gli slot per l'invio del pacchetto.
		*/
					void		sendThrough(QTcpSocket * sock);								// LANCIA ECCEZIONE
		/*!
		* \brief	Riceve l'oggetto attraverso una socket TCP.
		* \param	sock socket da cui l'oggetto deve essere ricevuto.
		* \details	Dopo aver chiamato {@link clear()} controlla lo stato della socket e connette i segnali per la ricezione.
		*/
					void		receiveFrom(QTcpSocket * sock);								// LANCIA ECCEZIONE
		/*!
		* \brief	Setta la dimensione dei pacchetti.
		* \param	payload vedi {@link Payload}
		*/
					void		setBytesPerPacket(Payload payload = Payload_512KB);			// LANCIA ECCEZIONE
		/*!
		* \return	Il numero di bytes per pacchetto.
		*/
					qint64		getBytesPerPacket() const;
	signals:
		/*!
		* \brief	Emesso quando l'oggetto è stato inviato.
		*/
					void		objectSended();
		/*!
		* \brief	Emesso quando l'oggetto è stato ricevuto.
		*/
					void		objectReceived();
		/*!
		* \brief	Indica che il blocco di ricezione è cambiato.
		* \param	block blocco corrente.
		*/
					void		changeBlock(qint64 block);
	public slots:

	protected slots:
		/*!
		 * \brief	Invia l'oggetto, da implementare nelle classi che ereditano.
		 * \details	Questo slot è chiamato automaticamente da {@link sendThrough()}, implementandolo è possibile
		 *			inviare l'oggetto sulla socket.
		 * \param	block blocco corrente di trasmissione.
		 */
		virtual		void		send(const qint64 block = 0) = 0;
		/*!
		 * \brief	Riceve l'oggetto, da implementare nelle classi che ereditano.
		 * \details	Questo slot è chiamato automaticamente da {@link receiveFrom()}, implementandolo è possibile
		 *			ricevere l'oggetto da una socket.
		 */
		virtual		void		recv() = 0;
		/*!
		 * \brief	Stabilisce come devono essere gestiti i segnali della socket in scrittura.
		 * \param	dim numero di bytes scritti sulla socket.
		 * \details	Tramite questo slot è possibile tenere il conto di quanti bytes sono stati realmente scritti sulla socket.
		 *			QTcpSocket offre il segnale <a href="http://doc.qt.io/qt-5/qiodevice.html#bytesWritten">bytesWritten(qint64)</a>
		 *			per cui tramite questo slot è possibile decrementare il numero dei bytes da scrivere (vedi {@link m_bytesCounter})
		 *			di modo che, quando tutto il pacchetto è stato inviato, si può inviare un nuovo pacchetto tramite {@link changeBlock()}.
		 */
		virtual		void		behaviorOnSend(const qint64 dim) = 0;						// LANCIA ECCEZIONE
		/*!
		 * \brief	Lascia la socket.
		 * \details	Chiamato di automaticamente tramite {@link objectSended()} ed {@link objectReceived()}.
		 */
		virtual		void		leaveSocket();


	protected:
		/*!
		 * \brief	Setta l'oggetto come pronto per la rete.
		 */
					void		setReady();
		/*!
		 * \brief	Setta l'oggetto come non pronto per la rete.
		 */
					void		setNotReady();
		/*!
		 * \brief	Controlla che l'oggetto sia pronto per la rete.
		 * \return	true se l'oggetto è pronto false altrimenti.
		 */
					bool		isReady() const;
		/*!
		 * \brief	Calcola la dimensione dell'oggetto sulla rete.
		 * \return	Il numero di bytes che devono essere traferiti.
		 * \details	Trattandosi di una funzione virtuale pura deve essere implementata nelle sottoclassi.
		 */
		virtual		qint64		calculateNetworkSize() = 0;

					bool		m_readyFlag;		/**< Flag che memorizza lo stato dell'oggetto.*/
					qint64		m_bytesPerPacket;	/**< Numero di bytes per pacchetto.*/
					qint64		m_bytesCounter;		/**< Numero di bytes ricevuti o inviati (varia a seconda del contesto).*/
					qint64		m_packets;			/**< Numero totale di pacchetti interi.*/
					qint64		m_spareBytes;		/**< Numero di bytes rimaenti.*/
					qint64		m_currentBlock;		/**< Blocco di ricezione o invio corrente (varia a seconda del contesto).*/
					QTcpSocket *m_channel;			/**< Puntatore alla socket.*/
	};
}

#endif // NETOBJECT_H
