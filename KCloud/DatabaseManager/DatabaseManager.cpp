#include "DatabaseManager.h"

//const QString	KCloud::DatabaseManager::m_username("soefykxr_testgd");
//const QString	KCloud::DatabaseManager::m_password("Z3Jx0id13lxB");
//const QString	KCloud::DatabaseManager::m_hostname("mysql.netsons.com");
//const QString	KCloud::DatabaseManager::m_database("soefykxr_testGD");
//const int		KCloud::DatabaseManager::m_hostport(3306);

const QString	KCloud::DatabaseManager::m_username("KCloudMan");
const QString	KCloud::DatabaseManager::m_password("kcloud");
const QString	KCloud::DatabaseManager::m_hostname("localhost");
const QString	KCloud::DatabaseManager::m_database("KCloud");
const int		KCloud::DatabaseManager::m_hostport(3306);

KCloud::DatabaseManager::DatabaseManager(const QString &name, QObject *parent) : QObject(parent){

	m_db = QSqlDatabase::addDatabase("QMYSQL", name);
	m_db.setUserName(m_username);
	m_db.setPassword(m_password);
	m_db.setHostName(m_hostname);
	m_db.setDatabaseName(m_database);
	m_db.setPort(m_hostport);
	m_openFlag = false;
	m_lastDriverError.clear();
    m_lastSqlError.clear();
}

KCloud::DatabaseManager::~DatabaseManager()
{

}

bool KCloud::DatabaseManager::open(){

	m_db.open();
	QSqlQuery q(m_db);
	m_openFlag = q.exec("SELECT VERSION()");
	m_lastDriverError	= (isOpen() ? "No Error!" : q.lastError().driverText());
	m_lastSqlError		= (isOpen() ? "No Error!" : q.lastError().databaseText());
	return isOpen();
}

bool KCloud::DatabaseManager::isOpen() const{

	return m_openFlag;
}

void KCloud::DatabaseManager::close(){

	m_db.close();
	m_openFlag = false;
}

QString KCloud::DatabaseManager::lastDriverError() const{

	return m_lastDriverError;
}

QString KCloud::DatabaseManager::lastSqlError() const{

	return m_lastSqlError;
}
