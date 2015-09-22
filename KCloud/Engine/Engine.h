#ifndef ENGINE_H
#define ENGINE_H

/*
 *
 * GIUSEPPE
 *
 */

#include "engine_global.h"
#include <QObject>
#include <QThread>
#include <QTcpSocket>

#include "../Resource/Resource.h"
#include "../CommandPacket/CommandPacket.h"

namespace KCloud{

	class ENGINESHARED_EXPORT Engine : public QThread{
		Q_OBJECT
		public:
			explicit						Engine(QObject *parent = 0);
											~Engine();

		signals:

		protected slots:
			virtual		void				parse() = 0;

		protected:
						QTcpSocket *		m_socket;
						Resource *			m_resource;
						CommandPacket *		m_packet;
	};
}

#endif // ENGINE_H
