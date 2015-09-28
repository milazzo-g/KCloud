#include "Resource.h"

#define ZIP	".zip"

KCloud::Resource::Resource(QObject *parent) : NetObject(parent){

	m_zipFile = NULL;
}

QString KCloud::Resource::getResourcePath() const{

	return m_resourcePath;
}

QString KCloud::Resource::getZipName() const{

	return m_zipName;
}

QString KCloud::Resource::getZipPath() const{

	//eventualmente sistemare per winzoz
	QString zip;
	zip += getZipDir();
	zip += "/";
	zip += getZipName();
	return zip;
}

void KCloud::Resource::setResourcePath(const QString &path) throw(Exception){

	if(path.isEmpty()){
		throw EmptyPathException();
	}

	m_resourcePath = path;
	checkResource();
}

void KCloud::Resource::setZipName(const QString &name, const QString &ext){

	m_zipName = name;
	if(ext == "" || !ext.startsWith(".")){
		m_zipName += ZIP;
	}else{
		m_zipName += ext;
	}
}

void KCloud::Resource::setZipDir(const QString &path) throw(Exception){

	if(path.isEmpty()){
		throw EmptyPathException();
	}

	m_zipDir = path;
	checkDir();
}

QString KCloud::Resource::getZipDir() const{

	return m_zipDir;
}

void KCloud::Resource::clear(){

	NetObject::clear();
	setNotCompressed();
	m_resourcePath.clear();
	m_zipDir.clear();
	m_zipName.clear();
	if(m_zipFile){
		m_zipFile->close();

		delete m_zipFile;
	}
}

void KCloud::Resource::prepareForSend() throw(Exception){

	compress();
	checkZip();

	if(isCompressed()){
		m_zipFile = new QFile(getZipPath());
		m_zipFile->open(QIODevice::ReadOnly);

		NetObject::prepareForSend();
		m_total = calculateNetworkSize() + (qint64)sizeof(calculateNetworkSize());
	}
}

void KCloud::Resource::prepareForRecv(){

	m_zipFile = new QTemporaryFile(this);
	m_zipFile->open(QIODevice::WriteOnly);
	setReady();
	qDebug() << "void KCloud::Resource::prepareForRecv() -> OK!";
}

void KCloud::Resource::compress() throw(Exception){

	checkResource();
	checkDir();
	if(QFileInfo(getResourcePath()).isDir()){

		if(JlCompress::compressDir(getZipPath(), getResourcePath())){

			setCompressed();
			return;
		}else{

			throw ZippingErrorException();
		}

	}

	if(JlCompress::compressFile(getZipPath(), getResourcePath())){

		setCompressed();
		return;
	}else{

		throw ZippingErrorException();
	}
}

void KCloud::Resource::decompress(const ResourceHeader &head, const bool &autoRemove) throw(Exception){
	//giuseppe

	checkZip();

	QStringList tmp;
	QDir dir(getZipDir());
	if(head.getType() == ResourceHeader::Dir){
		dir.mkdir(QFileInfo(getZipPath()).baseName());
		dir.cd(QFileInfo(getZipPath()).baseName());
	}
	tmp = JlCompress::extractDir(getZipPath(), dir.path());
	if(autoRemove){
		m_zipFile->remove();
	}
	if(tmp.isEmpty()){

		throw UnZippingErrorException();
	}
}

bool KCloud::Resource::removeZipFile() throw(Exception){

	checkZip();
	if(m_zipFile){
		return m_zipFile->remove();
	}
	return false;
}

void KCloud::Resource::send(const qint64 block){

	qDebug() << "Numero Packet rimanenti = " << m_packets - block + 1;
	if(block == 0){
		QDataStream stream(m_channel);
		stream << getNetworkSize();
		qDebug() << "Dimensione da inviare = " << getNetworkSize();
		return;
	}
	if(block <= m_packets){
		m_bytesCounter = getBytesPerPacket();
		m_channel->write(m_zipFile->read(getBytesPerPacket()));
		return;
	}
	if(block == m_packets + 1){
		m_bytesCounter = m_spareBytes;
		m_channel->write(m_zipFile->read(m_bytesCounter));
		return;
	}
}

void KCloud::Resource::recv() throw(Exception){

	if(m_currentBlock == 0){

		if(m_channel->bytesAvailable() < (qint64)sizeof(m_bytesCounter)){
			return;
		}
		QDataStream stream(m_channel);
		stream >> m_bytesCounter;
		m_currentBlock++;
		m_total = m_bytesCounter;
		qDebug() << "Dimensione ricevuta: " << m_bytesCounter << " bytes";
	}else{
		emit transmissionRate(m_total, m_total - m_channel->bytesAvailable(), Download);

		qDebug() << "<< RICEVUTI : " << m_channel->bytesAvailable() << " bytes";
		m_bytesCounter -= m_zipFile->write(m_channel->readAll());
		qDebug() << ">> RIMANENTI: " << m_bytesCounter << " bytes";

		if(m_bytesCounter == 0){
			m_zipFile->close();
			m_zipFile->rename(getZipPath());
			leaveSocket();
			emit objectReceived();
		}else if(m_bytesCounter < 0){

			throw UntrustedBytesCounter();
		}
	}
}

void KCloud::Resource::behaviorOnSend(const qint64 dim) throw(Exception){

	m_transmitted += dim;
	emit transmissionRate(m_total, m_transmitted, Upload);
	m_bytesCounter -= dim;
	if(m_bytesCounter == 0){

		m_currentBlock++;
		if(m_currentBlock == m_packets + 2){
			m_zipFile->close();
			leaveSocket();
			emit objectSended();
		}else{
			emit changeBlock(m_currentBlock);
		}

	}else if(m_bytesCounter < 0){

		throw UntrustedBytesCounter();
	}
}

qint64 KCloud::Resource::calculateNetworkSize() throw(Exception){

	checkZip();
	return QFileInfo(getZipPath()).size();
}

void KCloud::Resource::setCompressed(){

	m_compressionFlag = true;
}

void KCloud::Resource::setNotCompressed(){

	m_compressionFlag = false;
}

bool KCloud::Resource::isCompressed() const{

	return m_compressionFlag;
}

void KCloud::Resource::checkResource() throw(Exception){
	if(!(QFileInfo(m_resourcePath).exists())){
		m_resourcePath.clear();
		throw BadPathException();
	}
}

void KCloud::Resource::checkDir() throw(Exception){
	if(!(QFileInfo(m_zipDir).exists()) || QFileInfo(m_zipDir).isFile()){
		m_zipDir.clear();
		throw BadPathException();
	}
}

void KCloud::Resource::checkZip() throw(Exception){

	QDir dir(getZipDir());

	checkDir();
	if(!dir.entryList().contains(getZipName())){

		throw AlreadyFileNameInUseException();
	}
}
