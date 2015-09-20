#include "Resource.h"

KCloud::Resource::Resource(const QString &path, QObject *parent) : NetObject(parent){

	clear();
	setPath(path);
}

QString KCloud::Resource::getPath() const{

	return filePath;
}

void KCloud::Resource::setPath(const QString &path){

	QFileInfo info(path);

}
