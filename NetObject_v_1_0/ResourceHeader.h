#ifndef RESOURCEHEADER_H
#define RESOURCEHEADER_H

#include <QMap>
#include <QObject>
#include <QString>
#include <QDataStream>

namespace KCloud{

	class ResourceManager;

    class ResourceHeader : public QObject{
        Q_OBJECT

		friend class ResourceManager;
		friend QDataStream &operator<<(QDataStream &out, const ResourceHeader &tmp);
		friend QDataStream &operator>>(QDataStream &inp, ResourceHeader &tmp);

		public:
			enum ResourcePerm{
				Read,
				Write,
				PermUndef
			};


			explicit						ResourceHeader(QObject *parent = 0);

		signals:

		public slots:

		private:
			qint64							m_size;
			quint64							m_id;
			quint64							m_parentId;
			QString							m_owner;
			ResourcePerm					m_publicPerm;
			QMap<QString, ResourcePerm>		m_permissionTable;


    };

	QDataStream &operator<<(QDataStream &out, const ResourceHeader &tmp);
	QDataStream &operator>>(QDataStream &inp, ResourceHeader &tmp);
	QDataStream &operator<<(QDataStream &out, const ResourceHeader::ResourcePerm &tmp);
	QDataStream &operator>>(QDataStream &inp, ResourceHeader::ResourcePerm &tmp);

}

#endif // RESOURCEHEADER_H
