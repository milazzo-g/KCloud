#include "MainServer.h"
#include "defines.h"
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
//	qDebug() << "MainServer: Stopped!";
}

void KCloud::MainServer::start(){
	QSettings appSettings;
	if(!appSettings.contains(T_STARTED)){
		appSettings.setValue(T_STARTED, 0);
	}else{
		int times = appSettings.value(T_STARTED).toInt();
		times++;
		appSettings.setValue(T_STARTED, times);
		clog(QString("Started ") + QString::number(appSettings.value(T_STARTED).toInt()) + QString(" times."));
	}
	if(appSettings.value(T_STARTED).toInt() == 0){
		QDir dir;
		clog(QString("First execution!"));

		clog(QString("creating ") + QString(ROOT_PATH));
		dir = dir.home();
		dir.mkdir(m_coreApplication->applicationName());
		dir.cd(m_coreApplication->applicationName());
		clog(QString(ROOT_PATH) + QString(" created in ") + dir.path());
		appSettings.setValue(ROOT_PATH, dir.path());

		clog(QString("creating ") + QString(INSTANCES));
		dir.mkdir("instances");
		dir.cd("instances");
		clog(QString(INSTANCES) + QString(" created in ") + dir.path());
		appSettings.setValue(INSTANCES, dir.path());

		dir.cdUp();
		clog(QString("creating ") + QString(RESOURCES));
		dir.mkdir("resources");
		dir.cd("resources");
		clog(QString(RESOURCES) + QString(" created in ") + dir.path());
		appSettings.setValue(RESOURCES, dir.path());
	}
	clog("enter 'start' to start Server...");

}

void KCloud::MainServer::execCommand(const QString &cmd){

	if(cmd == "quit"){
//		foreach (WorkerServer * item, m_clientsHandlers.values()) {
//			item->quit();
//		}
		m_console->quit();
		m_coreApplication->quit();
	}else if(cmd == "start"){
		clog("Start listening...");
		listen(QHostAddress::AnyIPv4, 8000);
	}else if(cmd == "threads"){
//		if(m_clientsHandlers.size() == 0){
//			clog("No active threads!");
//		}else{
//			clog(QString("Active threads: ") + QString::number(m_clientsHandlers.size()));
//			foreach (QString add, m_clientsHandlers.keys()) {
//				clog(add);
//			}
//		}
	}else{
		clog("Unknown Command!");
	}
}

//void KCloud::MainServer::removeHandler(const QString &add){

//	m_clientsHandlers.remove(add);
//}

void KCloud::MainServer::clog(const QString &log){
	QString str(Console::Blue + this->metaObject()->className() + Console::Reset);
	str += " ";
	str += log;
	m_console->output(str);
}

void KCloud::MainServer::incomingConnection(qintptr handle){

	clog("New Connection Accepted!");
	clog("Creating New Worker Server...");
	WorkerServer * tmp = new WorkerServer(handle);
//	qDebug() << "Socket Descriptor : " << handle;
	clog("Worker Server Created!");
//	connect(tmp, SIGNAL(consoleOutRequest(QString)			) , m_console	, SLOT(output(QString)			));
//	connect(tmp, SIGNAL(removeFromActiveHandlers(QString)	) ,	this		, SLOT(removeHandler(QString)	));
	connect(tmp, SIGNAL(finished()							) , tmp			, SLOT(deleteLater()			));
//	m_clientsHandlers.insert(tmp->address(), tmp);
	tmp->start();
}
