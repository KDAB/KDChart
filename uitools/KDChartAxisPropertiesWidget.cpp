#include <KDChartAxisPropertiesWidget.h>
#include <KDChartAxisPropertiesWidget_p.h>

#include <QWidget>
#include <QDebug>
#include <QLabel>

#include <KDChartPosition.h>
#include <KDChartCartesianAxis.h>



#define d d_func()

using namespace KDChart;

AxisPropertiesWidget::Private::Private()
    :axis(0), instantApply(true)
{

}

AxisPropertiesWidget::Private::~Private()
{
}


AxisPropertiesWidget::AxisPropertiesWidget( QWidget *parent )
    :QWidget( parent ),  _d( new Private )
{
    setupUi( this ),

    connect( mPositionCombo, SIGNAL( activated( int ) ),
             this, SLOT( slotPositionChanged( int ) ) );
    connect( titleED,  SIGNAL( textChanged( const QString ) ),
             this,  SLOT( slotTitleChanged( const QString  ) ) );

    setEnabled( false );
}

AxisPropertiesWidget::~AxisPropertiesWidget()
{

}

void AxisPropertiesWidget::setAxis( CartesianAxis * a )
{
    d->axis = a;
    if ( a ) {
        readFromAxis( a );
        setEnabled( true );
    } else {
        setEnabled( false);
    }
}

void AxisPropertiesWidget::setInstantApply( bool value )
{
    d->instantApply = value;
}

void AxisPropertiesWidget::readFromAxis( const CartesianAxis * a  )
{
    mPositionCombo->setCurrentIndex( a->position() );
    titleED->setText( a->titleText() );
}


void AxisPropertiesWidget::writeToAxis( CartesianAxis * a )
{
    KDChart::CartesianAxis::Position pos =
        static_cast<KDChart::CartesianAxis::Position>(mPositionCombo->currentIndex());

    if ( !a ) return;
    a->setPosition( pos );
    a->setTitleText( titleED->text() );
}

void AxisPropertiesWidget::slotPositionChanged( int idx )
{
     KDChart::CartesianAxis::Position pos =
         static_cast<KDChart::CartesianAxis::Position>(idx);

    if ( d->axis && d->instantApply ) {
        d->axis->setPosition( pos );
    } else {
        emit changed();
    }
}


void AxisPropertiesWidget::slotTitleChanged( const QString& text )
{
    Q_UNUSED( text );

    if ( d->axis && d->instantApply ) {
        d->axis->setTitleText( titleED->text() );
    } else {
        emit changed();
    }
}
