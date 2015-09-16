#ifndef USER_GLOBAL_H
#define USER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(USER_LIBRARY)
#  define USERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define USERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // USER_GLOBAL_H
