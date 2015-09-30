#include "RenameForm.h"
#include "ui_RenameForm.h"

RenameForm::RenameForm(const GraphicResourceHeader * const res, QWidget *parent) : QDialog(parent), ui(new Ui::RenameForm){

	ui->setupUi(this);

	this->setFixedSize(this->size());

	QRegExp rx("[a-z0-9]+", Qt::CaseInsensitive, QRegExp::RegExp);

	m_name			=	ui->newNameEdit;
	m_confirm		=	ui->confirmButton;
	m_cancel		=	ui->cancelButton;
	m_renameLabel	=	ui->renameLabel;
	m_header		=	res;
	m_accepted		=	false;

	m_renameLabel->setText(QString("Rinomina ") + res->getName() + QString(" come"));

	if(res->getType() == GraphicResourceHeader::Dir){

		QRegExpValidator * validator = new QRegExpValidator(this);
		validator->setRegExp(rx);
		m_name->setValidator(validator);
	}
}

RenameForm::~RenameForm(){

	delete ui;
}

bool RenameForm::wasAccepted(){

	return m_accepted;
}

QString RenameForm::getNewName(){

	return m_newName;
}

void RenameForm::on_cancelButton_clicked(){

	this->close();
}

void RenameForm::on_confirmButton_clicked(){

	if(!(m_name->text().isEmpty())){
		m_accepted	= true;
		m_newName	= m_name->text();
	}
}
