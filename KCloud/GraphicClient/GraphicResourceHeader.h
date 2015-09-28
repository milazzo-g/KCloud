#ifndef GRAPHICRESOURCEHEADER_H
#define GRAPHICRESOURCEHEADER_H

#include "../ResourceHeader/ResourceHeader.h"
#include "../User/User.h"

#include <QDir>
#include <QIcon>
#include <QPixmap>
#include <QFileInfo>
#include <QTreeWidget>
#include <QTreeWidgetItem>

namespace KCloud {
	class GraphicResourceHeader : public QTreeWidgetItem, public ResourceHeader{
		public:
			enum RootType{
				SessionUser,
				OtherUser,
				Public
			};

					GraphicResourceHeader(const ResourceHeader &header, QTreeWidget * view, const RootType type = SessionUser);
					GraphicResourceHeader(const ResourceHeader &header, GraphicResourceHeader * parent);
					~GraphicResourceHeader();

					QPixmap getImage();



		signals:

		public slots:

		private:
			QIcon	getIcon() const;
	};

}

#endif // GRAPHICRESOURCEHEADER_H
