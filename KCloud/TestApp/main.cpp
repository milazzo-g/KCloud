#include <QCoreApplication>

#include "../MainServer/MainServer.h"

using namespace KCloud;

int main(int argc, char *argv[]){

	QCoreApplication a(argc, argv);

	MainServer * serv = new MainServer(&a);

	return a.exec();
}

