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

	m_pixmap	= new QPixmap(":/images/testImage.png");
	m_scene		= new QGraphicsScene(this);

	m_scene->addPixmap(m_pixmap->scaled(100, 100, Qt::KeepAspectRatio));
	m_graphics->setScene(m_scene);
	m_graphics->setStyleSheet("background: transparent; border-style: none;");
	m_remember->setChecked(false);
	m_login->setEnabled(false);

	m_message->clear();
	m_loader->clear();

	connect(m_email		, SIGNAL(textChanged(QString)	), this, SLOT(updateMail(QString))	);
	connect(m_password	, SIGNAL(textChanged(QString)	), this, SLOT(updateHash(QString))	);
	connect(m_login		, SIGNAL(clicked(bool)			), this, SLOT(connectHost())		);

}

LoginForm::~LoginForm(){

	delete m_scene;
	delete m_pixmap;
	delete ui;
}

void LoginForm::updateMail(const QString &mail){

	if(!mail.isEmpty() && !m_resultHash.isEmpty()){
		m_login->setEnabled(true);
	}else{
		m_login->setEnabled(false);
	}
	m_resultMail = mail;
}

void LoginForm::updateHash(const QString &hash){

	if(!hash.isEmpty() && !m_resultMail.isEmpty()){
		m_login->setEnabled(true);
	}else{
		m_login->setEnabled(false);
	}
	m_resultHash = QCryptographicHash::hash(hash.toLocal8Bit(), QCryptographicHash::Md5).toHex();
}



void LoginForm::connectHost(){

	m_movie = new QMovie(this);
	m_movie->setFileName(":/animations/loader.gif");
	m_loader->setMovie(m_movie);
	m_movie->start();
	m_message->setText(tr("Provo a connettermi al server..."));
}
