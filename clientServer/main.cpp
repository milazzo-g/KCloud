#include <QCoreApplication>
#include <QTimer>
#include <QFile>
#include "ClientServer.h"

using namespace KCloud;

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	MainServer *serv = new MainServer(&a);

	return a.exec();
}

