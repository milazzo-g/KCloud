#include <QCoreApplication>
#include "netobject.h"
#include "client.h"
#include <QTimer>
#include <QTextStream>

using namespace KCloud;

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	return a.exec();
}

