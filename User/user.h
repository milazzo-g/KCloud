#ifndef USER_H
#define USER_H

#include "user_global.h"
#include <QString>

class USERSHARED_EXPORT User
{

	public:
		User(QString nome);
		QString getName();

	private:
		QString pippo;
};

#endif // USER_H
