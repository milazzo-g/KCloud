#include "ClientServer.h"
#include <QDebug>
KCloud::MainServer::MainServer(QObject *parent) : QTcpServer(parent){
	m_coreApplication = QCoreApplication::instance();
	m_console = new Console(this);

	m_coreApplication->setApplicationName("KCloudServer");
	m_coreApplication->setApplicationVersion("1.0");
	m_coreApplication->setOrganizationName("Unikore");
	m_coreApplication->setOrganizationDomain("www.unikore.it");

	connect(m_console, SIGNAL(input(QString)), this, SLOT(execCommand(QString)));
	m_console->start();
	m_console->output("MainServer: Spawned!");
}

KCloud::MainServer::~MainServer(){
	qDebug() << "MainServer: Stopped!";
}

void KCloud::MainServer::execCommand(const QString &cmd){

	if(cmd == "quit"){
		m_console->quit();
		m_coreApplication->quit();
	}else{
		m_console->output("MainServer: Unknown Command!");
	}
}


