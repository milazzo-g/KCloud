#ifndef CLIENT_H
#define CLIENT_H

#include "client_global.h"

#include <QCoreApplication>

#include "../Engine/Engine.h"
#include "../Console/Console.h"

namespace KCloud{

	class CLIENTSHARED_EXPORT Client : public Engine{
		Q_OBJECT
		public:
			enum WorkMode{
				Thread,
				Standalone
			};
											Client(WorkMode mode, QObject *parent = 0);
		private:
						QCoreApplication *	m_coreApplication;
						Console	*			m_console;
						WorkMode			m_workMode;

	};
}

#endif // CLIENT_H
