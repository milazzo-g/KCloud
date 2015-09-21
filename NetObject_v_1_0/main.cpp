#include <QCoreApplication>
#include <QDir>
#include "Resource.h"
#include "clientserver.h"

using namespace KCloud;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

	MainServer * serv = new MainServer(&a);

	serv->listen(QHostAddress::Any, 5000);

	return a.exec();
}

