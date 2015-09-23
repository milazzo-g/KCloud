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

void KCloud::Client::parse(){

	switch (m_lastCommand) {

		case CommandPacket::Login:

			switch (m_packet->getServerAnswer()) {

				case CommandPacket::LoginOk:
					clog("Login OK!");
					break;

				case CommandPacket::AlreadyLogged:
					clog(QString("Errore nel login, il server riporta: ") + m_packet->getLastError());
					break;

				default:
					clog(QString("Errore pacchetto corrotto [enum non prevista]!"));
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

void KCloud::Client::execCommand(const QString &cmd){

	QStringList arg = cmd.split(" ", QString::SkipEmptyParts, Qt::CaseInsensitive);

	switch(arguments.size()){
		case 1:
			if(QRegExp("[quit]", Qt::CaseInsensitive, QRegExp::RegExp).exactMatch(arg[0])){
				clog("Stopped!");
				m_console->quit();
				m_coreApplication->quit();
			}else{
				clog("Unknown Command!");
			}
			break;
		case 3:
			if(QRegExp("[login]", Qt::CaseInsensitive).exactMatch(arg[0])){
				try{
					setUserForLogin(arg[1], arg[2]);
				}catch(Exception &e){
					clog("Exception occurred!");
					clog(e.what());
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
