#include "Waiter.h"
#include "ui_Waiter.h"

Waiter::Waiter(Client * client, const QString &message, QWidget *parent) : QDialog(parent), ui(new Ui::Waiter){

	ui->setupUi(this);
	m_client	= client;
	m_movie		= new QMovie(this);
	m_message	= ui->m_message;
	m_loader	= ui->m_loader;
	m_close		= true;

	m_movie->setFileName(":/animations/loader.gif");
	m_message->setText(message);

	connect(m_client, SIGNAL(newCommand()), this, SLOT(restoreClose()));
	connect(m_client, SIGNAL(finalizeOK()), this, SLOT(restoreClose()));

	QTimer::singleShot(0, this, SLOT(startLoader()));
}

Waiter::~Waiter()
{
	delete ui;
}

void Waiter::closeEvent(QCloseEvent *event){

	if(m_close){
		event->ignore();
	}else{
		event->accept();
	}
}

void Waiter::restoreClose(){
	m_close			= false;
	disconnect(m_client, SIGNAL(newCommand()), this, SLOT(restoreClose()));
	disconnect(m_client, SIGNAL(finalizeOK()), this, SLOT(restoreClose()));
	close();
}

void Waiter::startLoader(){

	m_loader->setMovie(m_movie);
	m_movie->start();
}
