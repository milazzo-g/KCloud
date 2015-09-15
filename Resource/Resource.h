#ifndef RESOURCE_H
#define RESOURCE_H

#include "resource_global.h"
#include <QException>

namespace KCloud{

	class ResourceHeaderData;
	class ResourceData;
	class ResourceManager;

	class ResourceException : public QException{
		typedef enum{
			ResourceBasicException
		}Type;
		virtual const char *		what()	const throw ();
		virtual Type				type()	const;
		virtual ResourceException *	clone() const;
		virtual void				raise() const;
	};
}

#endif // RESOURCE_H
