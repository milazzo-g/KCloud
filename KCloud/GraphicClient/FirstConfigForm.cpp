#include "FirstConfigForm.h"
#include "ui_FirstConfigForm.h"



FirstConfigForm::FirstConfigForm(Client * client, QWidget *parent) : QDialog(parent), ui(new Ui::FirstConfigForm){

	QRegExp rx("^0*(?:6553[0-5]|655[0-2][0-9]|65[0-4][0-9]{2}|6[0-4][0-9]{3}|[1-5][0-9]{4}|[1-9][0-9]{1,3}|[0-9])$");

	ui->setupUi(this);
	this->setFixedSize(this->size());

	m_movie					= new QMovie(this);
	m_result				= false;
	m_tabWidget				= ui->tabWidget;
	m_goToConnection		= ui->goToConnectionButton;
	m_goToRegister			= ui->goToRegisterButton;
	m_goToPathConfig		= ui->goToPathConfig;
	m_goToEnd				= ui->goToEndButton;
	m_end					= ui->endButton;
	m_connectionLoader		= ui->connectionLoaderLabel;
	m_connectionMessage		= ui->connectionMessageLabel;
	m_registerLoader		= ui->registerLoaderLabel;
	m_registerMessage		= ui->registerMessageLabel;
	m_pathLoader			= ui->pathConfigLoaderLabel;
	m_pathMessage			= ui->pathConfigMessageLabel;
	m_confirmPasswordLabel	= ui->confirmPasswordLabel;
	m_confirmPasswordEdit	= ui->confirmPasswordEdit;
	m_alreadyRegistered		= ui->alreadyRegistered;
	m_workPath				= QDir::home().path();

	ui->serverPortEdit->setValidator(new QRegExpValidator(rx, this));

	m_goToConnection->setEnabled(false);
	m_goToRegister->setEnabled(false);
	m_goToPathConfig->setEnabled(false);
	m_goToEnd->setEnabled(false);
	m_end->setEnabled(false);

	for(int i = 1; i < m_tabWidget->count(); i++){
		m_tabWidget->setTabEnabled(i, false);
	}

	m_movie->setFileName(":/animations/loader.gif");

	m_address.clear();
	m_port = 0;
	m_client = client;

	connect(m_client,	SIGNAL(	serverAnswer(CommandPacket::ServerAnswer)),
			this	,	SLOT(	onServerAnswer(CommandPacket::ServerAnswer)));

	connect(m_client,	SIGNAL(	clientError(Exception::Type)),
			this	,	SLOT(	onClientError(Exception::Type)));

	connect(m_client,	SIGNAL(	connected()),
			this	,	SLOT(	onClientConnected()));

	connect(m_client,	SIGNAL(	disconnected()),
			this	,	SLOT(	onClientDisconnected()));

	connect(m_client,	SIGNAL(	socketError(QAbstractSocket::SocketError)),
			this	,	SLOT(	onClientSocketError(QAbstractSocket::SocketError)));

	QFile lgpl(this);
	lgpl.setFileName("./LGPL");
	lgpl.open(QIODevice::ReadOnly);
	QTextStream lgplStream(&lgpl);
	ui->infoEdit->setText(lgplStream.readAll());
}

FirstConfigForm::~FirstConfigForm()
{
	delete ui;
}

void FirstConfigForm::on_acceptCheckBox_stateChanged(int arg1){

	if(arg1 == Qt::Checked){
		m_goToConnection->setEnabled(true);
	}else{
		m_goToConnection->setEnabled(false);
	}
}

void FirstConfigForm::on_goToConnectionButton_clicked(){

	m_tabWidget->setTabEnabled(0, false);
	m_tabWidget->setTabEnabled(1, true);
	m_connectionMessage->setText("Inserisci i parametri per la connessione al server.");
}

void FirstConfigForm::on_serverAddressEdit_textChanged(const QString &arg1){

	m_address = arg1;
	if(!m_address.isEmpty() && m_port != 0){
		m_goToRegister->setEnabled(true);
	}
}

void FirstConfigForm::on_serverPortEdit_textChanged(const QString &arg1){

	m_port = arg1.toUShort();
	if(!m_address.isEmpty() && m_port != 0){
		m_goToRegister->setEnabled(true);
	}
}

void FirstConfigForm::onServerAnswer(const CommandPacket::ServerAnswer res){

	m_movie->stop();
	m_registerLoader->setMovie(NULL);
	m_alreadyRegistered->setChecked(false);

	switch (res) {
		case CommandPacket::LoginOk:
			m_registerMessage->setText("Login effettuato con successo!");
			m_client->logout();
			QThread::msleep(1000);
			m_tabWidget->setTabEnabled(2, false);
			m_tabWidget->setTabEnabled(3, true);
			m_tabWidget->setCurrentIndex(3);
			m_goToEnd->setEnabled(true);
			ui->workPathEdit->setText(m_workPath);
			break;
		case CommandPacket::AlreadyLogged:
			m_registerMessage->setText("Risulti già collegato...");
			m_goToPathConfig->setEnabled(true);
			break;
		case CommandPacket::WrongEmail:
			m_registerMessage->setText("Non risulti essere registrato...");
			m_goToPathConfig->setEnabled(true);
			m_alreadyRegistered->setChecked(true);
			break;
		case CommandPacket::WrongPassword:
			m_registerMessage->setText("La password è errata...");
			m_goToPathConfig->setEnabled(true);
			m_alreadyRegistered->setChecked(true);
			break;
		case CommandPacket::UserRegisterOk:
			m_registerMessage->setText("Registrazione effettuata con successo!");
			m_client->disconnectFromHost();
			QThread::msleep(1000);
			m_tabWidget->setTabEnabled(2, false);
			m_tabWidget->setTabEnabled(3, true);
			m_tabWidget->setCurrentIndex(3);
			m_goToEnd->setEnabled(true);
			ui->workPathEdit->setText(m_workPath);
			break;
		case CommandPacket::UsernameAlreadyInUse:
			m_registerMessage->setText("L'email è già in uso...");
			m_goToPathConfig->setEnabled(true);
			break;
		case CommandPacket::UserRegisterFail:
			m_registerMessage->setText("Ci sono problemi con il database, riprova più tardi...");
			m_goToPathConfig->setEnabled(true);
			break;
		default:
			break;
	}
}

void FirstConfigForm::onClientError(const Exception::Type extType){

	m_movie->stop();
	m_registerLoader->setMovie(NULL);
	m_alreadyRegistered->setChecked(false);

	switch (extType) {
		case Exception::UserBadMail:
			m_registerMessage->setText("L'email inserita non è valida, riprova");
			m_goToPathConfig->setEnabled(true);
			break;
		default:
			m_registerMessage->setText("Ops, grave errore interno! Riavvia il client e riprova...");
			break;
	}
}

void FirstConfigForm::onClientConnected(){
	m_goToRegister->setEnabled(true);
	m_connectionMessage->clear();
	m_connectionLoader->setMovie(NULL);
	m_movie->stop();
	m_tabWidget->setTabEnabled(1, false);
	m_tabWidget->setTabEnabled(2, true);
	m_tabWidget->setCurrentIndex(2);
}

void FirstConfigForm::onClientDisconnected(){

}

void FirstConfigForm::onClientSocketError(const QAbstractSocket::SocketError err){

	Q_UNUSED(err);
	m_goToRegister->setEnabled(true);
	m_connectionMessage->setText("Non riesco a raggiungere il server, rivedi i parametri...");
	m_connectionLoader->setMovie(NULL);
	m_movie->stop();

}

void FirstConfigForm::on_goToRegisterButton_clicked(){

	m_goToRegister->setEnabled(false);
	m_connectionLoader->setMovie(m_movie);
	m_movie->start();
	m_connectionMessage->setText("Provo a connettermi al server...");
	m_client->connectToHost(m_address, m_port);
}

void FirstConfigForm::on_alreadyRegistered_stateChanged(int arg1){

	if(arg1 == Qt::Checked){
		m_confirmPasswordEdit->setVisible(false);
		m_confirmPasswordLabel->setVisible(false);
		m_goToPathConfig->setText("Accedi");
	}else{
		m_confirmPasswordEdit->setVisible(true);
		m_confirmPasswordLabel->setVisible(true);
		m_goToPathConfig->setText("Registrati");
	}
}

void FirstConfigForm::on_lineEdit_textChanged(const QString &arg1){

	m_email = arg1;

	if(m_email.isEmpty()){
		m_goToPathConfig->setEnabled(false);
	}else{
		if(m_alreadyRegistered->isChecked()){
			if(!m_hash1.isEmpty()){
				m_goToPathConfig->setEnabled(true);
			}else{
				m_goToPathConfig->setEnabled(false);
			}
		}else{
			if(m_hash1 == m_hash2 && !m_hash1.isEmpty()){
				m_goToPathConfig->setEnabled(true);
			}else{
				m_goToPathConfig->setEnabled(false);
			}
		}
	}
}

void FirstConfigForm::on_lineEdit_2_textChanged(const QString &arg1){

	if(arg1.length() < 6 || arg1.contains(" ")){
		m_registerMessage->setText("La password inserita non è valida (almeno 6 caratteri e nessuno spazio).");
		return;
	}

	m_hash1 = QCryptographicHash::hash(arg1.toLocal8Bit(), QCryptographicHash::Md5).toHex();

	if(m_alreadyRegistered->isChecked()){
		m_registerMessage->clear();
		if(!m_email.isEmpty()){
			m_goToPathConfig->setEnabled(true);
		}
	}else{
		m_registerMessage->clear();
		if(m_hash1 == m_hash2 && !m_hash1.isEmpty()){
			m_goToPathConfig->setEnabled(true);
		}else{
			m_registerMessage->setText("Le password non coincidono.");
		}
	}
}

void FirstConfigForm::on_confirmPasswordEdit_textChanged(const QString &arg1){

	if(arg1.length() < 6 || arg1.contains(" ")){
		m_registerMessage->setText("La password inserita non è valida (almeno 6 caratteri e nessuno spazio).");
		return;
	}

	m_hash2 = QCryptographicHash::hash(arg1.toLocal8Bit(), QCryptographicHash::Md5).toHex();

	m_registerMessage->clear();
	if(m_hash2 == m_hash1 && !m_hash2.isEmpty()){
		m_goToPathConfig->setEnabled(true);
	}else{
		m_registerMessage->setText("Le password non coincidono.");
	}
}

void FirstConfigForm::on_goToPathConfig_clicked(){

	m_goToPathConfig->setEnabled(false);
	m_registerLoader->setMovie(m_movie);
	m_movie->start();

	if(m_alreadyRegistered->isChecked()){
		m_registerMessage->setText("Tento il login...");
		m_client->login(m_email, m_hash1, User::NotEncrypt);
	}else{
		m_registerMessage->setText("Tento la registrazione...");
		m_client->userRegister(m_email, m_hash1, User::NotEncrypt);
	}
}

void FirstConfigForm::on_changePathButton_clicked(){

	m_workPath = QFileDialog::getExistingDirectory(this, "Seleziona cartella di lavoro", QDir::home().path(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	ui->workPathEdit->setText(m_workPath);
}

void FirstConfigForm::on_goToEndButton_clicked(){

	m_tabWidget->setTabEnabled(3, false);
	m_tabWidget->setTabEnabled(4, true);
	m_tabWidget->setCurrentIndex(4);
	m_end->setEnabled(true);
	ui->endEdit->setText("Configurazione completata ma comunque riscrivetemi per bene!");
}

void FirstConfigForm::on_endButton_clicked(){

	QSettings appSettings;

	if(!appSettings.contains(T_STARTED)){
		QDir dir(m_workPath);
		appSettings.setValue(ROOT_PATH, m_workPath);
		dir.mkdir(_DOWNLOAD);
		dir.cd(_DOWNLOAD);
		appSettings.setValue(DOWN_PATH, dir.path());
		dir.cdUp();
		dir.mkdir(_TEMPFILE);
		dir.cd(_TEMPFILE);
		appSettings.setValue(TMPF_PATH, dir.path());
		appSettings.setValue(HOST_ADDR, m_address);
		appSettings.setValue(HOST_PORT, m_port);
		appSettings.setValue(HOME_USER, m_email);
		appSettings.setValue(HOME_PASS, m_hash1);
	}
	appSettings.setValue(T_STARTED, 1);
	m_result = true;
	this->close();
}

bool FirstConfigForm::getResult(){

	return m_result;
}
