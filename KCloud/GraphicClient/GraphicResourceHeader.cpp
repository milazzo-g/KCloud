#include "GraphicResourceHeader.h"


KCloud::GraphicResourceHeader::GraphicResourceHeader(const KCloud::ResourceHeader &header, QTreeWidget *view) : QTreeWidgetItem(view), ResourceHeader(header){

	QDir dir(":/");
	QFileInfo info(getName());
	setText(0, getName());
	setText(1, QString::number(getSize()));
	setIcon(0, getIcon(info.completeSuffix()));

}

QIcon KCloud::GraphicResourceHeader::getIcon(const QString &ext) const{

	QDir dir(":/");

	foreach (QFileInfo f, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::Files)) {
		if(f.baseName() == ext){
			return QIcon(f.filePath());
		}
	}
	return QIcon();
}
