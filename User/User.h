#ifndef USER_H
#define USER_H

#include "user_global.h"
#include <QException>

namespace KCloud {

	class UserData;
	class UserManager;

	class USERSHARED_EXPORT UserException : public QException{

		public:

			enum Type{
				UserBasicException,
				UserEmptyMailException,
				UserEmptyHashException,
				UserEmailPatternMismatchException
			};

			virtual const char *	what()	const throw ();
			virtual	Type			type()	const;
			virtual void			raise() const;
			virtual UserException *	clone() const;
	};

	class USERSHARED_EXPORT User{

		friend class UserManager;

		public:

			typedef enum{
				Hash,
				Password
			}Mode;

			User(const QString &email, const QString &pass, Mode mode = Password) throw (UserException);
			~User();

			QString getMail() const;
			QString getHash() const;
			qint64	getOccupiedSpace() const;
			qint64	getFreeSpace() const;
			bool	isLogged() const;

			void	setLogged();
			void	setUnlogged();

		private:
			UserData * const data;
	};
}

#endif // USER_H
