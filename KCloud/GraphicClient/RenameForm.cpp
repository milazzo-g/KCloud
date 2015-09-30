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
	m_view			=	ui->graphicsView;
	m_table			=	ui->tableWidget;
	m_header		=	res;
	m_accepted		=	false;

	m_renameLabel->setText(QString("Rinomina ") + res->getName() + QString(" come"));

	if(res->getType() == GraphicResourceHeader::Dir){

		QRegExpValidator * validator = new QRegExpValidator(this);
		validator->setRegExp(rx);
		m_name->setValidator(validator);
	}

	QGraphicsScene * m_scene = new QGraphicsScene(this);
	m_scene->addPixmap(m_header->getImage());
	m_view->setScene(m_scene);

	m_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
	m_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
	m_table->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	m_table->horizontalHeader()->hide();
	m_table->verticalHeader()->hide();
	m_table->item(0, 1)->setText(m_header->getName());
	m_table->item(1, 1)->setText(QString::number(m_header->getSize()));
	m_table->item(2, 1)->setText(m_header->getType() == GraphicResourceHeader::Dir ? "Cartella" : "File");
	if(m_header->getPublicPermission() != GraphicResourceHeader::PermUndef){
		if(m_header->getType() == GraphicResourceHeader::Dir){
			if(m_header->getPublicPermission() == GraphicResourceHeader::Read){
				m_table->item(3, 1)->setIcon(QIcon(":/icons/icons/read_dir.png"));
			}else{
				m_table->item(3, 1)->setIcon(QIcon(":/icons/icons/write_dir.png"));
			}
		}else{
			if(m_header->getPublicPermission() == GraphicResourceHeader::Read){
				m_table->item(3, 1)->setIcon(QIcon(":/icons/icons/read_file.png"));
			}else{
				m_table->item(3, 1)->setIcon(QIcon(":/icons/icons/write_file.png"));
			}
		}
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
		close();
	}
}
