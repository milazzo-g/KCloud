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
				UnknownException
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

											ResourceHeader(const QString path, const QString owner) throw(ResourceException);
											ResourceHeader(const ResourceHeader &cpy);

			qint64							getCompressedSize() const;
			qint64							getNaturalSize() const;
			quint64							getParent() const;
			quint64							getId() const;
			QString							getName() const;
			QString							getOwner() const;
			QString							getSuffix() const;
			ResourceType					getType() const;
			ResourcePerm					getPermission(QString email) const;
			QMap<QString, ResourcePerm>		getPermissionTable() const;

			void							setOwner(const QString owner);
			bool							updatePath(const QString path);
			bool							addPermission(const QString user, const ResourcePerm perm);
			bool							delPermission(const Qstring user);
			bool							modPermission(const QString user, const ResourcePerm perm);


		protected:

			quint64							id;
			QString							name;
			ResourceType					basicType;
			qint64							naturalSize;
			qint64							compressedSize;
			QMap<QString, ResourcePerm>		permissionTable;
			quint64							parent;
			QString							owner;


		private:


	};
}



#endif // RESOURCES_H
