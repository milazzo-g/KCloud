#include "Client.h"
#include <QDebug>
KCloud::Client::Client(const KCloud::Client::WorkMode mode, QObject *parent) : Engine(parent){

	if(mode == AsConsoleThread){

		m_coreApplication	= QCoreApplication::instance();
		m_console			= new Console(this);

		m_coreApplication->setApplicationName("KCloudClient");
		m_coreApplication->setApplicationVersion("1.0");
		m_coreApplication->setOrganizationName("Unikore");
		m_coreApplication->setOrganizationDomain("www.unikore.it");

		connect(m_console, SIGNAL(input(QString)), this		, SLOT(execCommand(QString)));
		connect(m_console, SIGNAL(finished()	), m_console, SLOT(deleteLater())		);

		m_console->start();
	}
	connect(m_packet, SIGNAL(objectReceived()), this, SLOT(parse()));
}

KCloud::Client::~Client(){

	qDebug() << "Client Stopped!";
}

void KCloud::Client::parse() throw (KCloud::Exception){

	qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << "RISPOSTA DEL SERVER ENUM = ->>>>>>>>>>" << m_packet->getServerAnswer();

	switch (m_lastCommand){

		case CommandPacket::Login:

			switch (m_packet->getServerAnswer()){

				case CommandPacket::LoginOk:
					clog("Login OK!");
					break;

				case CommandPacket::AlreadyLogged:
					clog(QString("Errore nel login, utente già loggato"));
					break;

				case CommandPacket::WrongEmail:
					clog("Errore nel login, email non valida");
					break;

				case CommandPacket::WrongPassword:
					clog("Errore nel login, password non valida");
					break;

				default:
					throw CorruptPacketException();
					break;

			}
			break;

		case CommandPacket::Logout:

			switch (m_packet->getServerAnswer()){
				case CommandPacket::LogoutOk:
					clog("Logout ok");
					m_console->quit();
					m_coreApplication->quit();
					break;

				case CommandPacket::LogoutFail:
					clog("Errore nel logout");
					break;

				default:
					throw CorruptPacketException();
					break;
			}
			break;

		default:
			clog("Processamento comandi non ancora implementato per questi valori!");
			break;
	}
}

void KCloud::Client::login() throw (KCloud::Exception){

	if(m_user == NULL){
		throw NullUserPointer();
	}else{
		m_packet->setForLogin(*m_user);
		m_lastCommand = m_packet->getClientCommand();
		sendCommand();
		connect(m_packet, SIGNAL(objectSended()), this, SLOT(receiveCommand()), Qt::UniqueConnection);
	}
}

void KCloud::Client::logout(){

	m_packet->setForLogout();
	m_lastCommand = m_packet->getClientCommand();
	sendCommand();
	connect(m_packet, SIGNAL(objectSended()), this, SLOT(receiveCommand()), Qt::UniqueConnection);
}

void KCloud::Client::resourceUp(){

}

void KCloud::Client::resourceMod(){

}

void KCloud::Client::resourceDel(){

}

void KCloud::Client::resourceTree(){

}

void KCloud::Client::resourceDown(){

}

void KCloud::Client::userRegister(){

}

void KCloud::Client::resourcePerm(){

}

void KCloud::Client::resourceShare(){

}

void KCloud::Client::passwordChange(){

}

void KCloud::Client::setUserForLogin(const QString &email, const QString &pwd) throw (KCloud::Exception){

	m_user = new User(email, pwd, User::Encrypt, this);
}

void KCloud::Client::connectToHost(const QString &addr, const quint16 &port) throw (KCloud::Exception){

	m_socket->connectToHost(addr, port);
	if(!(m_socket->waitForConnected())){
		throw UnreachableServer();
	}
}

void KCloud::Client::execCommand(const QString &cmd){

	QStringList arg = cmd.split(" ", QString::SkipEmptyParts, Qt::CaseInsensitive);

	switch(arg.size()){
		case 1:
			if(QRegExp("quit", Qt::CaseInsensitive, QRegExp::RegExp).exactMatch(arg[0])){

				clog("Stopped!");
				m_console->quit();
				m_coreApplication->quit();
			}else if(QRegExp("connect", Qt::CaseInsensitive, QRegExp::RegExp).exactMatch(arg[0])){

				clog("Mi sto collegando!");
				connectToHost("192.168.1.3", QString("8000").toUShort());
			}else if(QRegExp("login", Qt::CaseInsensitive, QRegExp::RegExp).exactMatch(arg[0])){

				clog("Faccio il login!");
				setUserForLogin("dio@fungo.it", "giallettiDiPaceco");
				login();
			}else if(QRegExp("logout", Qt::CaseInsensitive, QRegExp::RegExp).exactMatch(arg[0])){

				clog("Faccio il logout!");
				logout();
			}else{

				clog("Unknown Command!");
			}
			break;
		case 2:
			if(QRegExp("setZipDir", Qt::CaseInsensitive, QRegExp::RegExp).exactMatch(arg[0])){

				clog(QString("Setting zip dir to: " + arg[1]));
				try{
					m_resource->setZipDir(arg[1]);
				}catch(Exception &e){
					clog("Exception occurred!");
					clog(e.what());
					m_resource->clear();
				}
			}else if(QRegExp("setZipName", Qt::CaseInsensitive, QRegExp::RegExp).exactMatch(arg[0])){

				clog(QString("Setting zip name to: ") + arg[1]);


			}else{

			clog("Unknown Command!");
			}
			break;
		case 3:
			if(QRegExp("login", Qt::CaseInsensitive).exactMatch(arg[0])){
				try{
					setUserForLogin(arg[1], arg[2]);
					login();
				}catch(Exception &e){
					clog("Exception occurred!");
					clog(e.what());
				}
			}
			if(QRegExp("connect", Qt::CaseInsensitive).exactMatch(arg[0])){
				bool exc = false;
				try{
					connectToHost(arg[1], QString(arg[2]).toUShort());
				}catch(Exception &e){
					exc = true;
					clog("Exception occurred!");
					clog(e.what());
				}
				if(exc){
					clog("Connected!");
				}
			}

		default:
			break;
	}
}

void KCloud::Client::clog(const QString &log){

	QString str(Console::Green + this->metaObject()->className() + Console::Reset);
	str += " ";
	str += log;
	m_console->output(log);
}
