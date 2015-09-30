#include "PermSettings.h"
#include "ui_PermSettings.h"

PermSettings::PermSettings(const User * const user, const GraphicResourceHeader * const res, QWidget *parent) : QDialog(parent), ui(new Ui::PermSettings){

	ui->setupUi(this);

	this->setFixedSize(this->size());

	m_table				= ui->tableWidget;
	m_publicRead		= ui->readRadio;
	m_publicWrite		= ui->writeRadio;
	m_public			= ui->publicChec;
	m_currentResource	= res;
	m_user				= user->getEmail();
	m_accepted			= false;
	m_publicPerm		= GraphicResourceHeader::PermUndef;

	ui->addUserBtn->setIcon(QIcon(":/icons/icons/user_add.png"));
	ui->delUserBtn->setIcon(QIcon(":/icons/icons/user_del.png"));

	if(m_currentResource->getType() == GraphicResourceHeader::File){
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

	foreach (QString usr, m_currentResource->getPermissionTable().keys()){
		m_table->insertRow(0);
		QTableWidgetItem * user = new QTableWidgetItem(usr);
		QTableWidgetItem * perm = new QTableWidgetItem(m_currentResource->getPermissionTable()[usr] == GraphicResourceHeader::Read ? "Lettura" : "Scrittura");
		if(m_currentResource->getType() == GraphicResourceHeader::Dir){
			perm->setIcon(perm->text() == "Lettura" ? QIcon(":/icons/icons/read_dir.png") : QIcon(":/icons/icons/write_dir.png"));
		}else{
			perm->setIcon(perm->text() == "Lettura" ? QIcon(":/icons/icons/read_file.png") : QIcon(":/icons/icons/write_file.png"));
		}
		user->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
		perm->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		m_table->setItem(0, 0, user);
		m_table->setItem(0, 1, perm);
	}

	if(m_currentResource->getPublicPermission() != GraphicResourceHeader::PermUndef){
		m_public->setChecked(true);
		m_publicRead->setEnabled(true);
		m_publicWrite->setEnabled(true);
		m_public->setChecked(true);
		m_currentResource->getPublicPermission() == GraphicResourceHeader::Read ? m_publicRead->setChecked(true) : m_publicWrite->setChecked(true);
	}

}

PermSettings::~PermSettings()
{
	delete ui;
}

bool PermSettings::wasAccepted() const{

	return m_accepted;
}

GraphicResourceHeader::ResourcePerm PermSettings::getPublicPerm() const{

	return m_publicPerm;
}

ResourceHeader PermSettings::getNewHeader() const{

	ResourceHeader res(m_currentResource->getHeader());
	res.setPermissionTable(getPermMap());
	res.setPublicPermission(getPublicPerm());
	return res;
}

QMap<QString, GraphicResourceHeader::ResourcePerm> PermSettings::getPermMap() const{

	return m_map;
}

void PermSettings::on_tableWidget_itemClicked(QTableWidgetItem *item){

		if(item->column() == 1){
			if(item->text() == "Lettura"){
				item->setText("Scrittura");
				if(m_currentResource->getType() == GraphicResourceHeader::File){
					item->setIcon(QIcon(":/icons/icons/write_file.png"));
				}else{
					item->setIcon(QIcon(":/icons/icons/write_dir.png"));
				}
			}else{
				item->setText("Lettura");
				if(m_currentResource->getType() == GraphicResourceHeader::File){
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
		m_publicPerm = GraphicResourceHeader::Read;
	}else{
		m_publicRead->setEnabled(false);
		m_publicWrite->setEnabled(false);
		m_publicRead->setChecked(false);
		m_publicPerm = GraphicResourceHeader::PermUndef;
	}
}

void PermSettings::on_addUserBtn_clicked(){

	m_crafting = true;
	m_table->insertRow(0);
	QTableWidgetItem * user = new QTableWidgetItem("Inserisci email");
	QTableWidgetItem * perm = new QTableWidgetItem("Lettura");
	perm->setIcon(m_currentResource->getType() == GraphicResourceHeader::File ? QIcon(":/icons/icons/read_file.png") : QIcon(":/icons/icons/read_dir.png"));
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

	if(m_crafting){
		return;
	}
	try{
		if(item->column() == 0){
			User::checkMail(item->text());
		}
		if(item->text() == m_user){
			QMessageBox::information(this, "KCloud::Permessi", "Mi sembra ovvio che tua abbia tutti i permessi, sei il proprietario...");
			m_table->removeRow(item->row());
		}else if(m_table->findItems(item->text(), Qt::MatchExactly).size() != 1){
			QMessageBox::information(this, "KCloud::Permessi", "Hai già inserito questo utente...");
			m_table->removeRow(item->row());
		}
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

	for(int row = 0; row < m_table->rowCount(); row++){
			if(m_table->item(row, 0)->text() != "Inserisci email"){
				m_map.insert(m_table->item(row, 0)->text(),
							 m_table->item(row, 1)->text() == "Lettura" ?
								 GraphicResourceHeader::Read :
								 GraphicResourceHeader::Write);
			}
	}
	m_accepted = true;
	close();
}

void PermSettings::on_readRadio_clicked(){

	m_publicPerm	= GraphicResourceHeader::Read;
}

void PermSettings::on_writeRadio_clicked(){

	m_publicPerm	= GraphicResourceHeader::Write;
}

void PermSettings::on_cancelBtn_clicked(){

	m_accepted		= false;
	close();
}
