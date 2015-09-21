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
				UserBadMail

				//Resource

				//Database

				//varie ed eventuali l'importante è dargli un nome con il commmento
			};

			virtual const char *		what()		const throw ()	= 0;
			virtual	Type				type()		const			= 0;
			virtual ExceptionSubclass	subclass()	const			= 0;
			virtual Exception *			clone()		const;
			virtual void				raise()		const;
	};

	class UserException : public Exception{

		public:
			virtual ExceptionSubclass	subclass()	const;
	};

	class BadMailException : public UserException{

		public:
			virtual const char *		what()		const throw ();
			virtual	Type				type()		const;
	};


}
#endif // EXCEPTION_H
