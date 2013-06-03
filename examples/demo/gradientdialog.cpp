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

#include "gradientdialog.h"
#include "ui_gradientdialog.h"

#include "colorslider.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpinBox>

#include <QLabel>


class GradientDialog::Private : public QObject
{
    Q_OBJECT
public:
    Private( GradientDialog *qq );
    ~Private();
    void init();
    ColorSlider *redSlider;
    ColorSlider *greenSlider;
    ColorSlider *blueSlider;
    QVector< QGradientStop > m_gradient;
    int m_currStopNr;
    Ui::GradientDialog *ui;
    GradientDialog *q;
public Q_SLOTS:
    void changedIndex( int index );
    void changeStopPosition( double value );
    void insertItem();
    void deleteItem();
    void resetColors();
    void updateGradientDisplay();
};

GradientDialog::Private::Private( GradientDialog *qq )
    : m_currStopNr( 2 )
    , ui( new Ui::GradientDialog )
    , q( qq )
{
    m_gradient << qMakePair( 0.0, QColor( Qt::red ) );
    m_gradient << qMakePair( 1.0, QColor( Qt::blue ) );
}

GradientDialog::Private::~Private()
{
    delete ui;
}

void GradientDialog::Private::changeStopPosition( double value )
{
    m_gradient[ ui->stopSelector->currentIndex() ].first = value;
}

void GradientDialog::Private::changedIndex( int index )
{
    if ( index >= 0 )
    {
        ui->stopPosition->setValue( m_gradient[ index ].first );
        QColor stopColor = m_gradient[ index ].second;
        redSlider->setValue( stopColor.red() );
        greenSlider->setValue( stopColor.green() );
        blueSlider->setValue( stopColor.blue() );
    }
}

void GradientDialog::Private::insertItem()
{
    QGradientStop newStop;
    newStop.first = 0.5;
    newStop.second = Qt::white;
    const int index = ui->stopSelector->currentIndex() + 1;
    ui->stopSelector->setCurrentIndex( - 1 );
    m_gradient.insert( index, newStop );


    QStringList newItems;
    newItems << tr( "stop %1" ).arg( m_currStopNr );
    ui->stopSelector->insertItems( index, newItems );

    ui->stopSelector->setCurrentIndex( index );

    updateGradientDisplay();

}

void GradientDialog::Private::deleteItem()
{

}

void GradientDialog::Private::updateGradientDisplay()
{
    QLinearGradient gradient;
    gradient.setStart( 0, 0 );
    gradient.setStart( 1, 0 );
    gradient.setCoordinateMode( QGradient::ObjectBoundingMode );
    Q_FOREACH( const QGradientStop &stop, m_gradient )
        gradient.setColorAt( stop.first, stop.second );
    QPalette palette = ui->gradientDisplay->palette();
    palette.setBrush( QPalette::Background, gradient );
    ui->gradientDisplay->setPalette( palette );
}

void GradientDialog::Private::init()
{
    ui->setupUi( q );
    QStringList list; list << tr( "stop1" ) << tr( "stop2" );
    ui->stopSelector->addItems( list );
    QHBoxLayout *redLayout = new QHBoxLayout;
    dynamic_cast< QVBoxLayout* >( ui->gradientStopBox->layout() )->addLayout( redLayout );
    QLabel *redLabel = new QLabel( "R" );
    QFont redFont( redLabel->font() );
    redFont.setUnderline( true );
    redLabel->setFont( redFont );
    redLayout->addWidget( redLabel );
    redSlider = new ColorSlider( q );
    redSlider->setStartColor( Qt::black );
    redSlider->setEndColor( Qt::red );
    QSpinBox *redSpin = new QSpinBox( q );
    redSpin->setMinimum( 0 );
    redSpin->setMaximum( 255 );
    redSpin->setAccelerated( true );
    redSpin->setValue( redSlider->value() );
    connect( redSpin, SIGNAL( valueChanged( int ) ), redSlider, SLOT( setValue( int ) ) );
    connect( redSlider, SIGNAL( valueChanged( int ) ), redSpin, SLOT( setValue( int ) ) );
    redLayout->addWidget( redSlider );
    redLayout->addWidget( redSpin );

    QHBoxLayout *greenLayout = new QHBoxLayout;
    dynamic_cast< QVBoxLayout* >( ui->gradientStopBox->layout() )->addLayout( greenLayout );
    QLabel *greenLabel = new QLabel( "G" );
    QFont greenFont( greenLabel->font() );
    greenFont.setUnderline( true );
    greenLabel->setFont( greenFont );
    greenLayout->addWidget( greenLabel );
    greenSlider = new ColorSlider( q );
    greenSlider->setStartColor( Qt::black );
    greenSlider->setEndColor( Qt::green );
    QSpinBox *greenSpin = new QSpinBox( q );
    greenSpin->setMinimum( 0 );
    greenSpin->setMaximum( 255 );
    greenSpin->setAccelerated( true );
    greenSpin->setValue( greenSlider->value() );
    connect( greenSpin, SIGNAL( valueChanged( int ) ), greenSlider, SLOT( setValue( int ) ) );
    connect( greenSlider, SIGNAL( valueChanged( int ) ), greenSpin, SLOT( setValue( int ) ) );
    greenLayout->addWidget( greenSlider );
    greenLayout->addWidget( greenSpin );

    QHBoxLayout *blueLayout = new QHBoxLayout;
    dynamic_cast< QVBoxLayout* >( ui->gradientStopBox->layout() )->addLayout( blueLayout );
    QLabel *blueLabel = new QLabel( "B" );
    QFont blueFont( blueLabel->font() );
    blueFont.setUnderline( true );
    blueLabel->setFont( blueFont );
    blueLayout->addWidget( blueLabel );
    blueSlider = new ColorSlider( q );
    blueSlider->setStartColor( Qt::black );
    blueSlider->setEndColor( Qt::blue );
    QSpinBox *blueSpin = new QSpinBox( q );
    blueSpin->setMinimum( 0 );
    blueSpin->setMaximum( 255 );
    blueSpin->setAccelerated( true );
    blueSpin->setValue( blueSlider->value() );
    connect( blueSpin, SIGNAL( valueChanged( int ) ), blueSlider, SLOT( setValue( int ) ) );
    connect( blueSlider, SIGNAL( valueChanged( int ) ), blueSpin, SLOT( setValue( int ) ) );
    blueLayout->addWidget( blueSlider );
    blueLayout->addWidget( blueSpin );

    updateGradientDisplay();

    connect( redSlider, SIGNAL( valueChanged( int ) ), this, SLOT( resetColors() ) );
    connect( greenSlider, SIGNAL( valueChanged( int ) ), this, SLOT( resetColors() ) );
    connect( blueSlider, SIGNAL( valueChanged( int ) ), this, SLOT( resetColors() ) );

    connect( ui->newStop, SIGNAL( clicked() ), this, SLOT( insertItem() ) );
    connect( ui->deleteStop, SIGNAL( clicked() ), this, SLOT( deleteItem() ) );
    connect( ui->stopSelector, SIGNAL( currentIndexChanged( int ) ), this, SLOT( changedIndex( int ) ) );

    connect( ui->stopPosition, SIGNAL( valueChanged( double ) ), this, SLOT( changeStopPosition( double ) ) );
}

GradientDialog::GradientDialog( QWidget *parent )
    : QDialog( parent )
    , d( new Private( this ) )
{
    d->init();
}

GradientDialog::~GradientDialog()
{
    delete d;
}

void GradientDialog::Private::resetColors()
{
    const int red = redSlider->value();
    const int green = greenSlider->value();
    const int blue = blueSlider->value();
    QColor redSliderStartColor( 0, green, blue );
    QColor redSliderStopColor( 255, green, blue );
    redSlider->setStartColor( redSliderStartColor );
    redSlider->setEndColor( redSliderStopColor );
    m_gradient[ ui->stopSelector->currentIndex() ].second.setRed( red );

    QColor greenSliderStartColor( red, 0, blue );
    QColor greenSliderStopColor( red, 255, blue );
    greenSlider->setStartColor( greenSliderStartColor );
    greenSlider->setEndColor( greenSliderStopColor );
    m_gradient[ ui->stopSelector->currentIndex() ].second.setGreen( green );

    QColor blueSliderStartColor( red, green, 0 );
    QColor blueSliderStopColor( red, green, 255 );
    blueSlider->setStartColor( blueSliderStartColor );
    blueSlider->setEndColor( blueSliderStopColor );
    m_gradient[ ui->stopSelector->currentIndex() ].second.setBlue( blue );

    updateGradientDisplay();

}

QGradient GradientDialog::gradient() const
{
    QLinearGradient gradient;
    gradient.setStart( 0, 0 );
    gradient.setStart( 1, 0 );
    gradient.setCoordinateMode( QGradient::ObjectBoundingMode );
    Q_FOREACH( const QGradientStop &stop, d->m_gradient )
        gradient.setColorAt( stop.first, stop.second );
    return gradient;
}
void GradientDialog::setGradient( const QGradient &gradient )
{
    d->m_gradient.clear();
    d->ui->stopSelector->clear();
    const QGradientStops stops = gradient.stops();
    Q_FOREACH( const QGradientStop &stop, stops )
    {
        d->m_gradient.append( stop );
    }
    QStringList newEntries;
    for ( int i = 0; i < stops.count(); ++i )
        newEntries << tr( "stop %1" ).arg( i );
    d->ui->stopSelector->addItems( newEntries );
    d->m_currStopNr = newEntries.count();
    d->updateGradientDisplay();
}

QGradient GradientDialog::getGradient( const QGradient &gradient, QWidget *parent, const QString &title )
{
    GradientDialog dialog( parent );
    dialog.setGradient( gradient );
    dialog.setWindowTitle( title );
    dialog.exec();
    return dialog.gradient();
}

#include "gradientdialog.moc"
