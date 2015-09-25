#include "Client.h"
#include <QDebug>

#define TEST_USER		"test@test.it"
#define TEST_PASSWORD	"test"
#define TEST_HOST		"192.168.1.12"
#define TEST_PORT		8000

KCloud::Client::Client(const KCloud::Client::WorkMode mode, QObject *parent) : Engine(parent){

	if(mode == AsConsoleThread){

		m_coreApplication	= QCoreApplication::instance();
		m_console			= new Console(this);
		m_loginState		= false;
		m_coreApplication->setApplicationName("KCloudClient");
		m_coreApplication->setApplicationVersion("1.0");
		m_coreApplication->setOrganizationName("Unikore");
		m_coreApplication->setOrganizationDomain("www.unikore.it");

		connect(m_console,	SIGNAL(input(QString)),		this,			SLOT(execCommand(QString)), Qt::UniqueConnection);
		connect(m_console,  SIGNAL(finished()),			m_console,		SLOT(deleteLater()),		Qt::UniqueConnection);
		connect(m_socket,	SIGNAL(disconnected()),		this,			SLOT(closeAll()),			Qt::UniqueConnection);

		m_console->start();
	}
	connect(m_packet, SIGNAL(objectReceived()), this, SLOT(parse()), Qt::UniqueConnection);
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
					setSessionUser();
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
					closeAll();
					break;

				case CommandPacket::LogoutFail:
					clog("Errore nel logout");
					break;

				default:
					throw CorruptPacketException();
					break;
			}
			break;
		case CommandPacket::ResourceUp:

			switch (m_packet->getServerAnswer()){
				case CommandPacket::ResourceUpOk:
					clog(QString("Caricamento consentito"));
					resourceUp();
					break;

				case CommandPacket::ResourceUpFail:
					clog(QString("Caricamento rifiutato"));
					break;

				case CommandPacket::ResourceUpInvalidPerm:
					clog(QString("Caricamento rifiutato, permessi insufficienti"));
					break;

				case CommandPacket::ResourceUpSpaceExhausted:
					clog(QString("Caricamento rifiutato, spazio sul server esaurito"));
					break;

				case CommandPacket::ResourceUpFinalizeOk:
					clog(QString("Caricamento completato con successo"));

					try{

						removeTempFile();
					}catch(Exception &e){

						clog("Exception occurred!");
						clog(e.what());
					}
					break;

				case CommandPacket::ResourceUpFinalizeFail:
					clog(QString("Caricamento non finalizzato, per i seguenti utenti"));

					foreach (QString err, m_packet->getLastError()){
						clog(err);
					}

					try{

						removeTempFile();
					}catch(Exception &e){

						clog("Exception occurred!");
						clog(e.what());
					}
					break;

				default:
					break;
			}
			break;

		case CommandPacket::ResourceDown:

			switch (m_packet->getServerAnswer()){
				case CommandPacket::ResourceDownOk:
					clog("Scaricamento consentito");
					resourceDown();
					break;

				case CommandPacket::ResourceDownInvalidId:
					clog("Scaricamento Fallito, Id non valido");
					break;

				case CommandPacket::ResourceDownFail:
					clog("Scaricamento fallito");
					break;

				default:
					throw CorruptPacketException();
					break;
			}
			break;

		case CommandPacket::ResourceTree:

			switch (m_packet->getServerAnswer()){

				case CommandPacket::ResourceTreeOk:
					clog("Ricevuto albero delle risorse");
					break;

				case CommandPacket::ResourceTreeError:
					clog("Errore nella richiesta dell'albero delle risorse");
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
		connect(m_packet, SIGNAL(objectSended()), this, SLOT(receiveCommand()), Qt::UniqueConnection);
		sendCommand();
	}
}

void KCloud::Client::logout(){

	m_packet->setForLogout();
	m_lastCommand = m_packet->getClientCommand();
	connect(m_packet, SIGNAL(objectSended()), this, SLOT(receiveCommand()), Qt::UniqueConnection);
	sendCommand();
}

void KCloud::Client::resourceUp(){

	connect(m_resource, SIGNAL(objectSended()), this, SLOT(receiveCommand()), Qt::UniqueConnection);
	sendResource();
}

void KCloud::Client::resourceMod(){

}

void KCloud::Client::resourceDel(){

}

void KCloud::Client::resourceTree() throw (KCloud::Exception){

	if(!isLogged()){
		throw NotLogged();
	}
	m_packet->setForResourceTree();
	m_lastCommand = m_packet->getClientCommand();
	connect(m_packet, SIGNAL(objectSended()), this, SLOT(receiveCommand()), Qt::UniqueConnection);
	sendCommand();
}

void KCloud::Client::resourceDown(){

	m_head = m_packet->getFirstResourceHeader();
	connect(m_resource, SIGNAL(objectReceived()), this, SLOT(finalizeResource()), Qt::UniqueConnection);
	receiveResource();
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

void KCloud::Client::newUpload(const QString &localPath,
							   KCloud::User *sessionUser,
							   const quint64 &parentId,
							   const QMap<QString, KCloud::ResourceHeader::ResourcePerm> &permissionTable,
							   KCloud::ResourceHeader::ResourcePerm publicPerm) throw (Exception){

	if(!isLogged()){

		throw NotLogged();
	}
	if(m_user == NULL){

		throw NullUserPointer();
	}else{

		try{
			QSettings appSettings;
			m_packet->setForResourceUp(localPath, User(*sessionUser), parentId, permissionTable, publicPerm);
			m_resource->setResourcePath(localPath);
			m_resource->setZipDir(appSettings.value(TMPF_PATH).toString());
			m_resource->setZipName(m_packet->getFirstResourceHeader().getName());
			m_lastCommand = m_packet->getClientCommand();
			sendCommand();
			connect(m_packet, SIGNAL(objectSended()), this, SLOT(receiveCommand()), Qt::UniqueConnection);
		}catch(Exception &e){
			clog("Exception occurred");
			clog(e.what());
			m_packet->clear();
		}
	}
}

void KCloud::Client::newDownload(const quint64 &resourceId, const QString &savePath) throw (Exception){

	QString path;

	if(!isLogged()){
		throw NotLogged();
	}
	if(savePath.isEmpty()){

		QSettings appSettings;
		path = appSettings.value(DOWN_PATH).toString();
	}else{

		if(QFileInfo(savePath).exists() && QFileInfo(savePath).isDir()){
			path = savePath;
		}else{
			throw BadPathException();
		}
	}
	m_packet->setForResourceDown(resourceId);
	m_resource->setZipDir(path);
	m_lastCommand = m_packet->getClientCommand();
	sendCommand();
	connect(m_packet, SIGNAL(objectSended()), this, SLOT(receiveCommand()), Qt::UniqueConnection);
}

void KCloud::Client::setSessionUser(){

	if(m_user){

		*m_user = m_packet->getUser();
		m_loginState = true;
	}else{

		throw NullUserPointer();
	}
}

void KCloud::Client::removeTempFile() throw (Exception){

	if(!m_resource->removeZipFile()){

		throw RemoveTempFileException();
	}
	m_resource->clear();
}

void KCloud::Client::finalizeResource(){

	m_resource->setZipName(m_head.getName());
	try{
		m_resource->decompress(m_head);
	}catch(Exception &e){
		clog("Exception occurred");
		clog(e.what());
	}

	clog("Download completato");
}

void KCloud::Client::run(){

	QSettings	appSettings;
	QDir		dir;
	if(!appSettings.contains(T_STARTED)){

		appSettings.setValue(T_STARTED, 0);
	}

	if(appSettings.value(T_STARTED).toLongLong() <= 0){

		dir = dir.home();
		dir.mkdir(m_coreApplication->applicationName());
		dir.cd(m_coreApplication->applicationName());
		appSettings.setValue(ROOT_PATH, dir.path());
		dir.mkdir(_DOWNLOAD);
		dir.cd(_DOWNLOAD);
		appSettings.setValue(DOWN_PATH, dir.path());
		dir.cdUp();
		dir.mkdir(_TEMPFILE);
		dir.cd(_TEMPFILE);
		appSettings.setValue(TMPF_PATH, dir.path());
	}

	qint64 i = appSettings.value(T_STARTED).toLongLong();
	appSettings.setValue(T_STARTED, i++);
}

void KCloud::Client::closeAll(){

	m_console->quit();
	m_coreApplication->quit();

}

void KCloud::Client::execCommand(const QString &cmd){

	QStringList arg = cmd.split(" ", QString::SkipEmptyParts, Qt::CaseInsensitive);

	switch(arg.size()){
		case 1:
			if(QRegExp("quit", Qt::CaseInsensitive, QRegExp::RegExp).exactMatch(arg[0])){

				clog("Stopped!");
				closeAll();
			}else if(QRegExp("connect", Qt::CaseInsensitive, QRegExp::RegExp).exactMatch(arg[0])){

				connectToHost(TEST_HOST, TEST_PORT);
				clog("Connesso al server");
			}else if(QRegExp("login", Qt::CaseInsensitive, QRegExp::RegExp).exactMatch(arg[0])){

				clog("Faccio il login con user_test!");
				setUserForLogin(TEST_USER, TEST_PASSWORD);
				login();
			}else if(QRegExp("logout", Qt::CaseInsensitive, QRegExp::RegExp).exactMatch(arg[0])){

				clog("Faccio il logout!");
				logout();
			}else if(QRegExp("resourceList", Qt::CaseInsensitive, QRegExp::RegExp).exactMatch(arg[0])){

				clog("Chiedo la lista delle risorse!");
				resourceTree();
			}else{

				clog("Unknown Command!");
			}
			break;
		case 2:
			if(QRegExp("download", Qt::CaseInsensitive, QRegExp::RegExp).exactMatch(arg[0])){

				clog(QString("Download file con id: " + arg[1]));
				newDownload(arg[1].toULongLong());
			}else if(QRegExp("remove", Qt::CaseInsensitive, QRegExp::RegExp).exactMatch(arg[0])){

				clog(QString("Rimozione risorsa con id = " + arg[1]));
				m_packet->setForResourceDel(arg[1].toULongLong());

			}else{

				clog("Unknown Command!");
			}
			break;
		case 3:
			if(QRegExp("login", Qt::CaseInsensitive).exactMatch(arg[0])){

				clog(QString("Faccio il login"));
				try{
					setUserForLogin(arg[1], arg[2]);
					login();
				}catch(Exception &e){
					clog("Exception occurred!");
					clog(e.what());
				}
			}else if(QRegExp("connect", Qt::CaseInsensitive).exactMatch(arg[0])){

				clog(QString("Mi sto collegando: host = " + arg[1] + ", port = " + arg[2]));
				try{
					connectToHost(arg[1], QString(arg[2]).toUShort());
				}catch(Exception &e){
					clog("Exception occurred!");
					clog(e.what());
				}
			}else if(QRegExp("upload", Qt::CaseInsensitive, QRegExp::RegExp).exactMatch(arg[0])){

				clog(QString("Upload file = " + arg[1] + ",	 id parent = " + arg[2].toULongLong()));

				//per avere condivisione
//				QMap<QString, ResourceHeader::ResourcePerm> map;
//				map.insert("milazzo.ga@gmail.com", ResourceHeader::Write);
//				map.insert("gesu@bambino.it", ResourceHeader::Read);
//				newUpload(arg[1], m_user, arg[2].toULongLong(), map, ResourceHeader::Read);

				//normalmente
				newUpload(arg[1], m_user, arg[2].toULongLong());
			}else if(QRegExp("download", Qt::CaseInsensitive, QRegExp::RegExp).exactMatch(arg[0])){

				clog(QString("Download file con id: " + arg[1]));
				newDownload(arg[1].toULongLong());
			}else{

				clog("Unknown Command!");
			}

		default:
			break;
	}
}

void KCloud::Client::clog(const QString &log){

	QString str(Console::Green + this->metaObject()->className() + Console::Reset);
	str += " ";
	str += log;
	m_console->output(str);
}

bool KCloud::Client::isLogged(){

	return m_loginState;
}
