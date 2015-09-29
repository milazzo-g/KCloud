#include "Waiter.h"
#include "ui_Waiter.h"

Waiter::Waiter(QWidget *parent) : QDialog(parent), ui(new Ui::Waiter){

	ui->setupUi(this);
	m_movie		= new QMovie(this);
	m_message	= ui->m_message;
	m_loader	= ui->m_loader;
	m_close		= true;

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
	trace << "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
	m_close = true;
	m_message->setText(message);
}

void Waiter::closeEvent(QCloseEvent *event){

	if(m_close){
		event->ignore();
	}else{
		event->accept();
	}
}

void Waiter::startLoader(){

	m_loader->setMovie(m_movie);
	m_movie->start();
}
