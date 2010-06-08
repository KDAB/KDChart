#include <KDChartHeaderFooterPropertiesWidget.h>
#include <KDChartHeaderFooterPropertiesWidget_p.h>

#include <QWidget>
#include <QDebug>
#include <QLabel>

#include <KDChartPosition.h>
#include <KDChartHeaderFooter.h>


#define d d_func()

using namespace KDChart;

HeaderFooterPropertiesWidget::Private::Private()
    :headerfooter(0), instantApply(true)
{

}

HeaderFooterPropertiesWidget::Private::~Private()
{
}

HeaderFooterPropertiesWidget::HeaderFooterPropertiesWidget( QWidget *parent )
    :QWidget( parent ), _d( new Private )
{
    setupUi( this );

    const QStringList labels = KDChart::Position::printableNames();
    const QList<QByteArray> names = KDChart::Position::names();

    for ( int i = 0, end = qMin( labels.size(), names.size() ) ; i != end ; ++i )
        mPositionCombo->addItem( labels[i], names[i] );

    connect( mPositionCombo, SIGNAL( activated( int ) ),
             this, SLOT( slotPositionChanged( int ) ) );
    connect( headerRB,  SIGNAL( toggled( bool ) ),
             this,  SLOT( slotTypeChanged( bool ) ) );
    connect( textED,  SIGNAL( textChanged( const QString ) ),
             this,  SLOT( slotTextChanged( const QString  ) ) );

    setEnabled( false );

}

HeaderFooterPropertiesWidget::~HeaderFooterPropertiesWidget()
{

}

void HeaderFooterPropertiesWidget::setHeaderFooter( HeaderFooter * hf )
{
     d->headerfooter = hf;
    if ( hf ) {
        readFromHeaderFooter( hf );
        setEnabled( true );
    } else {
        setEnabled( false);
    }
}

void HeaderFooterPropertiesWidget::setInstantApply( bool value )
{
    d->instantApply = value;
}


void HeaderFooterPropertiesWidget::readFromHeaderFooter( const HeaderFooter * hf  )
{
    mPositionCombo->setCurrentIndex( mPositionCombo->findData( QByteArray( hf->position().name() ) ) );
    textED->setText( hf->text() );
    if (  hf->type() == HeaderFooter::Header )
        headerRB->setChecked( true );
    else
        footerRB->setChecked( true );
}


void HeaderFooterPropertiesWidget::writeToHeaderFooter( HeaderFooter * hf )
{
    if ( !hf ) return;
    hf->setPosition( Position::fromName( mPositionCombo->itemData( mPositionCombo->currentIndex() ).toByteArray() ) );
    hf->setText( textED->text() );
    if (  headerRB->isChecked() )
        hf->setType( HeaderFooter::Header );
    else
        hf->setType( HeaderFooter::Footer );
}

void HeaderFooterPropertiesWidget::slotPositionChanged( int idx )
{
    if ( d->headerfooter && d->instantApply ) {
        d->headerfooter->setPosition( Position::fromName( mPositionCombo->itemData( idx ).toByteArray() ) );
    } else {
        emit changed();
    }
}


void HeaderFooterPropertiesWidget::slotTextChanged( const QString& text )
{
    Q_UNUSED( text );

    if ( d->headerfooter && d->instantApply ) {
        d->headerfooter->setText( textED->text() );
    } else {
        emit changed();
    }
}

void HeaderFooterPropertiesWidget::slotTypeChanged( bool toggled )
{
    Q_UNUSED( toggled );

    if ( d->headerfooter && d->instantApply ) {
        if (  headerRB->isChecked() )
            d->headerfooter->setType( HeaderFooter::Header );
        else
            d->headerfooter->setType( HeaderFooter::Footer );
    } else {
        emit changed();
    }
}

