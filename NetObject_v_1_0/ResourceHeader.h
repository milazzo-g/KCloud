#ifndef RESOURCEHEADER_H
#define RESOURCEHEADER_H

#include <QMap>
#include <QDir>
#include <QObject>
#include <QString>
#include <QFileInfo>
#include <QDataStream>
#include <QFileInfoList>

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

			enum ResourceType{
				Dir,
				File
			};

			explicit								ResourceHeader(QObject *parent = 0);
													ResourceHeader(const QString &path,
																   const User &sessionUser,
																   const quint64 &parentId,
																   const QMap<QString, ResourcePerm> &permissionTable = QMap<QString, ResourcePerm>(),
																   ResourcePerm publicPerm = PermUndef,
																   QObject *parent = 0) throw (Exception);
						void						clear();
						void						setParentId(const quint64 &id);
						void						setOwner(const User &sessionUser);
						void						setPermissionTable(const QMap<QString, ResourcePerm> &permissioTable = QMap<QString, ResourcePerm>());
						bool						addPermission(const QString &mail, ResourcePerm perm) throw(Exception);
						bool						modPermission(const QString &mail, ResourcePerm perm) throw(Exception);
						bool						delPermission(const QString &mail) throw(Exception);
						void						setPublicPermission(ResourcePerm perm = PermUndef);
						qint64						getSize() const;
						quint64						getId() const;
						quint64						getParentId() const;
						QString						getOwner() const;
						ResourceType				getType() const;
						ResourcePerm				getPublicPermission();
						ResourcePerm				getPermission(const QString &mail) throw(Exception);
						QMap<QString, ResourcePerm>	getPermissionTable() const;

		signals:

		public slots:

		private:
						qint64						m_size;
						quint64						m_id;
						quint64						m_parentId;
						QString						m_owner;
						ResourcePerm				m_publicPerm;
						ResourceType				m_type;
						QMap<QString, ResourcePerm>	m_permissionTable;

						qint64						calculateDirSize(const QString &path);

    };

	QDataStream &operator<<(QDataStream &out, const ResourceHeader &tmp);
	QDataStream &operator>>(QDataStream &inp, ResourceHeader &tmp);
	QDataStream &operator<<(QDataStream &out, const ResourceHeader::ResourcePerm &tmp);
	QDataStream &operator>>(QDataStream &inp, ResourceHeader::ResourcePerm &tmp);
	QDataStream &operator<<(QDataStream &out, const ResourceHeader::ResourceType &tmp);
	QDataStream &operator>>(QDataStream &inp, ResourceHeader::ResourceType &tmp);

}

#endif // RESOURCEHEADER_H
