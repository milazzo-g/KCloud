#ifndef RESOURCES_H
#define RESOURCES_H

#include "resources_global.h"
#include <QException>
#include <QString>
#include <QMap>


namespace KCloud{

	class ResourcesManager;

	class RESOURCESSHARED_EXPORT ResourceException : public QException{

		public:

			enum Type{
				UnknownException,
				BadPathException
			};

			virtual const char *		what()	const throw ();
			virtual	Type				type()	const;
			virtual ResourceException * clone() const;
			virtual void				raise() const;
	};

	class RESOURCESSHARED_EXPORT ResourceHeader{

			friend class ResourcesManager;

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

											ResourceHeader(const ResourceHeader &cpy);

			qint64							getNaturalSize() const;
			qint64							getCompressedSize() const;
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

		private:


	};
}



#endif // RESOURCES_H
