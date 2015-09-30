#include "Client.h"
#include <QDebug>

#define TEST_USER		"test@test.it"
#define TEST_PASSWORD	"test"
#define TEST_HOST		"192.168.1.12"
#define TEST_PORT		8000

const QString	KCloud::Client::MSG_00("Risposta non settata."																						);
const QString	KCloud::Client::MSG_01("Non sei loggato."																														);
const QString	KCloud::Client::MSG_02("Login effettuato con successo"																											);
const QString	KCloud::Client::MSG_03("L'email che hai inserito non esiste."																									);
const QString	KCloud::Client::MSG_04("La password è errata"																													);
const QString	KCloud::Client::MSG_05("Risulti già essere loggato"																												);
const QString	KCloud::Client::MSG_06("Logout effettuato con successo"																											);
const QString	KCloud::Client::MSG_07("Logout fallito, verrai disconnesso automaticamente."																					);
const QString	KCloud::Client::MSG_08("Albero delle risorse ricevuto."																											);
const QString	KCloud::Client::MSG_09("L'albero delle risorse è vuoto."																										);
const QString	KCloud::Client::MSG_10("Upload consentito, inizio la compressione."																								);
const QString	KCloud::Client::MSG_11("La cartella è stata eliminata, non posso caricare."																						);
const QString	KCloud::Client::MSG_12("Non hai i permessi per scrivere in questa cartella."																					);
const QString	KCloud::Client::MSG_13("Caricando questa risorsa il tuo spazio occupato sarebbe maggiore di 4GB, rimuovi risorse per liberarlo."								);
const QString	KCloud::Client::MSG_14("Non puoi caricare su un file, seleziona una cartella."																					);
const QString	KCloud::Client::MSG_15("Esiste già una risorsa con lo stesso nome nella stessa cartella, rinominala e riprova a caricare."										);
const QString	KCloud::Client::MSG_16("Risorsa finalizzata con successo."																										);
const QString	KCloud::Client::MSG_17("Risorsa finalizzata con successo, tuttavia i seguenti utenti non sono stati trovati: "													);
const QString	KCloud::Client::MSG_18("Download consentito."																													);
const QString	KCloud::Client::MSG_19("Download fallito a causa di un errore interno al server, riprova più tardi."															);
const QString	KCloud::Client::MSG_20("La risorsa richiesta non è più disponibile."																							);
const QString	KCloud::Client::MSG_21("I tuoi permessi non sono sufficienti per scaricare questa risorsa, probabilmente sono stati modificati mentre effettuavo la richiesta."	);
const QString	KCloud::Client::MSG_22("Eliminazione effettuata con successo."																									);
const QString	KCloud::Client::MSG_23("Eliminazione fallita a causa di un errore interno al server, riprova più tardi."														);
const QString	KCloud::Client::MSG_24("I tuoi permessi non sono sufficienti per eliminare questa risorsa."																		);
const QString	KCloud::Client::MSG_25("Registrazione effettuata con successo."																									);
const QString	KCloud::Client::MSG_26("La registrazione è fallita poichè la tua email risulta già registrata."																	);
const QString	KCloud::Client::MSG_27("Risposta non settata."																													);
const QString	KCloud::Client::MSG_28("Password modificata."																													);
const QString	KCloud::Client::MSG_29("La password non può essere cambiata per un errore interno al server, riprova più tardi."												);
const QString	KCloud::Client::MSG_30("Risorsa modificata con successo."																										);
const QString	KCloud::Client::MSG_31("La risorsa non può essere modificata per un errore interno al server, riprova più tardi."												);
const QString	KCloud::Client::MSG_32("La risorsa non può essere modificata perchè non è più disponibile."																		);
const QString	KCloud::Client::MSG_33("I tuoi permessi non sono sufficienti per modificare questa risorsa."																	);
const QString	KCloud::Client::MSG_34("Risorsa condivisa con successo."																										);
const QString	KCloud::Client::MSG_35("I tuoi permessi non sono sufficienti per condividere questa risorsa."																	);
const QString	KCloud::Client::MSG_36("La risorsa non può essere condivisa perchè non è più disponibile."																		);
const QString	KCloud::Client::MSG_37("La risorsa non è stata condivisa con i seguenti utenti in quanto non registrati: "														);
const QString	KCloud::Client::MSG_38("La risorsa non può essere condivisa per un errore interno al server, riprova più tardi."												);
const QString	KCloud::Client::MSG_39("Errore interno al server, sicuramente causa di una ricorsione abbastanza lunga."														);
const QString	KCloud::Client::MSG_40("Generalmente non dovrei ricevere risposte di questo tipo, il codice della risposta è: "													);

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

	trace;
}

void KCloud::Client::parse() throw (KCloud::Exception){

	emit commandReceived();

	try{
		CommandPacket::ServerAnswer r = m_packet->getServerAnswer();

		if(r == CommandPacket::NotLoggedUser){
			throw NotLogged();
		}else if(r == CommandPacket::ServerInternalError){
			clog(MSG_39);
			m_errors << m_packet->getLastError();
			emit serverAnswer(r);
			return;
		}
		switch (m_lastCommand){

			case CommandPacket::Login:

				switch (r){

					case CommandPacket::LoginOk:
						clog(MSG_02);
						setSessionUser();
						emit serverAnswer(r);
						break;

					case CommandPacket::AlreadyLogged:
						clog(MSG_05);
						emit serverAnswer(r);
						break;

					case CommandPacket::WrongEmail:
						clog(MSG_03);
						emit serverAnswer(r);
						break;

					case CommandPacket::WrongPassword:
						clog(MSG_04);
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
							clog(MSG_06);
							emit serverAnswer(r);
							break;

						case CommandPacket::LogoutFail:
							clog(MSG_07);
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
							clog(MSG_10);
							emit serverAnswer(r);
							if(m_workMode == AsConsoleThread){
								resourceUp();
							}
							break;

						case CommandPacket::ResourceUpFail:
							clog(MSG_11);
							emit serverAnswer(r);
							break;

						case CommandPacket::ResourceUpInvalidPerm:
							clog(MSG_12);
							emit serverAnswer(r);
							break;

						case CommandPacket::ResourceUpParentIsNotDir:
							clog(MSG_14);
							emit serverAnswer(r);
							break;

						case CommandPacket::ResourceUpAlreadyExists:
							clog(MSG_15);
							emit serverAnswer(r);
							break;

						case CommandPacket::ResourceUpSpaceExhausted:
							clog(MSG_13);
							emit serverAnswer(r);
							break;

						case CommandPacket::ResourceUpFinalizeOk:
							clog(MSG_16);
							removeTempFile();
							emit serverAnswer(r);
							break;

						case CommandPacket::ResourceUpFinalizeFail:
							clog(MSG_17);
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
							clog(MSG_18);
							emit serverAnswer(r);
							resourceDown();
							break;

						case CommandPacket::ResourceDownInvalidId:
							clog(MSG_20);
							emit serverAnswer(r);
							break;

						case CommandPacket::ResourceDownInvalidPerm:
							clog(MSG_21);
							emit serverAnswer(r);
							break;

						case CommandPacket::ResourceDownFail:
							clog(MSG_19);
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
							clog(MSG_08);
							saveResourcesTree();
							emit serverAnswer(r);
							break;

						case CommandPacket::ResourceTreeError:
							clog(MSG_09);
							emit serverAnswer(r);
							break;

						default:
							throw CorruptPacketException();
							break;
					}
				break;

			case CommandPacket::ResourceDel:

				switch (r){
						case CommandPacket::ResourceDelOk:
							clog(MSG_22);
							emit serverAnswer(r);
							break;

						case CommandPacket::ResourceDelInvalidPerm:
							clog(MSG_24);
							emit serverAnswer(r);
							break;

						case CommandPacket::ResourceDelFail:
							clog(MSG_23);
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
							clog(MSG_25);
							emit serverAnswer(r);
							break;

						case CommandPacket::UsernameAlreadyInUse:
							clog(MSG_26);
							emit serverAnswer(r);
							break;

						case CommandPacket::UserRegisterFail:
							clog(MSG_27);
							emit serverAnswer(r);
							break;

						default:
							throw CorruptPacketException();
							break;
					}
				break;
			case CommandPacket::PasswordChange:

				switch (r){
					case CommandPacket::PasswordChangeOk:
						clog(MSG_28);
						emit serverAnswer(r);
						break;

					case CommandPacket::PasswordChangeFail:
						clog(MSG_29);
						emit serverAnswer(r);
						break;

					default:
						throw CorruptPacketException();
						break;
				}
				break;

			case CommandPacket::ResourceMod:

				switch (r){
					case CommandPacket::ResourceModOk:
						clog(MSG_30);
						emit serverAnswer(r);
						break;

					case CommandPacket::ResourceModInvalidId:
						clog(MSG_32);
						emit serverAnswer(r);
						break;
					case CommandPacket::ResourceModInvalidPerm:
						clog(MSG_33);
						emit serverAnswer(r);
						break;

					case CommandPacket::ResourceModFail:
						clog(MSG_31);
						emit serverAnswer(r);
						break;

					default:
						throw CorruptPacketException();
						break;
				}
				break;

			case CommandPacket::ResourceSharing:

				switch (r){
					case CommandPacket::ResourceSharingOk:
						clog(MSG_34);
						emit serverAnswer(r);
						break;

					case CommandPacket::ResourceSharingInvalidPerm:
						clog(MSG_35);
						emit serverAnswer(r);
						break;

					case CommandPacket::ResourceSharingInvalidId:
						clog(MSG_36);
						emit serverAnswer(r);
						break;

					case CommandPacket::ResourceSharingErrors:
						clog(MSG_37);
						foreach (QString err, m_packet->getLastError()){
							m_errors << clog(err);
						}
						emit serverAnswer(r);
						break;

					case CommandPacket::ResourceSharingFail:
						clog(MSG_38);
						emit serverAnswer(r);
						break;

					default:
						throw CorruptPacketException();
						break;
				}
				break;
			default:
				trace << MSG_40;
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

	emit compressionStart();
	sendResource();
}

void KCloud::Client::resourceMod(const KCloud::ResourceHeader &modifiedHead){		//nuovo arrivo

	try{
		if(isLogged()){
			m_packet->setForResourceMod(modifiedHead);
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

void KCloud::Client::resourceShare(const KCloud::ResourceHeader &head){

	try{
		if(isLogged()){
			m_packet->setForResourceSharing(head);
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

void KCloud::Client::passwordChange(const QString &oldHash, const QString &newHash){			//nuovo arrivo

	try{
		if(isLogged()){
			if(oldHash == m_user->getHash()){
				User tmp = *m_user;
				tmp.setHash(newHash, User::NotEncrypt);
				m_packet->setForPasswordChange(tmp);
				m_lastCommand = m_packet->getClientCommand();
				sendCommand();
			}else{
				throw OldPasswordNotCorresponding();
			}
		}else{
			throw NotLogged();
		}

	}catch(Exception &e){
		m_errors << clog(e.what());
		emit clientError(e.type());
	}
}

void KCloud::Client::disconnectFromHost(){
	m_socket->disconnectFromHost();
}

QStringList KCloud::Client::lastErrors(){

	clog(QString("NUMERO DI ERRORI : ") + QString::number(m_errors.size()));
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

	delete m_user;
	m_user = new User(m_packet->getUser());
	m_resourcesTree.clear();
	m_resourcesTree = m_packet->getResourceTree();
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
		emit decompressionStart();
		m_resource->decompress(m_head);
		emit decompressionEnd();
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

		QString str(Console::Green + this->metaObject()->className() + Console::Reset);
		str += " ";
		str += log;
	if(m_workMode == AsConsoleThread){
		m_console->output(str);
	}
	qDebug() << log;
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
