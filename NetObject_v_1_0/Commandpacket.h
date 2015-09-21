#ifndef COMMANDPACKET_H
#define COMMANDPACKET_H
#include "NetObject.h"

namespace KCloud{

	class CommandPacket : public NetObject{
		Q_OBJECT

		public:
		explicit	CommandPacket(QObject *parent = 0);

					void		clear();
		virtual		void		prepareForSend();										// LANCIA ECCEZIONE
		virtual		void		prepareForRecv();										// LANCIA ECCEZIONE

		protected slots:
		virtual		void		send(const qint64 block = 0);
		virtual		void		recv();
		virtual		void		behaviorOnSend(const qint64 dim);						// LANCIA ECCEZIONE

		protected:
		virtual		qint64		calculateNetworkSize();									// LANCIA ECCEZIONE

		private:

	};
}

#endif // COMMANDPACKET_H
