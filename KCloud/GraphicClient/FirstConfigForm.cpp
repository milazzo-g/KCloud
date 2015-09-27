#include "FirstConfigForm.h"
#include "ui_FirstConfigForm.h"

FirstConfigForm::FirstConfigForm(QWidget *parent) : QDialog(parent), ui(new Ui::FirstConfigForm){

	ui->setupUi(this);
	ui->tabWidget->setTabEnabled(1, false);
	ui->tabWidget->setTabEnabled(2, false);
	ui->tabWidget->setTabEnabled(3, false);

	ui->endBtn->setEnabled(false);
	ui->endBtn_2->setEnabled(false);
	ui->nextPageBtn->setEnabled(false);
	ui->nextPageBtn_2->setEnabled(false);
	ui->serverAddressEdit->setValidator(new QIntValidator(0, 65535, this));
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

	QRegExp reg1("^(([a-zA-Z0-9]|[a-zA-Z0-9][a-zA-Z0-9\-]*[a-zA-Z0-9])\.)*([A-Za-z0-9]|[A-Za-z0-9][A-Za-z0-9\-]*[A-Za-z0-9])$", Qt::CaseInsensitive);
	QRegExp reg2("^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$", Qt::CaseInsensitive);

	if(reg1.exactMatch(arg1) || reg2.exactMatch(arg1)){
		m_address = arg1;
		ui->messageLabel->clear();
	}else{
		ui->messageLabel->setText("L'indirizzo del server non è un indirizzo valido!");
	}

	if(!m_address.isEmpty() && m_port != 0){
		ui->nextPageBtn_2->setEnabled(true);
	}
}

void FirstConfigForm::on_serverPortEdit_textChanged(const QString &arg1)
{

}
