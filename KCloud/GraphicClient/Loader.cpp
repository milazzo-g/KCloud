#include "Loader.h"
#include "ui_Loader.h"
#include "../MainServer/defines.h"

Loader::Loader(QWidget *parent) : QDialog(parent), ui(new Ui::Loader){

	ui->setupUi(this);
	m_close			= true;
	m_transferred	= ui->transferredLabel;
	m_remaining		= ui->remainingLabel;
	m_progressBar	= ui->progressBar;
	m_message		= ui->messageLabel;


	m_progressBar->setValue(0);
}

Loader::~Loader()
{
	delete ui;
}

void Loader::closeEvent(QCloseEvent *event){

	if(m_close){
		event->ignore();
	}else{
		event->accept();
	}

}

void Loader::updateStatus(const qint64 &total, const qint64 &transmitted, const Resource::Mode mode){

	m_transferred->setText(QString::number(transmitted));
	m_remaining->setText(QString::number(total - transmitted));
	double ratio = (static_cast<double>(transmitted) / total) * 100;
	m_progressBar->setValue(ratio);
	m_message->setText(QString((mode == Resource::Download ? "Sto scaricando..." : "Sto caricando...")));

	if((total - transmitted) == 0){
		m_message->clear();
		m_remaining->clear();
		m_transferred->clear();
		emit trasmissionEnd();
	}
}

void Loader::quit(){
	m_close	= false;
	close();
}


