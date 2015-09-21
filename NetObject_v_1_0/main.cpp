#include <QCoreApplication>
#include <QDir>
#include <QTimer>
#include "Resource.h"
#include "clientserver.h"

using namespace KCloud;

int main(int argc, char *argv[]){
    QCoreApplication a(argc, argv);

	Client cl("192.168.1.100", 5000);
	cl.start();
	QTimer::singleShot(1000, &cl, SLOT(init()));
	QTimer::singleShot(3000, &cl, SLOT(sendData()));


	return a.exec();
}

