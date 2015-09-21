#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <QException>
#include <QString>

namespace KCloud{

	class Exception : public QException{

		public:

			enum ExceptionSubclass{
				UserException,
				ResourceException,
				DatabaseException
				//varie ed eventuali
			};

			enum Type{
				//User

				//Resource

				//Database

				//varie ed eventuali l'importante è dargli un nome con il commmento
			};

			virtual const char *		what()		const throw ()	= 0;
			virtual ExceptionSubclass	subclass()	const			= 0;
			virtual	Type				type()		const			= 0;
			virtual Exception *			clone()		const;
			virtual void				raise()		const;
	};
}
#endif // EXCEPTION_H
