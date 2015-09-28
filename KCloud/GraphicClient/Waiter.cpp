#include "Waiter.h"
#include "ui_Waiter.h"

Waiter::Waiter(Client * client, const QString &message, QWidget *parent) : QDialog(parent), ui(new Ui::Waiter){

	ui->setupUi(this);
	m_client	= client;
	m_movie		= new QMovie(this);
	m_message	= ui->m_message;
	m_loader	= ui->m_loader;
	m_close		= true;

	m_errorsList.clear();
	m_movie->setFileName(":/animations/loader.gif");
	m_message->setText(message);

	connect(m_client, SIGNAL(serverAnswer(CommandPacket::ServerAnswer)), this, SLOT(onServerAnswer(CommandPacket::ServerAnswer)));
	connect(m_client, SIGNAL(clientError(Exception::Type)), this, SLOT(onClientError(Exception::Type)));
	connect(m_client, SIGNAL(finalizeOK()), this, SLOT(restoreClose()));

	QTimer::singleShot(0, this, SLOT(startLoader()));
}

Exception::Type Waiter::getError() const{
	return m_error;
}

CommandPacket::ServerAnswer Waiter::getAnswer() const{
	return m_answer;
}

QStringList Waiter::getErrorStrings() const{
	return m_errorsList;
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

void Waiter::onServerAnswer(const CommandPacket::ServerAnswer res){
	m_close			= false;
	m_answer		= res;
	m_errorsList	= m_client->lastErrors();
	close();
}

void Waiter::onClientError(const Exception::Type type){
	m_close			= false;
	m_error			= type;
	m_errorsList	= m_client->lastErrors();
	close();
}

void Waiter::restoreClose(){
	m_close			= false;
	QThread::msleep(100);
	close();
}

void Waiter::stopLoader(){

	m_movie->stop();
	m_loader->clear();
}

void Waiter::startLoader(){

	m_loader->setMovie(m_movie);
	m_movie->start();
}
