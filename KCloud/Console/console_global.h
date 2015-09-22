#ifndef CONSOLE_GLOBAL_H
#define CONSOLE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CONSOLE_LIBRARY)
#  define CONSOLESHARED_EXPORT Q_DECL_EXPORT
#else
#  define CONSOLESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // CONSOLE_GLOBAL_H
