#include "FirstConfigForm.h"
#include "ui_FirstConfigForm.h"

FirstConfigForm::FirstConfigForm(QWidget *parent) : QDialog(parent), ui(new Ui::FirstConfigForm){

	QRegExp rx1("^0*(?:6553[0-5]|655[0-2][0-9]|65[0-4][0-9]{2}|6[0-4][0-9]{3}|[1-5][0-9]{4}|[1-9][0-9]{1,3}|[0-9])$");

	ui->setupUi(this);
	ui->tabWidget->setTabEnabled(1, false);
	ui->tabWidget->setTabEnabled(2, false);
	ui->tabWidget->setTabEnabled(3, false);

	ui->endBtn->setEnabled(false);
	ui->endBtn_2->setEnabled(false);
	ui->nextPageBtn->setEnabled(false);
	ui->nextPageBtn_2->setEnabled(false);
	ui->serverPortEdit->setValidator(new QRegExpValidator(rx1, this));

	m_port = 0;

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
		ui->nextPageBtn->setEnabled(true);
	}else{
		ui->nextPageBtn->setEnabled(false);
	}
}

void FirstConfigForm::on_nextPageBtn_clicked(){

	ui->tabWidget->setTabEnabled(0, false);
	ui->tabWidget->setTabEnabled(1, true);
	ui->tabWidget->setCurrentIndex(1);
}

void FirstConfigForm::on_serverAddressEdit_textChanged(const QString &arg1){


	QStringList str = arg1.split(".");

	bool res = true;

	foreach (QString s, str) {
		res = res && (s.toInt() >= 0 && s.toInt() <= 255);
	}

	if(str.size() != 4 || !res){
		ui->messageLabel->setText("L'indirizzo del server non è un indirizzo valido!");
	}else{
		m_address = arg1;
		ui->messageLabel->clear();
	}

	if(!m_address.isEmpty() && m_port != 0){
		ui->nextPageBtn_2->setEnabled(true);
	}
}

void FirstConfigForm::on_serverPortEdit_textChanged(const QString &arg1){

	m_port = arg1.toUShort();

	if(!m_address.isEmpty() && m_port != 0){
		ui->nextPageBtn_2->setEnabled(true);
	}

}
