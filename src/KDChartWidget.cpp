/* -*- Mode: C++ -*-
   KDChart - a multi-platform charting engine
   */

/****************************************************************************
 ** Copyright (C) 2001-2006 Klarälvdalens Datakonsult AB.  All rights reserved.
 **
 ** This file is part of the KD Chart library.
 **
 ** This file may be distributed and/or modified under the terms of the
 ** GNU General Public License version 2 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.
 **
 ** Licensees holding valid commercial KD Chart licenses may use this file in
 ** accordance with the KD Chart Commercial License Agreement provided with
 ** the Software.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** See http://www.kdab.net/kdchart for
 **   information about KD Chart Commercial License Agreements.
 **
 ** Contact info@kdab.net if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/

#include <KDChartAbstractDiagram.h>
#include <KDChartBarDiagram.h>
#include <KDChartCartesianCoordinatePlane.h>
#include <KDChartChart.h>
#include <KDChartAbstractCoordinatePlane.h>
#include <KDChartLineDiagram.h>
#include <KDChartPieDiagram.h>
#include <KDChartPolarCoordinatePlane.h>
#include <KDChartPolarDiagram.h>
#include <KDChartRingDiagram.h>
#include <KDChartWidget.h>
#include <KDChartWidget_p.h>

#include <QDebug>
#include <QGridLayout>
#include <QStandardItemModel>

#define d d_func()

namespace KDChart {

Widget::Private::Private()
{
    m_model = new QStandardItemModel();
    usedDatasetWidth = 0;
}

Widget::Private::~Private()
{
    delete m_model; m_model = NULL;
}


/**
* \class Widget KDChartWidget.h
* \brief The KDChart widget for usage without Interwiev.
*
* If you want to use KDChart with Interview, use KDChart::Chart instead.
*/

/**
 * Constructor. Creates a new widget with all data initialized empty.
 *
 * \param parent the widget parent; passed on to QWidget
 */
Widget::Widget( QWidget* parent ) :
    QWidget(parent), _d( new Private )
{
    // setting up the layout
    d->m_chart = new Chart( this );
    QGridLayout* layout = new QGridLayout( this );
    layout->addWidget( d->m_chart );

    // our internal model
    d->m_model = new QStandardItemModel();

    // as default we have a cartesian coordinate plane ...
    // ... and a line diagram
    setType( Line );
}

/**
 * Destructor.
 */
Widget::~Widget()
{
    delete _d; _d = 0;
}

void Widget::init()
{
}

/**
 * Sets the data in the given column using a QVector of double.
 */
void Widget::setDataset( int column, const QVector< double > & data, const QString& title )
{
    if ( ! checkDatasetWidth( 1 ) )
        return;

    QModelIndex index;
    QStandardItemModel* model = d->m_model;

    justifyModelSize( data.size(), column + 1 );

    for( int i = 0; i < data.size(); ++i )
    {
        index = model->index( i, column );
        model->setData( index, QVariant( data[i] ), Qt::DisplayRole );
    }
    if ( title != QString() )
        model->setHeaderData( column, Qt::Vertical, QVariant( title ) );
}

/**
 * Sets the data in the given column using a QVector of QPairs
 * of double.
 */
void Widget::setDataset( int column, const QVector< QPair< double, double > > & data, const QString& title )
{
    if ( ! checkDatasetWidth( 2 ))
        return;

    QModelIndex index;
    QStandardItemModel* model = d->m_model;

    justifyModelSize( data.size(), (column + 1) * 2 );

    for( int i = 0; i < data.size(); ++i )
    {
        index = model->index( i, column * 2 );
        model->setData( index, QVariant( data[i].first ), Qt::DisplayRole );

        index = model->index( i, column * 2 + 1 );
        model->setData( index, QVariant( data[i].second ), Qt::DisplayRole );
    }
    if ( title != QString() )
        model->setHeaderData( column * 2, Qt::Vertical, QVariant( title ) );
}

/*
 * Resets all data.
 */
void Widget::resetData()
{
    d->m_model->clear();
    d->usedDatasetWidth = 0;
}

/**
 * Sets all global leadings (borders).
 */
void Widget::setGlobalLeading( int left, int top, int right, int bottom )
{
    d->m_chart->setGlobalLeading( left, top, right, bottom );
}

/**
 * Sets the left leading (border).
 */
void Widget::setGlobalLeadingLeft( int leading )
{
    d->m_chart->setGlobalLeadingLeft( leading );
}

/**
 * Returns the left leading (border).
 */
int Widget::globalLeadingLeft() const
{
    return d->m_chart->globalLeadingLeft();
}

/**
 * Sets the top leading (border).
 */
void Widget::setGlobalLeadingTop( int leading )
{
    d->m_chart->setGlobalLeadingTop( leading );
}

/**
 * Returns the top leading (border).
 */
int Widget::globalLeadingTop() const
{
    return d->m_chart->globalLeadingTop();
}

/**
 * Sets the right leading (border).
 */
void Widget::setGlobalLeadingRight( int leading )
{
    d->m_chart->setGlobalLeadingRight( leading );
}

/**
 * Returns the right leading (border).
 */
int Widget::globalLeadingRight() const
{
    return d->m_chart->globalLeadingRight();
}

/**
 * Sets the bottom leading (border).
 */
void Widget::setGlobalLeadingBottom( int leading )
{
    d->m_chart->setGlobalLeadingBottom( leading );
}

/**
 * Returns the bottom leading (border).
 */
int Widget::globalLeadingBottom() const
{
    return d->m_chart->globalLeadingBottom();
}

/**
 * Returns the first of all headers.
 */
KDChart::HeaderFooter* Widget::firstHeaderFooter() const
{
    return d->m_chart->headerFooter();
}

/**
 * Returns a list with all headers.
 */
const QList<KDChart::HeaderFooter*> Widget::allHeadersFooters() const
{
    return d->m_chart->headerFooters();
}

/**
 * Returns the count over all headers.
 */
int Widget::headerFooterCount() const
{
    return d->m_chart->headerFooters().count();
}

/**
 * Adds a new header/footer with the given text to the position.
 */
void Widget::addHeaderFooter( const QString& text,
                              HeaderFooter::HeaderFooterType type,
                              Position position)
{
    HeaderFooter* newHeader = new HeaderFooter( d->m_chart );
    newHeader->setType( type );
    newHeader->setPosition( position );
    newHeader->setText( text );
    d->m_chart->addHeaderFooter( newHeader ); // we need this explicit call !
}

/**
 * Adds an existing header / footer object.
 */
void Widget::addHeaderFooter( HeaderFooter* header )
{
    header->setParent( d->m_chart );
    d->m_chart->addHeaderFooter( header ); // we need this explicit call !
}

void Widget::replaceHeaderFooter( HeaderFooter* header, int position )
{
    header->setParent( d->m_chart );
    d->m_chart->replaceHeaderFooter( header, position );
}

void Widget::removeHeaderFooter( int position )
{
    d->m_chart->removeHeaderFooter( position );
}

/**
 * Returns the first of all legends.
 */
KDChart::Legend* Widget::firstLegend() const
{
    return d->m_chart->legend();
}

/**
 * Returns a list with all legends.
 */
const QList<KDChart::Legend*> Widget::allLegends() const
{
    return d->m_chart->legends();
}

/**
 * Returns the count over all legends.
 */
int Widget::legendCount() const
{
    return d->m_chart->legends().count();
}

/**
 * Adds an empty legend on the given position.
 */
void Widget::addLegend( Position position )
{
    Legend* legend = new Legend( diagram(), d->m_chart );
    d->m_chart->addLegend( legend );
    legend->setPosition( position );
}

/**
 * Adds a new, already existing, legend.
 */
void Widget::addLegend (Legend* legend )
{
    legend->setDiagram( diagram() );
    legend->setParent( d->m_chart );
}

void Widget::replaceLegend( Legend* legend, int position )
{
    legend->setDiagram( diagram() );
    legend->setParent( d->m_chart );
    d->m_chart->replaceLegend( legend, position );
}

void Widget::removeLegend( int position )
{
    d->m_chart->removeLegend( position );
}

/**
 * Returns a pointer to the current diagram.
 */
AbstractDiagram* Widget::diagram() const
{
    if ( coordinatePlane() == 0 )
        qDebug() << "diagram(): coordinatePlane() was NULL";

    return coordinatePlane()->diagram();
}

/**
 * Returns a pointer to the current coordinate plane.
 */
AbstractCoordinatePlane* Widget::coordinatePlane() const
{
    return d->m_chart->coordinatePlane();
}

/**
 * Sets the type of the chart.
 */
void Widget::setType( ChartType chartType )
{
    AbstractDiagram* diag = 0;
    CartesianCoordinatePlane* cartPlane = 0;
    PolarCoordinatePlane* polPlane = 0;

    if ( chartType == type() )
        return;

    switch ( chartType )
    {
        case Bar:
           cartPlane = new CartesianCoordinatePlane();
           d->m_chart->replaceCoordinatePlane( cartPlane );
           diag = new BarDiagram( cartPlane );
           break;
        case Line:
           cartPlane = new CartesianCoordinatePlane();
           d->m_chart->replaceCoordinatePlane( cartPlane );
           diag = new LineDiagram( cartPlane );
           break;
        case Pie:
           polPlane = new PolarCoordinatePlane();
           d->m_chart->replaceCoordinatePlane( polPlane );
           diag = new PieDiagram( polPlane );
           break;
        case Polar:
           polPlane = new PolarCoordinatePlane();
           d->m_chart->replaceCoordinatePlane( polPlane );
           diag = new PolarDiagram( polPlane );
           break;
        case Ring:
           polPlane = new PolarCoordinatePlane();
           d->m_chart->replaceCoordinatePlane( polPlane );
           diag = new RingDiagram( polPlane );
           break;
        case NoType:
           break;
    }
    if ( diag != NULL )
    {
        diag->setModel( d->m_model );
        coordinatePlane()->replaceDiagram( diag );

        LegendList legends = d->m_chart->legends();
        foreach(Legend* l, legends)
            l->setDiagram( diag );
    }
    coordinatePlane()->show();
}

/**
 * Returns the type of the chart.
 */
Widget::ChartType Widget::type() const
{
    if ( qobject_cast< BarDiagram* >( diagram() ) )
        return Bar;
    else if ( qobject_cast< LineDiagram* >( diagram() ) )
        return Line;
    else if( qobject_cast< PieDiagram* >( diagram() ) )
        return Pie;
    else if( qobject_cast< PolarDiagram* >( diagram() ) )
        return Polar;
    else if( qobject_cast< RingDiagram* >( diagram() ) )
        return Ring;
    else
        return NoType;
}

/**
 * Checks, wheter the given width matches with the one used until now.
 */
bool Widget::checkDatasetWidth( int width )
{
    if ( d->usedDatasetWidth == width || d->usedDatasetWidth == 0 ) {
        d->usedDatasetWidth = width;
        return true;
    }
    qDebug() << "It's impossible to mix up the different setDataset() methods on the same widget.";
    return false;
}

/**
 * Justifies the model, so that the given rows and columns fit into it.
 */
void Widget::justifyModelSize( int rows, int columns )
{
    QAbstractItemModel* model = d->m_model;
    int currentRows = model->rowCount();
    int currentCols = model->columnCount();

    if ( currentRows < rows )
        if ( ! model->insertRows( currentRows, rows - currentRows ))
            qDebug() << "justifyModelSize: could not increase model size.";

    if ( currentCols < columns )
        if ( ! model->insertColumns( currentCols, columns - currentCols ))
            qDebug() << "justifyModelSize: could not increase model size.";
}

}
