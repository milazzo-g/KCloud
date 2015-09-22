#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <QException>
#include <QString>

namespace KCloud{

	class Exception : public QException{

		public:

			enum ExceptionSubclass{
				Basic,
				User,
				Resource,
				NetObject,
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
				ResourceAlreadyFileNameInUse,
				ResourceZippingError,
				ResourceUnZippingError,
				ResourceNotReady,

				//NetObject
				NetObjectInvalidSocket,
				NetObjectUntrustedBytesCounter,
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

	/*
	 *		Eccezioni utente
	 *
	*/
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

	/*
	 *		Eccezioni risorse
	 *
	*/
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

	class AlreadyFileNameInUseException : public ResourceException{

		public:
			virtual const char *		what()		const throw ();
			virtual	Type				type()		const;
	};

	class ZippingErrorException : public ResourceException{

		public:
			virtual const char *		what()		const throw ();
			virtual	Type				type()		const;
	};

	class UnZippingErrorException : public ResourceException{

		public:
			virtual const char *		what()		const throw ();
			virtual	Type				type()		const;
	};

	class NotReadyException : public ResourceException{

		public:
			virtual const char *		what()		const throw ();
			virtual	Type				type()		const;
	};

	/*
	 *		Eccezioni NetObject
	 *
	*/

	class NetObjectException : public Exception{

		public:
			virtual ExceptionSubclass	subclass()	const;
	};

	class InvalidSocket : public NetObjectException{

		public:
			virtual const char *		what()		const throw ();
			virtual	Type				type()		const;
	};

	class UntrustedBytesCounter : public NetObjectException{

		public:
			virtual const char *		what()		const throw ();
			virtual	Type				type()		const;
	};

	/*
	 *		Eccezioni base
	 *
	*/
	class UnknownException : public Exception{

		public:
			virtual ExceptionSubclass	subclass()	const;
			virtual const char *		what()		const throw ();
			virtual	Type				type()		const;
	};

}
#endif // EXCEPTION_H
