#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include "exceptions_global.h"
#include <QException>
#include <QString>

namespace KCloud{

	class EXCEPTIONSSHARED_EXPORT Exception : public QException{

		public:

			enum ExceptionSubclass{
				Basic,
				User,
				Resource,
				NetObject,
				Database,
				Client,
				MainServer,
				WorkerServer
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
				DatabaseNotOpen,
				DatabaseQueryError,
				DatabaseMultipleRowsForPrimaryKey,
				//Client
				ClientNullUserPointer,
				ClientUnreachableServer,

				//MainServer

				//WorkerServer

				//varie ed eventuali l'importante è dargli un nome con il commmento
				Core,
				Unknown,
				CorruptPacket
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
	class EXCEPTIONSSHARED_EXPORT UserException : public Exception{

		public:
			virtual ExceptionSubclass	subclass()	const;
	};

	class EXCEPTIONSSHARED_EXPORT BadMailException : public UserException{

		public:
			virtual const char *		what()		const throw ();
			virtual	Type				type()		const;
	};

	class EXCEPTIONSSHARED_EXPORT EmptyPasswordException : public UserException{

		public:
			virtual const char *		what()		const throw ();
			virtual	Type				type()		const;
	};

	class EXCEPTIONSSHARED_EXPORT BadHashLengthException : public UserException{

		public:
			virtual const char *		what()		const throw ();
			virtual	Type				type()		const;
	};

	/*
	 *		Eccezioni risorse
	 *
	*/
	class EXCEPTIONSSHARED_EXPORT ResourceException : public Exception{

		public:
			virtual ExceptionSubclass	subclass()	const;
	};

	class EXCEPTIONSSHARED_EXPORT EmptyPathException : public ResourceException{

		public:
			virtual const char *		what()		const throw ();
			virtual	Type				type()		const;
	};

	class EXCEPTIONSSHARED_EXPORT BadPathException : public ResourceException{

		public:
			virtual const char *		what()		const throw ();
			virtual	Type				type()		const;
	};

	class EXCEPTIONSSHARED_EXPORT AlreadyFileNameInUseException : public ResourceException{

		public:
			virtual const char *		what()		const throw ();
			virtual	Type				type()		const;
	};

	class EXCEPTIONSSHARED_EXPORT ZippingErrorException : public ResourceException{

		public:
			virtual const char *		what()		const throw ();
			virtual	Type				type()		const;
	};

	class EXCEPTIONSSHARED_EXPORT UnZippingErrorException : public ResourceException{

		public:
			virtual const char *		what()		const throw ();
			virtual	Type				type()		const;
	};

	class EXCEPTIONSSHARED_EXPORT NotReadyException : public ResourceException{

		public:
			virtual const char *		what()		const throw ();
			virtual	Type				type()		const;
	};

	/*
	 *		Eccezioni NetObject
	 *
	*/

	class EXCEPTIONSSHARED_EXPORT NetObjectException : public Exception{

		public:
			virtual ExceptionSubclass	subclass()	const;
	};

	class EXCEPTIONSSHARED_EXPORT InvalidSocket : public NetObjectException{

		public:
			virtual const char *		what()		const throw ();
			virtual	Type				type()		const;
	};

	class EXCEPTIONSSHARED_EXPORT UntrustedBytesCounter : public NetObjectException{

		public:
			virtual const char *		what()		const throw ();
			virtual	Type				type()		const;
	};

	/*
	 *		Eccezioni base
	 *
	*/
	class EXCEPTIONSSHARED_EXPORT UnknownException : public Exception{

		public:
			virtual ExceptionSubclass	subclass()	const;
			virtual const char *		what()		const throw ();
			virtual	Type				type()		const;
	};

	class EXCEPTIONSSHARED_EXPORT CorruptPacketException : public Exception{

		public:
			virtual ExceptionSubclass	subclass()	const;
			virtual const char *		what()		const throw ();
			virtual	Type				type()		const;
	};

	/*
	 *		Eccezioni Database
	 *
	 */

	class EXCEPTIONSSHARED_EXPORT DatabaseException : public Exception{

		public:
			virtual ExceptionSubclass	subclass()	const;
	};

	class EXCEPTIONSSHARED_EXPORT OpenFailure : public DatabaseException{

		public:
			virtual const char *		what()		const throw ();
			virtual	Type				type()		const;
	};

	class EXCEPTIONSSHARED_EXPORT QueryFailure : public DatabaseException{

		public:
			virtual const char *		what()		const throw ();
			virtual	Type				type()		const;
	};

	class EXCEPTIONSSHARED_EXPORT MultipleRowsForPrimaryKey : public DatabaseException{

		public:
			virtual const char *		what()		const throw ();
			virtual	Type				type()		const;
	};

	/*
	 *		Eccezioni Client
	 *
	 */
	class EXCEPTIONSSHARED_EXPORT ClientException : public Exception{

		public:
			virtual ExceptionSubclass	subclass()	const;
	};

	class EXCEPTIONSSHARED_EXPORT NullUserPointer : public ClientException{

		public:
			virtual const char *		what()		const throw ();
			virtual	Type				type()		const;
	};

	class EXCEPTIONSSHARED_EXPORT UnreachableServer : public ClientException{

		public:
			virtual const char *		what()		const throw ();
			virtual	Type				type()		const;
	};
}

#endif // EXCEPTIONS_H
