#include "Client.h"
#include <QDebug>

#define TEST_USER		"test@test.it"
#define TEST_PASSWORD	"test"
#define TEST_HOST		"192.168.1.12"
#define TEST_PORT		8000

KCloud::Client::Client(const KCloud::Client::WorkMode mode, QObject *parent) : Engine(parent){ ////OK

	m_workMode = mode;
	if(m_workMode == AsConsoleThread){

		m_coreApplication	= QCoreApplication::instance();
		m_console			= new Console(this);
		m_coreApplication->setApplicationName("KCloudClient");
		m_coreApplication->setApplicationVersion("1.0");
		m_coreApplication->setOrganizationName("Unikore");
		m_coreApplication->setOrganizationDomain("www.unikore.it");

		connect(m_console,	SIGNAL(input(QString)),		this,			SLOT(execCommand(QString)), Qt::UniqueConnection);
		connect(m_console,  SIGNAL(finished()),			m_console,		SLOT(deleteLater()),		Qt::UniqueConnection);
		m_console->start();
	}

	connect(m_packet,	SIGNAL(objectSended()),			this,			SLOT(receiveCommand()),			Qt::UniqueConnection);
	connect(m_packet,	SIGNAL(objectReceived()),		this,			SLOT(parse()),					Qt::UniqueConnection);
	connect(m_resource, SIGNAL(objectReceived()),		this,			SLOT(finalizeResource()),		Qt::UniqueConnection);
	connect(m_resource, SIGNAL(objectSended()),			this,			SLOT(receiveCommand()),			Qt::UniqueConnection);
	connect(m_resource, SIGNAL(objectReceived()),		this,			SLOT(notifyReceived()),			Qt::UniqueConnection);
	connect(m_resource, SIGNAL(objectSended()),			this,			SLOT(notifySended()),			Qt::UniqueConnection);
	connect(m_resource, SIGNAL(transmissionRate(qint64, qint64, Resource::Mode)),
			this,		SLOT(notifyTransmissionRate(qint64,qint64,Resource::Mode)));
}

KCloud::Client::~Client(){

	qDebug() << "Client Stopped!";
}

void KCloud::Client::parse() throw (KCloud::Exception){

	try{
		CommandPacket::ServerAnswer r = m_packet->getServerAnswer();

		if(r == CommandPacket::NotLoggedUser){
			throw NotLogged();
		}else if(r == CommandPacket::ServerInternalError){
			clog("Errore interno al server");
			m_errors << m_packet->getLastError();
			emit serverAnswer(r);
			return;
		}
		switch (m_lastCommand){

			case CommandPacket::Login:

				switch (r){

					case CommandPacket::LoginOk:
						clog("Login OK!");
						setSessionUser();
						emit serverAnswer(r);
						break;

					case CommandPacket::AlreadyLogged:
						clog(QString("Errore nel login, utente già loggato"));
						emit serverAnswer(r);
						break;

					case CommandPacket::WrongEmail:
						clog("Errore nel login, email non valida");
						emit serverAnswer(r);
						break;

					case CommandPacket::WrongPassword:
						clog("Errore nel login, password non valida");
						emit serverAnswer(r);
						break;

					default:
						throw CorruptPacketException();
						break;

					}
					break;

			case CommandPacket::Logout:

					switch (r){
						case CommandPacket::LogoutOk:
							clog("Logout ok");
							emit serverAnswer(r);
							break;

						case CommandPacket::LogoutFail:
							clog("Errore nel logout");
							emit serverAnswer(r);
							break;

						default:
							throw CorruptPacketException();
							break;
					}
					break;

			case CommandPacket::ResourceUp:

					switch (r){
						case CommandPacket::ResourceUpOk:
							clog(QString("Caricamento consentito"));
							emit serverAnswer(r);
							resourceUp();
							break;

						case CommandPacket::ResourceUpFail:
							clog(QString("Caricamento rifiutato"));
							emit serverAnswer(r);
							break;

						case CommandPacket::ResourceUpInvalidPerm:
							clog(QString("Caricamento rifiutato, permessi insufficienti"));
							emit serverAnswer(r);
							break;

						case CommandPacket::ResourceUpParentIsNotDir:
							clog(QString("Caricamento rifiutato, il parent id è un file!"));
							emit serverAnswer(r);
							break;

						case CommandPacket::ResourceUpAlreadyExists:
							clog(QString("Caricamento rifiutato, presente file con lo stesso nome!"));
							emit serverAnswer(r);
							break;

						case CommandPacket::ResourceUpSpaceExhausted:
							clog(QString("Caricamento rifiutato, spazio sul server esaurito"));
							emit serverAnswer(r);
							break;

						case CommandPacket::ResourceUpFinalizeOk:
							clog(QString("Caricamento completato con successo"));
							removeTempFile();
							emit serverAnswer(r);
							break;

						case CommandPacket::ResourceUpFinalizeFail:
							clog(QString("Caricamento non finalizzato, per i seguenti utenti"));
							foreach (QString err, m_packet->getLastError()){
								m_errors << clog(err);
							}
							removeTempFile();
							emit serverAnswer(r);
							break;

						default:
							throw CorruptPacketException();
							break;
					}
					break;

			case CommandPacket::ResourceDown:

					switch (r){
						case CommandPacket::ResourceDownOk:
							clog("Scaricamento consentito");
							emit serverAnswer(r);
							resourceDown();
							break;

						case CommandPacket::ResourceDownInvalidId:
							clog("Scaricamento Fallito, Id non valido");
							emit serverAnswer(r);
							break;

						case CommandPacket::ResourceDownInvalidPerm:
							clog("Scaricamento Fallito, permessi insufficienti");
							emit serverAnswer(r);
							break;

						case CommandPacket::ResourceDownFail:
							clog("Scaricamento fallito");
							emit serverAnswer(r);
							break;

						default:
							throw CorruptPacketException();
							break;
					}
					break;

			case CommandPacket::ResourceTree:

					switch (r){

						case CommandPacket::ResourceTreeOk:
							clog("Ricevuto albero delle risorse");
							saveResourcesTree();
							emit serverAnswer(r);
							break;

						case CommandPacket::ResourceTreeError:
							clog("Errore nella richiesta dell'albero delle risorse");
							emit serverAnswer(r);
							break;

						default:
							throw CorruptPacketException();
							break;
					}
					break;

			case CommandPacket::ResourceDel:

					switch (r) {
						case CommandPacket::ResourceDelOk:
							clog("Cancellazione avvenuta con successo");
							emit serverAnswer(r);
							break;

						case CommandPacket::ResourceDelInvalidPerm:
							clog("Cancellazione non riuscita, permessi insufficienti!");
							emit serverAnswer(r);
							break;

						case CommandPacket::ResourceDelFail:
							clog("Cancellazione non riuscita!");
							emit serverAnswer(r);
							break;

						default:
							throw CorruptPacketException();
							break;
					}
					break;

			case CommandPacket::UserRegister:

					switch (r){
						case CommandPacket::UserRegisterOk:
							clog("Registrazione avvenuta con successo");
							emit serverAnswer(r);
							break;

						case CommandPacket::UsernameAlreadyInUse:
							clog("Registrazione fallita, username già in uso!");
							emit serverAnswer(r);
							break;

						case CommandPacket::UserRegisterFail:
							clog("Registrazione fallita:");
							emit serverAnswer(r);
							break;

						default:
							throw CorruptPacketException();
							break;
					}
					break;

			default:
				qDebug() << "Non ancora implementate!";
				break;
		}
	}catch(Exception &e){
		m_errors << clog(e.what());
		emit clientError(e.type());
	}
}

void KCloud::Client::login(const QString &mail, const QString &pass, const KCloud::User::PwdMode mode){ ///ok

	try{
		m_user = new User(mail, pass, mode, this);
		m_packet->setForLogin(*m_user);
		m_lastCommand = m_packet->getClientCommand();
		sendCommand();
	}catch(Exception &e){
		m_errors << clog(e.what());
		emit clientError(e.type());
	}
}

void KCloud::Client::logout(){	///ok

	m_packet->setForLogout();
	m_lastCommand = m_packet->getClientCommand();
	sendCommand();
}

void KCloud::Client::resourceUp(){	///ok

	sendResource();
}

void KCloud::Client::resourceMod(){

}

void KCloud::Client::resourceDel(const quint64 &resourceId){	///ok

	try{
		if(isLogged()){
			m_packet->setForResourceDel(resourceId);
			m_lastCommand = m_packet->getClientCommand();
			sendCommand();
		}else{
			throw NotLogged();
		}
	}catch(Exception &e){
		m_errors << clog(e.what());
		emit clientError(e.type());
	}
}

void KCloud::Client::resourceTree(){	///ok

	try{
		if(isLogged()){
			m_packet->setForResourceTree();
			m_lastCommand = m_packet->getClientCommand();
			sendCommand();
		}else{
			throw NotLogged();
		}
	}catch(Exception &e){
		m_errors << clog(e.what());
		emit clientError(e.type());
	}
}

void KCloud::Client::resourceDown(){	/////////Da riguardare

	m_head = m_packet->getFirstResourceHeader();
	m_resource->setZipName(m_head.getName());
	receiveResource();
}

void KCloud::Client::userRegister(const QString &email, const QString &password, const User::PwdMode mode){ ///ok

	try{
		User tmp(email, password, mode);
		m_packet->setForUserRegister(tmp);
		m_lastCommand = m_packet->getClientCommand();
		sendCommand();
	}catch(Exception &e){
		m_errors << clog(e.what());
		emit clientError(e.type());
	}
}

void KCloud::Client::resourcePerm(){

}

void KCloud::Client::resourceShare(){

}

void KCloud::Client::passwordChange(){

}

void KCloud::Client::disconnectFromHost(){
	m_socket->disconnectFromHost();
}

QStringList KCloud::Client::lastErrors(){

	QStringList err = m_errors;
	m_errors.clear();
	return err;
}

void KCloud::Client::connectToHost(const QString &addr, const quint16 &port){

	m_socket->connectToHost(addr, port);
}

void KCloud::Client::newUpload(const QString &localPath,
							   KCloud::User *sessionUser,
							   const quint64 &parentId,
							   const QMap<QString, KCloud::ResourceHeader::ResourcePerm> &permissionTable,
							   KCloud::ResourceHeader::ResourcePerm publicPerm) throw (Exception){

	if(!isLogged()){

		throw NotLogged();
	}

	try{
		QSettings appSettings;
		m_packet->setForResourceUp(localPath, User(*sessionUser), parentId, permissionTable, publicPerm);
		m_resource->setResourcePath(localPath);
		m_resource->setZipDir(appSettings.value(TMPF_PATH).toString());
		m_resource->setZipName(m_packet->getFirstResourceHeader().getName());
		m_lastCommand = m_packet->getClientCommand();
		sendCommand();
	}catch(Exception &e){
		clog("Exception occurred");
		clog(e.what());
		m_packet->clear();
	}
}

void KCloud::Client::newDownload(const quint64 &resourceId, const QString &savePath){ ///ok

	QString path;
	try{
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
	}catch(Exception &e){
		m_errors << clog(e.what());
		emit clientError(e.type());
	}
}

QList<KCloud::ResourceHeader> KCloud::Client::getResourceList() const{

	return m_resourcesTree;
}

KCloud::User *KCloud::Client::getSessionUser() const{

	return m_user;
}



void KCloud::Client::setSessionUser(){

	if(m_user){

		*m_user = m_packet->getUser();
	}else{

		throw NullUserPointer();
	}
}

void KCloud::Client::saveResourcesTree(){

	m_resourcesTree.clear();
	m_resourcesTree = m_packet->getResourceTree();

	foreach (ResourceHeader gesu, m_resourcesTree){

		clog(gesu.toString());
	}
	clog(QString::number(m_resourcesTree.size()));
}

void KCloud::Client::removeTempFile() throw (Exception){

	if(!m_resource->removeZipFile()){

		throw RemoveTempFileException();
	}
	m_resource->clear();
}

void KCloud::Client::finalizeResource() throw (Exception){

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

	if(m_workMode == AsGuiThread){
		exec();
	}

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
	exec();
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

				bool flag = false;
				try{
					connectToHost(TEST_HOST, TEST_PORT);
				}catch(Exception &e){
					flag = true;
					clog("Exception occurred!");
					clog(e.what());
				}
				if(!flag){
					clog("Connesso al server");
				}
			}else if(QRegExp("login", Qt::CaseInsensitive, QRegExp::RegExp).exactMatch(arg[0])){

				clog("Faccio il login con user_test!");
				try{
					login(TEST_USER, TEST_PASSWORD, User::Encrypt);
				}catch(Exception &e){
					clog("Exception occurred!");
					clog(e.what());
				}
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
				try{
					newDownload(arg[1].toULongLong());
				}catch(Exception &e){
					clog("Exception occurred!");
					clog(e.what());
				}

			}else if(QRegExp("remove", Qt::CaseInsensitive, QRegExp::RegExp).exactMatch(arg[0])){

				clog(QString("Rimozione risorsa con id = " + arg[1]));
				try{
					resourceDel(arg[1].toULongLong());
				}catch(Exception &e){
					clog("Exception occurred!");
					clog(e.what());
				}

			}else{

				clog("Unknown Command!");
			}
			break;
		case 3:
			if(QRegExp("login", Qt::CaseInsensitive).exactMatch(arg[0])){

				clog(QString("Faccio il login"));
				try{
					login(arg[1], arg[2], User::Encrypt);
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

				clog(QString("Upload file = " + arg[1] + ",	 id parent = " + arg[2]));

				//per avere condivisione
//				QMap<QString, ResourceHeader::ResourcePerm> map;
//				map.insert("milazzo.ga@gmail.com", ResourceHeader::Write);
//				map.insert("gesu@bambino.it", ResourceHeader::Read);
//				newUpload(arg[1], m_user, arg[2].toULongLong(), map, ResourceHeader::Read);

				//normalmente
				try{
					newUpload(arg[1], m_user, arg[2].toULongLong());
				}catch(Exception &e){
					clog("Exception occurred!");
					clog(e.what());
				}
			}else if(QRegExp("download", Qt::CaseInsensitive, QRegExp::RegExp).exactMatch(arg[0])){

				clog(QString("Download file con id: " + arg[1] + ", in dir: " + arg[2]));
				try{
					newDownload(arg[1].toULongLong(), arg[2]);
				}catch(Exception &e){
					clog("Exception occurred!");
					clog(e.what());
				}
			}else if(QRegExp("register", Qt::CaseInsensitive, QRegExp::RegExp).exactMatch(arg[0])){

				clog(QString("Richiesta registrazione utente, email = " + arg[1] + ", password = " + arg[2] ));
				try{
					userRegister(arg[1], arg[2], User::Encrypt);
				}catch(Exception &e){
					clog("Exception occurred!");
					clog(e.what());
				}
			}else{

				clog("Unknown Command!");
			}

		default:
			break;
	}
}

QString KCloud::Client::clog(const QString &log){

	if(m_workMode == AsConsoleThread){
		QString str(Console::Green + this->metaObject()->className() + Console::Reset);
		str += " ";
		str += log;
		m_console->output(str);
	}
	return log;
}

bool KCloud::Client::isLogged() throw(Exception){

	if(m_user){

		return m_user->isLogged();
	}else{

		throw NullUserPointer();
	}
}

void KCloud::Client::notifyReceived(){

	emit resourceReceived();
}

void KCloud::Client::notifySended(){

	emit resourceSended();
}

void KCloud::Client::notifyTransmissionRate(const qint64 total, const qint64 transmitted, KCloud::Resource::Mode mod){

	emit transmissionRate(total, transmitted, mod);
}
