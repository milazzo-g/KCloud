#include "LoginForm.h"
#include "ui_LoginForm.h"

LoginForm::LoginForm(Client *client, QWidget *parent) :	QDialog(parent), ui(new Ui::LoginForm){


	QSettings appSettings;
	ui->setupUi(this);
	this->setFixedSize(this->size());

	m_result	= false;
	m_remember	= ui->rememberCheckBox;
	m_password	= ui->passwordEdit;
	m_message	= ui->messageLabel;
	m_loader	= ui->loaderLabel;
	m_graphics	= ui->graphicView;
	m_email		= ui->emailEdit;
	m_canel		= ui->cancelBtn;
	m_login		= ui->loginBtn;
	m_client	= client;

	m_movie		= new QMovie(this);
	m_pixmap	= new QPixmap(":/kcloudIcons/kcloud_128x128.png");
	m_scene		= new QGraphicsScene(this);

	m_movie->setFileName(":/animations/loader.gif");
	m_scene->addPixmap(*m_pixmap);
	m_graphics->setScene(m_scene);
	m_graphics->setStyleSheet("background: transparent; border-style: none;");
	m_remember->setChecked(false);
	m_login->setEnabled(false);

	m_message->clear();
	m_loader->clear();

	if(!appSettings.value(HOME_USER).toString().isEmpty() && !appSettings.value(HOME_PASS).toString().isEmpty()){
		m_remember->setChecked(true);
		m_resultMail = appSettings.value(HOME_USER).toString();
		m_resultHash = appSettings.value(HOME_PASS).toString();
		m_email->setText(m_resultMail);
		disconnect(ui->passwordEdit, SIGNAL(textChanged(QString)), this, SLOT(on_passwordEdit_textChanged(QString)));
		m_password->setText("FAKEPASSOWRD");
		connect(ui->passwordEdit, SIGNAL(textChanged(QString)), this, SLOT(on_passwordEdit_textChanged(QString)), Qt::UniqueConnection);
		m_login->setEnabled(true);
	}

	connect(m_client, SIGNAL(clientError(Exception::Type)), this, SLOT(onClientError(Exception::Type)));
	connect(m_client, SIGNAL(connected()), this, SLOT(onClientConnected()));
	connect(m_client, SIGNAL(serverAnswer(CommandPacket::ServerAnswer)), this, SLOT(onServerAnswer(CommandPacket::ServerAnswer)));
	connect(m_client, SIGNAL(socketError(QAbstractSocket::SocketError)), this, SLOT(onClientSocketError(QAbstractSocket::SocketError)));
}

LoginForm::~LoginForm(){

	delete m_pixmap;
	delete ui;
}

bool LoginForm::getResult() const{

	return m_result;
}

void LoginForm::onServerAnswer(const CommandPacket::ServerAnswer res){

	m_movie->stop();
	m_loader->clear();
	m_canel->setEnabled(true);
	QSettings appSettings;

	switch (res) {
		case CommandPacket::LoginOk:
			m_message->setText("Login effettuato con successo!");
			m_result = true;
			QThread::msleep(1000);
			if(m_remember->isChecked()){
				appSettings.setValue(HOME_USER, m_resultMail);
				appSettings.setValue(HOME_PASS, m_resultHash);
			}else{
				appSettings.setValue(HOME_USER, "");
				appSettings.setValue(HOME_PASS, "");
			}
			close();
			break;
		case CommandPacket::AlreadyLogged:
			m_message->setText("Risulti già collegato...!");
			m_login->setEnabled(true);
			m_client->disconnectFromHost();
			break;
		case CommandPacket::WrongEmail:
			m_message->setText("Non risulti essere registrato...");
			m_login->setEnabled(true);
			m_client->disconnectFromHost();
			break;
		case CommandPacket::WrongPassword:
			m_message->setText("La password è errata...");
			m_login->setEnabled(true);
			m_client->disconnectFromHost();
			break;
		default:
			m_message->setText("Errore interno al server, riprova più tardi...");
			m_login->setEnabled(true);
			m_client->disconnectFromHost();
			break;
	}

}

void LoginForm::onClientError(const Exception::Type extType){

	m_movie->stop();
	m_loader->clear();

	switch (extType) {
		case Exception::UserBadMail:
			m_client->disconnectFromHost();
			m_message->setText("L'email inserita non è valida, riprova...");
			m_login->setEnabled(true);
			m_canel->setEnabled(true);
			break;
		default:
			m_client->disconnectFromHost();
			m_message->setText("Ops, grave errore interno! Riavvia il client...");
			break;
	}
}

void LoginForm::onClientConnected(){

	m_message->setText("Connesso al server...");
	m_client->login(m_resultMail, m_resultHash, User::NotEncrypt);
}

void LoginForm::onClientSocketError(const QAbstractSocket::SocketError err){

	Q_UNUSED(err);
	m_loader->clear();
	m_movie->stop();
	m_message->setText("Non riesco a raggingere il server, riprova più tardi...");
	m_login->setEnabled(true);
}

void LoginForm::on_cancelBtn_clicked(){

	m_email->clear();
	m_password->clear();
	m_message->clear();
}

void LoginForm::on_loginBtn_clicked(){

	QSettings appSettings;
	m_login->setEnabled(false);
	m_canel->setEnabled(false);
	QString hostAddress = appSettings.value(HOST_ADDR).toString();
	quint16 hostPort = (quint16)appSettings.value(HOST_PORT).toInt();
	m_loader->setMovie(m_movie);
	m_movie->start();
	m_message->setText(QString("Provo a connettermi al server ") + hostAddress);
	m_client->connectToHost(hostAddress, hostPort);
}

void LoginForm::on_emailEdit_textChanged(const QString &arg1){

	m_resultMail = arg1;
	if(!m_resultMail.isEmpty() && !m_password->text().isEmpty()){
		m_login->setEnabled(true);
	}else{
		m_login->setEnabled(false);
	}
}

void LoginForm::on_passwordEdit_textChanged(const QString &arg1){



	if(arg1.length() < 6 || arg1.contains(" ")){
		m_message->setText("La password inserita non è valida...");
		m_resultHash.clear();
	}else{
		m_resultHash = QCryptographicHash::hash(arg1.toLocal8Bit(), QCryptographicHash::Md5).toHex();
		m_message->clear();
	}
	if(!m_resultMail.isEmpty() && !m_resultHash.isEmpty()){
		m_login->setEnabled(true);
	}else{
		m_login->setEnabled(false);
	}
}
