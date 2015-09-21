#include <QCoreApplication>
#include <QDebug>
#include "DatabaseManager.h"

using namespace KCloud;

int main(int argc, char *argv[]){

	QCoreApplication a(argc, argv);

	DatabaseManager db(&a);

	if(db.open()){
		qDebug() << "OK!";
	}else{
		qDebug() << "Errori:";
		qDebug() << db.lastDriverError();
		qDebug() << db.lastSqlError();
	}

	return a.exec();
}

