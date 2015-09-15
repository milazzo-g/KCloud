#ifndef USER_H
#define USER_H

#include "user_global.h"
#include <QException>

namespace KCloud {

	class UserData;
	class UserManager;

	/*!
	 * \brief The UserException class
	 */
	class USERSHARED_EXPORT UserException : public QException{

		public:

			typedef enum{
				UserBasicException,
				UserEmptyMailException,
				UserEmptyHashException
			}Type;

			/*!
			 * \brief what
			 * \return
			 */
			virtual const char *	what()	const throw ();
			/*!
			 * \brief type
			 * \return
			 */
			virtual	Type			type()	const;
			/*!
			 * \brief raise
			 */
			virtual void			raise() const;
			/*!
			 * \brief clone
			 * \return
			 */
			virtual UserException *	clone() const;
	};

	/*!
	 * \brief The User class
	 */
	class USERSHARED_EXPORT User{

		friend class UserManager;

		public:

			typedef enum{
				Hash,
				Password
			}Mode;

			/*!
			 * \brief	Costruttore
			 * \param	_email
			 * \param	_pass
			 * \param	_mode
			 */
			User(const QString _email, const QString _pass, Mode _mode = Password) throw (UserException);

			/*!
			 * \brief	getMail
			 * \return	L'email dell'utente.
			 */
			QString getMail() const;

			/*!
			 * \brief	getHash
			 * \return	L'hash della password.
			 */
			QString getHash() const;

			/*!
			 * \brief	getOccupiedSpace
			 * \return	Lo spazio occupato dall'utente.
			 */
			qint64	getOccupiedSpace() const;

			/*!
			 * \brief	getFreeSpace
			 * \return	Lo spazio libero dell'utente.
			 */
			qint64	getFreeSpace() const;

			/*!
			 * \brief	isLogged
			 * \return	True se l'utente è collegato, false altrimenti.
			 */
			bool	isLogged() const;



		protected:

		private:
			UserData * const data;
	};
}

#endif // USER_H
