#ifndef KDCHART1_EXPORT_H
#define KDCHART1_EXPORT_H

#include <qglobal.h>
#if defined(Q_OS_WIN32) && defined(KDCHART1_DLL)
#define DLL_EXPORT __declspec(dllexport)
#define DLL_IMPORT __declspec(dllimport)
#else
#define DLL_EXPORT
#define DLL_IMPORT
#endif

#if defined(KDCHART1_DLL)
#define KDCHART1_EXPORT DLL_EXPORT
#else
#define KDCHART1_EXPORT DLL_IMPORT
#endif

#endif

/*
   How to make a KD Chart DLL rather than linking statically:

   1. We have an  #include <kdchart1_export.h>  statement in all
      of our public KD Chart header files

   2. We have the  KDCHART1_EXPORT  macro added to all of our
      public classes.

      So e.g. the class header reads:

          class KDCHART1_EXPORT KDChart1Widget : public QWidget
          {

          }


   3. In order to create a DLL
      just add "CONFIG += dll" to the qmake command line or to the [KD Chart directory]/src/src.pro file
*/
