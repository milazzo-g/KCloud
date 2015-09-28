#include "Loader.h"
#include "ui_Loader.h"

Loader::Loader(Client *client, const Loader::Mode mode, QWidget *parent) : QDialog(parent), ui(new Ui::Loader){

	ui->setupUi(this);
	m_mode		= mode;
	m_client	= client;

}

Loader::~Loader()
{
	delete ui;
}

void Loader::closeEvent(QCloseEvent *event){

	event->ignore();
	if(m_mode == Download){
		if(m_spacchio == 0){
			m_message->setText("Attendi che il download termini...");
			m_spacchio++;
		}else{
			m_message->setText("Professore, non si può fare...");
		}
	}else{
		if(m_spacchio == 0){
			m_message->setText("Attendi che l'upload termini...");
			m_spacchio++;
		}else{
			m_message->setText("Professore, non si può fare...");
		}
	}
	QTimer::singleShot(5000, this, SLOT(setMessage()));
}

void Loader::setMessage(){
	m_message->clear();
	if(m_mode == Download){
		m_message->setText("Sto scariando...");
	}else{
		m_message->setText("Sto caricando...");
	}
}
