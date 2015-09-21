#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <QException>
#include <QString>

#include <c++/v1/type_traits>

namespace KCloud{

	class Exception : public QException{

		public:

			enum ExceptionSubclass{
				Basic,
				User,
				Resource,
				Database
				//varie ed eventuali
			};

			enum Type{
				//User
				UserBadMail,
				UserEmptyPassword,
				UserBadHashLength,

				//Resource
				ResourceEmptyPath,
				ResourceBadPath,

				//Database

				//varie ed eventuali l'importante è dargli un nome con il commmento
				Unknown,
				Core
			};

			virtual const char *		what()		const throw ();
			virtual	Type				type()		const;
			virtual ExceptionSubclass	subclass()	const;
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

	class EmptyPasswordException : public UserException{

		public:
			virtual const char *		what()		const throw ();
			virtual	Type				type()		const;
	};

	class BadHashLengthException : public UserException{

		public:
			virtual const char *		what()		const throw ();
			virtual	Type				type()		const;
	};

	class UnknownException : public UserException{

		public:
			virtual const char *		what()		const throw ();
			virtual	Type				type()		const;
	};

	class ResourceException : public Exception{

		public:
			virtual ExceptionSubclass	subclass()	const;
	};

	class EmptyPathException : public ResourceException{

		public:
			virtual const char *		what()		const throw ();
			virtual	Type				type()		const;
	};

	class BadPathException : public ResourceException{

		public:
			virtual const char *		what()		const throw ();
			virtual	Type				type()		const;
	};

}
#endif // EXCEPTION_H
