#ifndef COMMANDPACKET_H
#define COMMANDPACKET_H
#include "NetObject.h"

namespace KCloud{

	class CommandPacket : public NetObject{
		Q_OBJECT

		public:
		explicit	CommandPacket(QObject *parent = 0);

					void		clear();
		virtual		void		prepareForSend() throw(Exception);
		virtual		void		prepareForRecv();

		protected slots:
		virtual		void		send(const qint64 block = 0);
		virtual		void		recv() throw(Exception);
		virtual		void		behaviorOnSend(const qint64 dim) throw(Exception);

		protected:
		virtual		qint64		calculateNetworkSize() throw(Exception);

		private:

	};
}

#endif // COMMANDPACKET_H
