#ifndef CARD_GLOBAL_H
#define CARD_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CARD_LIBRARY)
#  define CARDSHARED_EXPORT Q_DECL_EXPORT
#else
#  define CARDSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // CARD_GLOBAL_H
