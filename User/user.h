#ifndef USER_H
#define USER_H

#include "user_global.h"
#include <QException>
#include <QString>
#include <QDataStream>

namespace KCloud{

	class UserManager;

	namespace Exceptions{
		class USERSHARED_EXPORT UserException : public QException{
			public:
				enum Type{
					UnknownException,
					EmailPatternMismatch,
					EmptyEmail,
					EmptyPassword,
					HashToLong,
					NegativeSpace
				};
				virtual const char *	what()	const throw ();
				virtual	Type			type()	const;
				virtual UserException * clone() const;
				virtual void			raise() const;
		};
	}

	class USERSHARED_EXPORT User{

		friend class	UserManager;
		friend			QDataStream &operator <<(QDataStream &out, const User &usr);
		friend			QDataStream &operator >>(QDataStream &inp, User &usr);

		public:

			enum Mode{
				Encrypt,
				NotEncrypt
			};

					User(const QString email, const QString pass, Mode mode = Encrypt)	throw (Exceptions::UserException);
					User(const User &cpy);

			QString getEmail()			const;
			QString getHash()			const;
			qint64	getFreeSpace()		const;
			qint64	getNetworkSize()	const;
			qint64	getOccupiedSpace()	const;
			bool	isLogged()			const;

			void	setHash(const QString pass, Mode mode = Encrypt)					throw (Exceptions::UserException);
			void	setEmail(const QString email)										throw (Exceptions::UserException);
			void	setLogged();
			void	setUnlogged();

		private:
							QString m_email;
							QString m_hash;
							qint64	m_space;
							bool	m_status;

			static	const	qint64	totalSpace;
			static	const	QString emailRegEx;
	};

	QDataStream &operator <<(QDataStream &out, const User &usr);
	QDataStream &operator >>(QDataStream &inp, User &usr);
	QDataStream &operator <<(QDataStream &out, User::Mode &mod);
	QDataStream &operator >>(QDataStream &inp, User::Mode &mod);
}


#endif // USER_H
