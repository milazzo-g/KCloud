#include "Exceptions.h"

KCloud::Exception *KCloud::Exception::clone() const{
	return new Exception(*this);
}

void KCloud::Exception::raise() const{
	const throw *this;
}
