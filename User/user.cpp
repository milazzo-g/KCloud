#include "user.h"

User::User(QString nome)
{
	pippo = nome;
}

QString User::getName()
{
	return pippo;
}
