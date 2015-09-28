#ifndef GRAPHICRESOURCEHEADER_H
#define GRAPHICRESOURCEHEADER_H

#include "../ResourceHeader/ResourceHeader.h"

#include <QDir>
#include <QIcon>
#include <QFileInfo>
#include <QTreeWidget>
#include <QTreeWidgetItem>

namespace KCloud {
	class GraphicResourceHeader : public QTreeWidgetItem, public ResourceHeader{
		public:
			GraphicResourceHeader(const ResourceHeader &header, const GraphicResourceHeader * parent);
			GraphicResourceHeader(const ResourceHeader &header, QTreeWidget * view);

		signals:

		public slots:

		private:
			QIcon	getIcon(const QString &ext) const;
	};

}

#endif // GRAPHICRESOURCEHEADER_H
