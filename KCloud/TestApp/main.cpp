#include <QCoreApplication>
#include "../Client/Client.h"
#include "../MainServer/MainServer.h"

using namespace KCloud;

#define SERVER false
#define CLIENT true

int main(int argc, char *argv[]){

	QCoreApplication a(argc, argv);

	bool mode = CLIENT;

	if(mode == SERVER){
		MainServer *server	= new MainServer(&a);
		server->listen(QHostAddress::Any, 8000);
	}else{
		Client *	client = new Client(Client::AsConsoleThread, &a);
		client->start();
	}


	return a.exec();
}

