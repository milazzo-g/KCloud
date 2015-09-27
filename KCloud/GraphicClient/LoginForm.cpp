#include "LoginForm.h"
#include "ui_LoginForm.h"

LoginForm::LoginForm(QWidget *parent) :	QDialog(parent), ui(new Ui::LoginForm){


	ui->setupUi(this);

	this->setFixedSize(this->size());

	//QPalette style;
	//style.setColor(QPalette::Background, QColor(51, 102, 255));
	//this->setAutoFillBackground(true);
	//this->setPalette(style);

	m_remember	= ui->rememberCheckBox;
	m_password	= ui->passwordEdit;
	m_message	= ui->messageLabel;
	m_loader	= ui->loaderLabel;
	m_graphics	= ui->graphicView;
	m_email		= ui->emailEdit;
	m_canel		= ui->cancelBtn;
	m_login		= ui->loginBtn;

	m_movie		= new QMovie(this);
	m_pixmap	= new QPixmap(":/images/testImage.png");
	m_scene		= new QGraphicsScene(this);

	m_movie->setFileName(":/animations/loader.gif");
	m_scene->addPixmap(m_pixmap->scaled(100, 100, Qt::KeepAspectRatio));
	m_graphics->setScene(m_scene);
	m_graphics->setStyleSheet("background: transparent; border-style: none;");
	m_remember->setChecked(false);
	m_login->setEnabled(false);

	m_message->clear();
	m_loader->clear();

	//connect
}

LoginForm::~LoginForm(){

	delete m_pixmap;
	delete ui;
}

void LoginForm::onServerAnswer(const CommandPacket::ServerAnswer res){

}

void LoginForm::onClientError(const Exception::Type extType){

}

void LoginForm::onClientConnected(){

}

void LoginForm::onClientDisconnected(){

}

void LoginForm::onClientSocketError(const QAbstractSocket::SocketError err){

}


void LoginForm::on_cancelBtn_clicked(){

	m_email->clear();
	m_password->clear();
	m_message->clear();
}

void LoginForm::on_loginBtn_clicked(){

	QSettings appSettings;
	QString hostAddress = appSettings.value(HOST_ADDR).toString();
	quint16 hostPort = (quint16)appSettings.value(HOST_PORT).toInt();
	m_loader->setMovie(m_movie);
	m_movie->start();
	m_message->setText(QString("Provo a connettermi al server ") + hostAddress);
	m_client->connectToHost(hostAddress, hostPort);
}

void LoginForm::closeEvent(QCloseEvent * event){

	event->ignore();
	QMessageBox::information(this, "Suca", "non mi puoi chiudere neanche con il cazzo (KILL -SIGKILL [miopid])");
}
