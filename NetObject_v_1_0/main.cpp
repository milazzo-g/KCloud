#include <QCoreApplication>
#include <QDir>
#include <QTimer>
#include "Resource.h"
#include "clientserver.h"

using namespace KCloud;

//#define CLIENT

int main(int argc, char *argv[]){
    QCoreApplication a(argc, argv);

	#ifdef CLIENT
		Client cl("192.168.1.100", 5000);
		cl.start();
		QTimer::singleShot(1000, &cl, SLOT(init()));
		QTimer::singleShot(3000, &cl, SLOT(sendData()));
	#else
		MainServer * serv = new MainServer(&a);
		serv->listen(QHostAddress::Any, 1995);
	#endif
	return a.exec();
}

