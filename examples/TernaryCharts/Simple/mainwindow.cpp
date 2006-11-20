#include "mainwindow.h"

MainWindow::MainWindow( QWidget* parent )
    : QDialog( parent )
    , m_chart( 0 )
    , m_diagram( 0 )
    , m_polarPlane( 0 )
{
    setupUi( this );
}
