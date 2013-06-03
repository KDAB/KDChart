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

#include "datasetsettings.h"
#include "ui_datasetsettings.h"

#include <KDChart/KDChartAbstractCoordinatePlane>
#include <KDChart/KDChartChart>

#include <QColorDialog>
#include <QFileDialog>

#if QT_VERSION < 0x050000
#include <QtGui/QCleanlooksStyle>
#else
#include <QStyleFactory>
#endif

#include <QtGui/QImage>

#include <QtCore/QObject>

#include <QtCore/QDebug>

#include "gradientdialog.h"

using namespace KDChart;

class DatasetSettings::Private : public QObject
{
    Q_OBJECT
public:
    Private( Chart *chart, DatasetSettings *q, QObject *parent );
    ~Private();

    Ui::DatasetSettings *ui;
    int m_dataset;
    int m_datasetCount;
    KDChart::Chart *m_chart;
    DatasetSettings *qq;
public Q_SLOTS:
    void changeColor();
    void changeOutline();
};

DatasetSettings::Private::Private( Chart *chart, DatasetSettings *q, QObject *parent )
    : QObject( parent )
    , ui( new Ui::DatasetSettings )
    , m_dataset( 0 )
    , m_datasetCount( 0 )
    , m_chart( chart )
    , qq( q )
{

}

DatasetSettings::Private::~Private()
{
    delete ui;
}

void DatasetSettings::Private::changeColor()
{
    const int index = ui->datasetSelector->currentIndex();
    if ( ui->Color->isChecked() )
    {
        QBrush setBrush = m_chart->coordinatePlane()->diagram()->brush( index );
        const QColor color = QColorDialog::getColor( setBrush.color(), qq, tr( "Choose new color" ) );
        if ( !color.isValid() )
            return;
        m_chart->coordinatePlane()->diagram()->setBrush( index, color );
        QPalette palette = ui->colorDisplay->palette();
        palette.setBrush( QPalette::Button, color );
        ui->colorDisplay->setPalette( palette );
    }
    else if ( ui->textureBtn->isChecked() )
    {
        //QBrush setBrush = m_chart->coordinatePlane()->diagram()->brush( index );
        QImage texture;

        const QString filename = QFileDialog::getOpenFileName( qq, tr( "Choose Texture" ), QString(), tr( "Images (*.png *.xpm *.jpg)" ) );
        if ( filename.isEmpty() )
            return;
        texture = QImage( filename );
        m_chart->coordinatePlane()->diagram()->setBrush( index, texture );
        QPalette palette = ui->colorDisplay->palette();
        palette.setBrush( QPalette::Button, QBrush( texture ) );
        ui->colorDisplay->setPalette( palette );
    }
    else
    {
        QBrush setBrush = m_chart->coordinatePlane()->diagram()->brush( index );
        QGradient grad;
        QLinearGradient lGrad;
        lGrad.setColorAt( 0, Qt::black );
        lGrad.setColorAt( 1, setBrush.color() );
        grad = lGrad;

        if ( setBrush.gradient() )
            grad = *setBrush.gradient();
        const QGradient &color = GradientDialog::getGradient( grad, qq, tr( "Choose new color" ) );
        m_chart->coordinatePlane()->diagram()->setBrush( index, color );
        QPalette palette = ui->colorDisplay->palette();
        palette.setBrush( QPalette::Button, QBrush( color ) );
        ui->colorDisplay->setPalette( palette );
    }
}

void DatasetSettings::Private::changeOutline()
{
    const int index = ui->datasetSelector->currentIndex();
    if ( ui->Color->isChecked() )
    {
        QPen pen = m_chart->coordinatePlane()->diagram()->pen( index );
        const QColor color = QColorDialog::getColor( pen.color(), qq, tr( "Choose new color" ) );
        if ( !color.isValid() )
            return;
        pen.setColor( color );
        m_chart->coordinatePlane()->diagram()->setPen( index, pen );
        QPalette palette = ui->outlineBtn->palette();
        palette.setBrush( QPalette::Button, color );
        ui->outlineBtn->setPalette( palette );
    }
}

DatasetSettings::DatasetSettings( Chart *chart, QWidget *parent )
    : QWidget( parent )
    , d( new Private( chart, this, this ) )
{
    d->ui->setupUi(this);
#ifdef Q_OS_LINUX
#if QT_VERSION < 0x050000
    d->ui->colorDisplay->setStyle( new QCleanlooksStyle );
    d->ui->outlineBtn->setStyle( new QCleanlooksStyle );
#else
    d->ui->colorDisplay->setStyle( QStyleFactory::create( QStringLiteral( "cleanlooks" ) ) );
    d->ui->outlineBtn->setStyle( QStyleFactory::create( QStringLiteral( "cleanlooks" ) ) );
#endif
#endif
    connect( d->ui->datasetSelector, SIGNAL( currentIndexChanged( int ) ), this, SLOT( indexChanged( int ) ) );
    connect( d->ui->colorDisplay, SIGNAL( clicked() ), d, SLOT( changeColor() ) );
    connect( d->ui->outlineBtn, SIGNAL( clicked() ), d, SLOT( changeOutline() ) );
}

DatasetSettings::~DatasetSettings()
{
    delete d;
}

int DatasetSettings::datasetCount() const
{
    return d->m_datasetCount;
}
void DatasetSettings::setDatasetCount( int value )
{
    if ( d->m_datasetCount != value )
    {
        d->m_datasetCount = value;
        QStringList list;
        for ( int i = 0; i < value; ++i )
        {
            list << tr( "Dataset %1" ).arg( i );
        }
        d->ui->datasetSelector->clear();
        d->ui->datasetSelector->addItems( list );
        Q_EMIT datasetCountChanged();
    }
}

void DatasetSettings::indexChanged( int index )
{
    if ( d->m_chart && index >= 0 )
    {
        const QBrush setBrush = d->m_chart->coordinatePlane()->diagram()->brush( index );
        QPalette palette = d->ui->colorDisplay->palette();
        if ( setBrush.gradient() )
            d->ui->radioButton_2->setChecked( true );
        else if ( !setBrush.textureImage().isNull() )
            d->ui->textureBtn->setChecked( true );
        else
            d->ui->Color->setChecked( true );
        palette.setBrush( QPalette::Button, setBrush );
        d->ui->colorDisplay->setPalette( palette );
        const QPen pen = d->m_chart->coordinatePlane()->diagram()->pen( index );
        QPalette penPalette = d->ui->outlineBtn->palette();
        penPalette.setBrush( QPalette::Button, pen.color() );
        d->ui->outlineBtn->setPalette( penPalette );
    }
}

void DatasetSettings::diagramTypeChanged()
{

}

#include "datasetsettings.moc"
