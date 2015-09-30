#include "Waiter.h"
#include "ui_Waiter.h"

Waiter::Waiter(QWidget *parent) : QDialog(parent), ui(new Ui::Waiter){

	ui->setupUi(this);

	this->setFixedSize(this->size());

	m_movie		= new QMovie(this);
	m_message	= ui->m_message;
	m_loader	= ui->m_loader;
	m_close		= true;
	m_timer		= new QTimer(this);


	m_movie->setFileName(":/animations/animations/loader.gif");
	QTimer::singleShot(0, this, SLOT(startLoader()));
}

Waiter::~Waiter()
{
	delete ui;
}

void Waiter::waitForServer() const{

	emit waitComplete();
}

void Waiter::quit(){
	m_close	= false;
	close();
}

void Waiter::setMessage(const QString &message){
	m_close = true;
	m_message->setText(message);
}

void Waiter::show(){

	m_timer->setSingleShot(true);
	m_timer->setInterval(60000);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(panic()));
	m_timer->start();
	QDialog::show();
}

void Waiter::closeEvent(QCloseEvent *event){

	if(m_close){
		event->ignore();
	}else{
		event->accept();
		disconnect(m_timer, SIGNAL(timeout()), this, SLOT(panic()));
	}
}

void Waiter::startLoader(){

	m_loader->setMovie(m_movie);
	m_movie->start();
}

void Waiter::panic(){
	QMessageBox::critical(this, "KCloud::Errore", "Il server non risponde, riavvia il client...");

	emit serverTimeout();
}
