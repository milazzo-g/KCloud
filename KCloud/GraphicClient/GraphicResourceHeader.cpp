#include "GraphicResourceHeader.h"

#include <QDebug>

KCloud::GraphicResourceHeader::GraphicResourceHeader(const KCloud::ResourceHeader &header, QTreeWidget *view,
													 const KCloud::GraphicResourceHeader::RootType type) :
QTreeWidgetItem(view), ResourceHeader(header){

	setText(0, getName());
	setText(1, "");

	switch (type) {
		case SessionUser:
			setIcon(0, QIcon(":/icons/icons/home.png"));
			break;
		case OtherUser:
			setIcon(0, QIcon(":/icons/icons/user.png"));
			break;
		case Public:
			setIcon(0, QIcon(":/icons/icons/public.png"));
			break;
		default:

			break;
	}
}

KCloud::GraphicResourceHeader::GraphicResourceHeader(const KCloud::ResourceHeader &header,
													 KCloud::GraphicResourceHeader *parent) :
QTreeWidgetItem(reinterpret_cast<QTreeWidgetItem *>(parent)), ResourceHeader(header){

	setText(0, getName());
	setText(1, header.getType() == Dir ? "" : QString::number(getSize()));
	setIcon(0, getIcon());
}

KCloud::GraphicResourceHeader::~GraphicResourceHeader(){

}

KCloud::ResourceHeader KCloud::GraphicResourceHeader::getHeader() const{

	return ResourceHeader::getThis();
}

QPixmap KCloud::GraphicResourceHeader::getImage() const{

	return icon(0).pixmap(64, 64);
}

QIcon KCloud::GraphicResourceHeader::getIcon() const{

	QDir dir(":/fileExtensionIcon/fileExtensionIcon/");
	QString	ext = QFileInfo(getName()).completeSuffix();

	if(getType() == ResourceHeader::Dir){
		return QIcon(":/fileExtensionIcon/fileExtensionIcon/dir.png");
	}

	foreach (QFileInfo f, dir.entryInfoList()) {
		if(f.baseName() == ext){
			return QIcon(f.filePath());
		}
	}
	return QIcon(":/icons/icons/unknown.png");
}
