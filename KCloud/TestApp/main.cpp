#include <QCoreApplication>
#include "../Client/Client.h"
#include "../MainServer/MainServer.h"
#include "../UsersManager/UsersManager.h"

using namespace KCloud;

#define SERVER false
#define CLIENT true

/*
 *
 * Server
 *
 */

int main(int argc, char *argv[]){

	QCoreApplication a(argc, argv);

	bool mode = SERVER;

	if(mode == SERVER){
		MainServer *server	= new MainServer(&a);
		server->listen(QHostAddress::AnyIPv4, 8000);
	}else{
		Client *	client = new Client(Client::AsConsoleThread, &a);
		client->start();
	}

	return a.exec();
}

