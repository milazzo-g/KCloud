#include <QCoreApplication>
#include "netobject.h"
#include "client.h"
#include <QTimer>
#include <QTextStream>
#include "rootserver.h"
using namespace KCloud;

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	RootServer * server = new RootServer(&a);

	server->listen(QHostAddress::Any, 5000);

//	ClientServerApplication * s = new Server(&a);
//	s->start();
//	QTimer::singleShot(1000, s, SLOT(work()));

	return a.exec();
}

