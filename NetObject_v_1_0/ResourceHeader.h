#ifndef RESOURCEHEADER_H
#define RESOURCEHEADER_H

#include <QMap>
#include <QObject>
#include <QString>
#include <QDataStream>
#include "User.h"

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


			explicit								ResourceHeader(QObject *parent = 0);
													ResourceHeader(const QString &path,
																   const User &sessionUser,
																   const quint64 &parentId,
																   const QMap<QString, ResourcePerm> &permissionTable = QMap<QString, ResourcePerm>(),
																   ResourcePerm publicPerm = PermUndef);

						void						setParentId(const quint64 &id);
						void						setOwner(const QString &owner);
						void						setPublicPerm(ResourcePerm publicPerm = PermUndef);
						void						setPermissionTable(const QMap<QString, ResourcePerm>	&permissioTable = QMap<QString, ResourcePerm>());
						bool						addPermission(const QString &mail, ResourcePerm perm);
						bool						modPermission(const QString &mail, ResourcePerm perm);
						bool						delPermission(const QString &mail);
						bool						setPublicPermission(ResourcePerm perm = PermUndef);
						qint64						getSize() const;
						quint64						getId() const;
						quint64						getParentId() const;
						QString						getOwner() const;
						ResourcePerm				getPublicPermission();
						ResourcePerm				getPermission(const QString &mail);
						QMap<QString, ResourcePerm>	getPermissionTable() const;


		signals:

		public slots:

		private:
						qint64						m_size;
						quint64						m_id;
						quint64						m_parentId;
						QString						m_owner;
						ResourcePerm				m_publicPerm;
						QMap<QString, ResourcePerm>	m_permissionTable;


    };

	QDataStream &operator<<(QDataStream &out, const ResourceHeader &tmp);
	QDataStream &operator>>(QDataStream &inp, ResourceHeader &tmp);
	QDataStream &operator<<(QDataStream &out, const ResourceHeader::ResourcePerm &tmp);
	QDataStream &operator>>(QDataStream &inp, ResourceHeader::ResourcePerm &tmp);

}

#endif // RESOURCEHEADER_H
