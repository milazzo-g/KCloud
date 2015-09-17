#include <QCoreApplication>
#include "netobject.h"
#include "client.h"
#include <QTimer>
#include <QTextStream>

using namespace KCloud;

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	ClientServerApplication * server;
	ClientServerApplication * client;

	server = new Server(&a);
	server->start();

	client = new Client(&a);
	client->start();

	QTimer::singleShot(1000, server, SLOT(work()));
	QTimer::singleShot(4000, client, SLOT(work()));

	return a.exec();
}

