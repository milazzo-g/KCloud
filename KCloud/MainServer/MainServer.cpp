#include "MainServer.h"
#include <QDebug>

KCloud::MainServer::MainServer(QObject *parent) : QTcpServer(parent){

	m_coreApplication	= QCoreApplication::instance();
	m_console			= new Console(this);

	m_coreApplication->setApplicationName("KCloudServer");
	m_coreApplication->setApplicationVersion("1.0");
	m_coreApplication->setOrganizationName("Unikore");
	m_coreApplication->setOrganizationDomain("www.unikore.it");

	connect(m_console, SIGNAL(input(QString)),	this		, SLOT(execCommand(QString)	));
	connect(m_console, SIGNAL(finished()	),	m_console	, SLOT(deleteLater()		));
	m_console->start();

	clog("Spawned!");
}

KCloud::MainServer::~MainServer(){
	qDebug() << "MainServer: Stopped!";
}

void KCloud::MainServer::execCommand(const QString &cmd){

	if(cmd == "quit"){
		foreach (WorkerServer * item, m_clientsHandlers) {
			if(item->isRunning()){
				item->quit();
			}
		}
		m_console->quit();
		m_coreApplication->quit();
	}else{
		clog("Unknown Command!");
	}
}

void KCloud::MainServer::clog(const QString &log){
	QString str(Console::Blue + this->metaObject()->className() + Console::Reset);
	str += " ";
	str += log;
	m_console->output(str);
}

void KCloud::MainServer::incomingConnection(qintptr handle){

	clog("New Connection Accepted!");
	clog("Creating New Worker Server...");
	WorkerServer * tmp = new WorkerServer(handle, this);
	clog("Worker Server Created!");
	connect(tmp, SIGNAL(consoleOutRequest(QString)	) , m_console	, SLOT(output(QString)	));
	connect(tmp, SIGNAL(finished()					) , tmp			, SLOT(deleteLater()	));
	m_clientsHandlers << tmp;
	tmp->start();
}
