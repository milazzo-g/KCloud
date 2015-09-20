#include "Resource.h"
#define COMPRESSED_EXT ".kcomp"

KCloud::Resource::Resource(QObject *parent)
{

}

KCloud::Resource::Resource(const QString &path, QObject *parent) : NetObject(parent){

	clear();
	setPath(path);
}

QString KCloud::Resource::getPath() const{

	return filePath;
}

void KCloud::Resource::setWorkingDir(const QString &path){

	QFileInfo info(path);
	if(info.exists() && info.isDir()){
		workingDir = path;
		return;
	}
	//lancia eccezione perchè il file non esiste
}

QString KCloud::Resource::getWorkingDir() const{

	return workingDir;
}

void KCloud::Resource::clear(){

	NetObject::clear();
}

void KCloud::Resource::prepareForSend(){

	NetObject::clear();
	compress();
	file = new QFile(getPath());
	file->open(QIODevice::ReadOnly);
	NetObject::prepareForSend();
}

void KCloud::Resource::prepareForRecv(){

}

void KCloud::Resource::compress(){

	checkFilePath();
	checkWorkingDir();
	QString newPath =  getWorkingDir() + QString("/") + QString(QFileInfo(getPath()).fileName() + COMPRESSED_EXT);

	if(QFileInfo(getPath()).isFile()){
		QFile::copy(getPath(), newPath);
	}else{

	}
	setPath(newPath);
	setCompressed();
}

void KCloud::Resource::send(const qint64 block){

}

void KCloud::Resource::recv(){

}

qint64 KCloud::Resource::calculateNetworkSize(){

	if(isCompressed()){
		return QFileInfo(getPath()).size();
	}
	// LANCIA ECCEZIONI
}

void KCloud::Resource::setCompressed(){

	compressionFlag = true;
}

void KCloud::Resource::setNotCompressed(){

	compressionFlag = false;
}

bool KCloud::Resource::isCompressed() const{

	return compressionFlag;
}

void KCloud::Resource::behaviorOnSend(const qint64 dim){

}

void KCloud::Resource::checkFilePath() const{
	if(!(QFileInfo(getPath()).exists())){
		//lancia eccezione perchè il file non esiste
	}
}

void KCloud::Resource::checkWorkingDir() const{

	QFileInfo info(getWorkingDir());
	if(info.exists() && info.isDir()){
		return;
	}
	//lancia eccezione perchè il file non esiste
}

void KCloud::Resource::setPath(const QString &path){

	if(!(QFileInfo(path).exists())){
		//lancia eccezione perchè il file non esiste
	}
	filePath = path;
}
