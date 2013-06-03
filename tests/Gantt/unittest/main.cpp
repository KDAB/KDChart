/****************************************************************************
** Copyright (C) 2001-2013 Klaralvdalens Datakonsult AB.  All rights reserved.
**
** This file is part of the KD Chart library.
**
** Licensees holding valid commercial KD Chart licenses may use this file in
** accordance with the KD Chart Commercial License Agreement provided with
** the Software.
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 and version 3 as published by the
** Free Software Foundation and appearing in the file LICENSE.GPL.txt included.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** Contact info@kdab.com if any conditions of this licensing are not
** clear to you.
**
**********************************************************************/

#include "../../../src/KDGantt/unittest/testregistry.h"

#include <QApplication>
#include <iostream>

int main( int argc , char ** argv ) {

  QApplication app( argc, argv );

  KDAB::UnitTest::Runner r;
  unsigned int failed = 0;
  if ( argc == 1 )
    failed = r.run();
  else {
    for ( int i = 1 ; i < argc ; ++i )
      if ( argv[i] && *argv[i] )
        failed += r.run( argv[i] );
      else {
        std::cerr << argv[0] << ": skipping empty group name" << std::endl;
      }
  }
  std::cout << failed << " tests failed." << std::endl;
  return failed;
}
