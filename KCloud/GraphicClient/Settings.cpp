#include "Settings.h"
#include "ui_Settings.h"

Settings::Settings(QWidget *parent) : QDialog(parent), ui(new Ui::Settings){

	QRegExp rx1("^0*(?:6553[0-5]|655[0-2][0-9]|65[0-4][0-9]{2}|6[0-4][0-9]{3}|[1-5][0-9]{4}|[1-9][0-9]{1,3}|[0-9])$");
	QRegExp rx2("[A-Z\.0-9]+");

	ui->setupUi(this);

	this->setFixedSize(this->size());

	QSettings appSettings;

	QRegExpValidator * portValidator = new QRegExpValidator(this);
	QRegExpValidator * addrValidator = new QRegExpValidator(this);

	ui->serverAddr->setValidator(addrValidator);
	ui->servPort->setValidator(portValidator);

	portValidator->setRegExp(rx1);
	addrValidator->setRegExp(rx2);
	m_accepted = false;
	ui->serverAddr->setText(appSettings.value(HOST_ADDR).toString());
	ui->servPort->setText(appSettings.value(HOST_PORT).toString());
	ui->path->setText(appSettings.value(ROOT_PATH).toString());

	if(appSettings.contains(TIME__OUT)){
		ui->timeout->setValue(appSettings.value(TIME__OUT).toUInt());
	}else{
		appSettings.setValue(TIME__OUT, 30);
		ui->timeout->setValue(30);
	}
	ui->timeout->setMinimum(5);
	ui->timeout->setMaximum(60);
	ui->path->setReadOnly(true);
}

bool Settings::wasAccepted() const{

	return m_accepted;
}

Settings::~Settings()
{
	delete ui;
}

void Settings::on_changePath_clicked(){

	QString p = QFileDialog::getExistingDirectory(this, "Seleziona cartella di lavoro", QDir::home().path(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if(p.isEmpty()){
		return;
	}
	ui->path->setText(p);
}

void Settings::on_confirmBtn_clicked()
{

}
