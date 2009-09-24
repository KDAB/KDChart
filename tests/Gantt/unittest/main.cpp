#include "../../src/unittest/testregistry.h"

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
      else{
        std::cerr << argv[0] << ": skipping empty group name" << std::endl;
      }
  }
  std::cout << failed << " tests failed." << std::endl;
  return failed;
}
