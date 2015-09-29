#include "PermSettings.h"
#include "ui_PermSettings.h"

PermSettings::PermSettings(const User * const user, const ResourceHeader::ResourceType type, QWidget *parent) : QDialog(parent), ui(new Ui::PermSettings){

	ui->setupUi(this);

	m_table			= ui->tableWidget;
	m_opMode		= type;
	m_publicRead	= ui->readRadio;
	m_publicWrite	= ui->writeRadio;
	m_public		= ui->publicChec;
	m_user			= user->getEmail();
	m_accepted		= false;

	ui->addUserBtn->setIcon(QIcon(":/icons/icons/user_add.png"));
	ui->delUserBtn->setIcon(QIcon(":/icons/icons/user_del.png"));

	if(m_opMode == GraphicResourceHeader::File){
		m_publicRead->setIcon(QIcon(":/icons/icons/read_file.png"));
		m_publicWrite->setIcon(QIcon(":/icons/icons/write_file.png"));
	}else{
		m_publicRead->setIcon(QIcon(":/icons/icons/read_dir.png"));
		m_publicWrite->setIcon(QIcon(":/icons/icons/write_dir.png"));
	}

	m_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
	m_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
	m_table->setRowCount(0);
	m_public->setChecked(false);
	m_publicRead->setEnabled(false);
	m_publicWrite->setEnabled(false);
}

PermSettings::~PermSettings()
{
	delete ui;
}

void PermSettings::on_tableWidget_itemClicked(QTableWidgetItem *item){

		if(item->column() == 1){
			if(item->text() == "Lettura"){
				item->setText("Scrittura");
				if(m_opMode == GraphicResourceHeader::File){
					item->setIcon(QIcon(":/icons/icons/write_file.png"));
				}else{
					item->setIcon(QIcon(":/icons/icons/write_dir.png"));
				}
			}else{
				item->setText("Lettura");
				if(m_opMode == GraphicResourceHeader::File){
					item->setIcon(QIcon(":/icons/icons/read_file.png"));
				}else{
					item->setIcon(QIcon(":/icons/icons/read_dir.png"));
				}
			}
		}
}

void PermSettings::on_publicChec_stateChanged(int arg1){

	if(arg1 == Qt::Checked){
		m_publicRead->setEnabled(true);
		m_publicWrite->setEnabled(true);
		m_publicRead->setChecked(true);
	}else{
		m_publicRead->setEnabled(false);
		m_publicWrite->setEnabled(false);
		m_publicRead->setChecked(false);
	}
}

void PermSettings::on_addUserBtn_clicked(){

	m_crafting = true;
	m_table->insertRow(0);
	QTableWidgetItem * user = new QTableWidgetItem("Inserisci email");
	QTableWidgetItem * perm = new QTableWidgetItem("Clicca per impostare");
	user->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
	perm->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	m_table->setItem(0, 0, user);
	m_table->setItem(0, 1, perm);
	m_crafting = false;
}

void PermSettings::on_delUserBtn_clicked(){

	if(m_table->rowCount() == 0){
		return;
	}
	m_table->removeRow(m_table->currentRow());
}

void PermSettings::on_tableWidget_itemChanged(QTableWidgetItem *item){

	try{
		if(item->column() == 0 && !m_crafting){
			User::checkMail(item->text());
		}
		if(item->text() == m_user){
			QMessageBox::information(this, "KCloud::Permessi", "Mi sembra ovvio che tua abbia tutti i permessi, sei il proprietario...");
			m_table->removeRow(item->row());
		}else if(m_table->findItems(item->text(), ))
	}catch(Exception &e){
		if(e.type() == Exception::UserBadMail){
			QMessageBox::warning(this, "KCloud::Permessi",
								 "L'email che hai inserito non corrisponde ad un normale indirizzo...");
			m_table->removeRow(item->row());
		}else{
			QMessageBox::critical(this, "KCloud::Permessi",
								  QString("Si è verificata un'eccezione che non sono in grado di gestire, "
										  "il codice di errore è ") +
								  QString::number((qint32)e.type()) +
								  QString(": ") +
								  QString(e.what()));
			close();
		}
	}

}

void PermSettings::on_confirmBtn_clicked(){

	m_accepted = true;
	for(int row = 0; row < m_table->rowCount(); row++){
			m_map.insert(m_table->item(row, 0)->text(),
						 m_table->item(row, 1)->text() == "Lettura" ?
							 GraphicResourceHeader::Read :
							 GraphicResourceHeader::Write);
	}
	close();
}
