#ifndef MIAMODULES_GLOBAL_H
#define MIAMODULES_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MIACOREMODULES_LIBRARY)
#  define MIACOREMODULESSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MIACOREMODULESSHARED_EXPORT Q_DECL_IMPORT
#endif

#if defined(MIAVIEWMODULES_LIBRARY)
#  define MIAVIEWMODULESSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MIAVIEWMODULESSHARED_EXPORT Q_DECL_IMPORT
#endif

#if defined(MIAMARKERS_LIBRARY)
#  define MIAMARKERSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MIAMARKERSSHARED_EXPORT Q_DECL_IMPORT
#endif

#if defined(MIACOMMONMODULES_LIBRARY)
#  define MIACOMMONMODULESSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MIACOMMONMODULESSHARED_EXPORT Q_DECL_IMPORT
#endif

#if defined(MIALEVELSETMODULES_LIBRARY)
#  define MIALEVELSETMODULESSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MIALEVELSETMODULESSHARED_EXPORT Q_DECL_IMPORT
#endif

#if defined(MIAMACHINELEARNINGMODULES_LIBRARY)
#  define MIAMACHINELEARNINGMODULESSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MIAMACHINELEARNINGMODULESSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MIAMODULES_GLOBAL_H