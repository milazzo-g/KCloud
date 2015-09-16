#ifndef RESOURCES_H
#define RESOURCES_H

#include "resources_global.h"
#include <QByteArray>
#include <QDataStream>
#include <QException>
#include <QString>
#include <QFile>
#include <QMap>

namespace KCloud{

	class ResourcesManager;

	namespace Exceptions{

		class RESOURCESSHARED_EXPORT ResourceException : public QException{

			public:

				enum Type{
					UnknownException,
					BadPathException,
					EmptyOwnerException
				};

				virtual const char *		what()	const throw ();
				virtual	Type				type()	const;
				virtual ResourceException * clone() const;
				virtual void				raise() const;
		};

	}

	class RESOURCESSHARED_EXPORT ResourceHeader{

			friend class	 ResourcesManager;
			friend			QDataStream &operator <<(QDataStream &out, const ResourceHeader &res);
			friend			QDataStream &operator >>(QDataStream &inp, ResourceHeader &res);

		public:

			enum ResourceType{
				Dir,
				File
			};

			enum ResourcePerm{
				Read,
				Write,
				PermUndef
			};

											ResourceHeader(const QString owner) throw(Exceptions::ResourceException);
											ResourceHeader(const ResourceHeader &cpy);
			qint64							getNaturalSize() const;
			qint64							getCompressedSize() const;
			qint64							getNetworkSize() const;
			quint64							getId() const;
			quint64							getParent() const;
			QString							getName() const;
			QString							getOwner() const;
			QString							getSuffix() const;
			ResourceType					getType() const;
			ResourcePerm					getPermission(QString email) const;
			QMap<QString, ResourcePerm>		getPermissionTable() const;
			void							setOwner(const QString owner);
			bool							delPermission(const QString user);
			bool							addPermission(const QString user, const ResourcePerm perm);
			bool							modPermission(const QString user, const ResourcePerm perm);

		protected:

			qint64							naturalSize;
			qint64							compressedSize;
			quint64							id;
			quint64							parent;
			QString							name;
			QString							owner;
			ResourceType					basicType;
			QMap<QString, ResourcePerm>		permissionTable;

	};

	class RESOURCESSHARED_EXPORT Resource : ResourceHeader{

		public:
									Resource(const QString path, const QString owner) throw(Exceptions::ResourceException);
						bool		compress(QString destPath) throw(Exceptions::ResourceException);
						bool		unCompress(QString destPath) throw(Exceptions::ResourceException);
						bool		deleteTempFile();
						QFile *		getFile();
			virtual		bool		open() = 0;

		private:
			QFile *		file;
			QString		path;
			bool		compressionFlag;
	};

	QDataStream &operator <<(QDataStream &out, const ResourceHeader &res);
	QDataStream &operator >>(QDataStream &inp, ResourceHeader &res);
	QDataStream &operator <<(QDataStream &out, ResourceHeader::ResourceType &res);
	QDataStream &operator >>(QDataStream &inp, ResourceHeader::ResourceType &res);
	QDataStream &operator <<(QDataStream &out, ResourceHeader::ResourcePerm &res);
	QDataStream &operator >>(QDataStream &inp, ResourceHeader::ResourcePerm &res);
}



#endif // RESOURCES_H
