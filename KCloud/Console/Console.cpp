#include "Console.h"
#include <QDebug>

const QString KCloud::Console::Red("\033[1;31m");
const QString KCloud::Console::Blue("\033[1;34m");
const QString KCloud::Console::Green("\033[1;32m");
const QString KCloud::Console::Cyan("\033[1;36m");
const QString KCloud::Console::Yellow("\033[1;33m");
const QString KCloud::Console::Reset("\033[0m");

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
