#include "KDChartHeaderFooter.h"
#include "KDChartHeaderFooter_p.h"

#include "KDChartChart.h"
#include <KDChartTextAttributes.h>
#include <QFont>
#include <QPainter>
#include <QAbstractTextDocumentLayout>
#include <QTextDocumentFragment>
#include <QTextBlock>
#include <QtDebug>
#include <QLabel>
#include "KDTextDocument.h"

#include <KDABLibFakes>

using namespace KDChart;

HeaderFooter::Private::Private() :
    type( Header ),
    position( Position::North )
{
}

HeaderFooter::Private::~Private()
{
}

#define d d_func()

HeaderFooter::HeaderFooter( Chart* parent ) :
    TextArea( new Private() )
{
    setParent( parent );
    init();
}

HeaderFooter::~HeaderFooter()
{
    emit destroyedHeaderFooter( this );
}

void HeaderFooter::setParent( QObject* parent )
{
    QObject::setParent( parent );
    setParentWidget( qobject_cast<QWidget*>( parent ) );
    if( parent && ! autoReferenceArea() )
        setAutoReferenceArea( parent );
}

void HeaderFooter::init()
{
    TextAttributes ta;
    ta.setPen( QPen(Qt::black) );
    ta.setFont( QFont( QLatin1String( "helvetica" ), 10, QFont::Bold, false ) );

    Measure m( 35.0 );
    m.setRelativeMode( autoReferenceArea(), KDChartEnums::MeasureOrientationMinimum );
    ta.setFontSize( m );

    m.setValue( 8.0 );
    m.setCalculationMode( KDChartEnums::MeasureCalculationModeAbsolute );
    ta.setMinimalFontSize( m  );

    setTextAttributes( ta );
}

/**
  * Creates an exact copy of this header/footer.
  */
HeaderFooter * HeaderFooter::clone() const
{
    HeaderFooter* headerFooter = new HeaderFooter( new Private( *d ), 0 );
    headerFooter->setType( type() );
    headerFooter->setPosition( position() );
    headerFooter->setText( text() );
    headerFooter->setTextAttributes( textAttributes() );
    return headerFooter;
}

bool HeaderFooter::compare( const HeaderFooter& other )const
{
    return  (type()           == other.type()) &&
            (position()       == other.position()) &&
            // also compare members inherited from the base class:
            (autoReferenceArea() == other.autoReferenceArea()) &&
            (text()              == other.text()) &&
            (textAttributes()    == other.textAttributes());
}

void HeaderFooter::setType( HeaderFooterType type )
{
    d->type = type;
    emit positionChanged( this );
}

HeaderFooter::HeaderFooterType HeaderFooter::type() const
{
    return d->type;
}

void HeaderFooter::setPosition( Position position )
{
    d->position = position;
    emit positionChanged( this );
}

Position HeaderFooter::position() const
{
    return d->position;
}
