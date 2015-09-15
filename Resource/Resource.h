#ifndef RESOURCE_H
#define RESOURCE_H

#include "resource_global.h"
#include <QException>

namespace KCloud{

	class ResourceHeaderData;
	class ResourceData;
	class ResourceManager;

	class ResourceException : public QException{

		public:

			enum Type{
				ResourceBasicException
			};

			virtual const char *		what()	const throw ();
			virtual Type				type()	const;
			virtual void				raise() const;
			virtual ResourceException *	clone() const;
	};
}

#endif // RESOURCE_H
