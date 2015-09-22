#include "WorkerServer.h"


KCloud::WorkerServer::~WorkerServer()
{

}

void KCloud::WorkerServer::run(){
	clog("Spawned!");
	//se non dovesse funzionare è perchè l'event loop non è ancora partito (almeno penso)
	//quindi possiamo provare con QTimer
	receiveCommand();
	exec();
}

void KCloud::WorkerServer::clog(const QString &log){

	QString str(Console::Green + QString("WorkerThread ") + Console::Reset);
	str += log;
	emit consoleOutRequest(str);
}
