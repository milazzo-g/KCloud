#include "Console.h"
#include <QDebug>


KCloud::Console::Console(QObject *parent) : QThread(parent), m_notitier(fileno(stdin), QSocketNotifier::Read){

	connect(&m_notitier, SIGNAL(activated(int)), this, SLOT(processNewInput()));
}

KCloud::Console::~Console(){
	output("Console: Stopped!");
}

void KCloud::Console::output(const QString &str){
	QTextStream stream(stdout);
	stream << str << endl;
}

void KCloud::Console::processNewInput(){
	QTextStream stream(stdin);
	QString str = stream.readLine();
	emit input(str);
}
