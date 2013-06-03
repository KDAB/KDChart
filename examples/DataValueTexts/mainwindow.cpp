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

#include "mainwindow.h"

#include <KDChartChart>
#include <KDChartDatasetProxyModel>
#include <KDChartAbstractCoordinatePlane>
#include <KDChartBarDiagram>
#include <KDChartTextAttributes>
#include <KDChartRelativePosition>
#include <KDChartPosition>


#include <QDebug>
#include <QPainter>

using namespace KDChart;

MainWindow::MainWindow( QWidget* parent ) :
    QWidget( parent )
{
    setupUi( this );

    QHBoxLayout* chartLayout = new QHBoxLayout( chartFrame );
    m_chart = new Chart();
    chartLayout->addWidget( m_chart );

    m_model.loadFromCSV( ":/data" );

    // Set up the diagram
    m_bars = new BarDiagram();
    m_bars->setModel( &m_model );

    QPen pen(  m_bars->pen() );
    pen.setColor( Qt::darkGray );
    pen.setWidth( 1 );
    m_bars->setPen( pen );
    m_chart->coordinatePlane()->replaceDiagram( m_bars );
    m_chart->setGlobalLeadingTop( 20 );

    scopeCommonRB->setFocus( Qt::OtherFocusReason );
    scopeCommonRB->setChecked( true );
    paintValuesCB->setChecked( true );
}


void MainWindow::on_scopeOneBarRB_toggled(  bool checked )
{
    if ( checked ) {
        scopeBarDatasetSB->setDisabled( false );
        scopeBarItemSB->setDisabled(    false );
        scopeDatasetSB->setDisabled(    true );
        populateWidgets();
    }
}
void MainWindow::on_scopeBarDatasetSB_valueChanged( int i )
{
    Q_UNUSED(i)
    populateWidgets();
}
void MainWindow::on_scopeBarItemSB_valueChanged(    int i )
{
    Q_UNUSED(i)
    populateWidgets();
}
void MainWindow::on_scopeDatasetRB_toggled( bool checked )
{
    if ( checked ) {
        scopeBarDatasetSB->setDisabled( true );
        scopeBarItemSB->setDisabled(    true );
        scopeDatasetSB->setDisabled(    false );
        populateWidgets();
    }
}
void MainWindow::on_scopeDatasetSB_valueChanged( int i )
{
    Q_UNUSED(i)
    populateWidgets();
}
void MainWindow::on_scopeCommonRB_toggled( bool checked )
{
    if ( checked ) {
        scopeBarDatasetSB->setDisabled( true );
        scopeBarItemSB->setDisabled(    true );
        scopeDatasetSB->setDisabled(    true );
        populateWidgets();
    }
}

void MainWindow::on_paintValuesCB_toggled( bool checked )
{
    DataValueAttributes da( attributes() );
    da.setVisible( checked );
    setAttributes( da );

    m_chart->update();
}

void MainWindow::on_fontCombo_currentIndexChanged( const QString & text )
{
    DataValueAttributes da( attributes() );
    TextAttributes ta( da.textAttributes() );
    QFont font( text );
    ta.setFont( font );
    da.setTextAttributes( ta );
    setAttributes( da );

    m_chart->update();
}

void MainWindow::on_relativeSizeSB_valueChanged( int i )
{
    DataValueAttributes da( attributes() );
    TextAttributes ta( da.textAttributes() );
    Measure fs( ta.fontSize() );
    fs.setValue( i );
    ta.setFontSize( i );
    da.setTextAttributes( ta );
    setAttributes( da );

    m_chart->update();
}

void MainWindow::on_minimumSizeSB_valueChanged(  int i )
{
    DataValueAttributes da( attributes() );
    TextAttributes ta( da.textAttributes() );
    Measure fs( ta.fontSize() );
    fs.setValue( i );
    ta.setMinimalFontSize( i );
    da.setTextAttributes( ta );
    setAttributes( da );

    m_chart->update();
}

void MainWindow::on_rotationSB_valueChanged( int i )
{
    DataValueAttributes da( attributes() );
    TextAttributes ta( da.textAttributes() );
    ta.setRotation( i );
    da.setTextAttributes( ta );
    setAttributes( da );

    m_chart->update();
}

void MainWindow::on_posPosCombo_currentIndexChanged(   const QString & text )
{
    DataValueAttributes da( attributes() );
    RelativePosition relPos( da.positivePosition() );
    relPos.setReferencePosition( Position::fromName( qPrintable( text ) ) );
    da.setPositivePosition( relPos );
    setAttributes( da );

    m_chart->update();
}

void MainWindow::on_posAlignCombo_currentIndexChanged( const QString & text )
{
    DataValueAttributes da( attributes() );
    RelativePosition relPos( da.positivePosition() );
    relPos.setAlignment( alignmentFromScreeName( text ) );
    da.setPositivePosition( relPos );
    setAttributes( da );

    m_chart->update();
}

void MainWindow::on_posPadHoriSB_valueChanged( int i )
{
    DataValueAttributes da( attributes() );
    RelativePosition relPos( da.positivePosition() );
    Measure pad( relPos.horizontalPadding() );
    pad.setValue( i );
    relPos.setHorizontalPadding( pad );
    da.setPositivePosition( relPos );
    setAttributes( da );

    m_chart->update();
}

void MainWindow::on_posPadVertSB_valueChanged( int i )
{
    DataValueAttributes da( attributes() );
    RelativePosition relPos( da.positivePosition() );
    Measure pad( relPos.verticalPadding() );
    pad.setValue( i );
    relPos.setVerticalPadding( pad );
    da.setPositivePosition( relPos );
    setAttributes( da );

    m_chart->update();
}

void MainWindow::on_negPosCombo_currentIndexChanged(   const QString & text )
{
    DataValueAttributes da( attributes() );
    RelativePosition relPos( da.negativePosition() );
    relPos.setReferencePosition( Position::fromName( qPrintable( text ) ) );
    da.setNegativePosition( relPos );
    setAttributes( da );

    m_chart->update();
}

void MainWindow::on_negAlignCombo_currentIndexChanged( const QString & text )
{
    DataValueAttributes da( attributes() );
    RelativePosition relPos( da.negativePosition() );
    relPos.setAlignment( alignmentFromScreeName( text ) );
    da.setNegativePosition( relPos );
    setAttributes( da );

    m_chart->update();
}

void MainWindow::on_negPadHoriSB_valueChanged( int i )
{
    DataValueAttributes da( attributes() );
    RelativePosition relPos( da.negativePosition() );
    Measure pad( relPos.horizontalPadding() );
    pad.setValue( i );
    relPos.setHorizontalPadding( pad );
    da.setNegativePosition( relPos );
    setAttributes( da );

    m_chart->update();
}

void MainWindow::on_negPadVertSB_valueChanged( int i )
{
    DataValueAttributes da( attributes() );
    RelativePosition relPos( da.negativePosition() );
    Measure pad( relPos.verticalPadding() );
    pad.setValue( i );
    relPos.setVerticalPadding( pad );
    da.setNegativePosition( relPos );
    setAttributes( da );

    m_chart->update();
}

void MainWindow::on_labelLE_textEdited(  const QString & text )
{
    DataValueAttributes da( attributes() );
    da.setDataLabel( text.isEmpty() ? QString() : text );
    setAttributes( da );

    m_chart->update();
}

void MainWindow::on_prefixLE_textEdited( const QString & text )
{
    DataValueAttributes da( attributes() );
    da.setPrefix( text.isEmpty() ? QString() : text );
    setAttributes( da );

    m_chart->update();
}

void MainWindow::on_suffixLE_textEdited( const QString & text )
{
    DataValueAttributes da( attributes() );
    da.setSuffix( text.isEmpty() ? QString() : text );
    setAttributes( da );

    m_chart->update();
}

const QModelIndex MainWindow::currentIndex() const
{
    const int dataset = scopeBarDatasetSB->value();
    const int item    = scopeBarItemSB->value();
    return m_bars->model()->index( item, dataset, QModelIndex() );
}

const KDChart::DataValueAttributes MainWindow::attributes() const
{
    if ( scopeOneBarRB->isChecked() ) {
        //qDebug() << "attributes() returns settings for one single bar";
        return m_bars->dataValueAttributes( currentIndex() );
    }
    if ( scopeDatasetRB->isChecked() ) {
        //qDebug() << "attributes() returns settings for a dataset";
        return m_bars->dataValueAttributes( scopeDatasetSB->value() );
    }
    //qDebug() << "attributes() returns common settings";
    return m_bars->dataValueAttributes();
}

void MainWindow::setAttributes( const KDChart::DataValueAttributes& da )
{
    if ( scopeOneBarRB->isChecked() )
        m_bars->setDataValueAttributes( currentIndex(), da );
    else if ( scopeDatasetRB->isChecked() )
        m_bars->setDataValueAttributes( scopeDatasetSB->value(), da );
    else
        m_bars->setDataValueAttributes( da );
}

// just a convenience method:
// In the combo box we have the text "( Default Value )" instead of "Unknown Position"
// because by setting a position to unknown we get KD Chart to use the
// diagram-specific default positions.
const char* MainWindow::positionToScreenName( const Position& pos ) const
{
    static const char* defaultPositionName = "( Default Value )";
    if ( pos.isUnknown() )
        return defaultPositionName;
    return pos.name();
}

const Qt::Alignment MainWindow::alignmentFromScreeName( const QString& name ) const
{
    if ( name == "Center" )      return Qt::AlignCenter;
    if ( name == "BottomLeft" )  return Qt::AlignLeft    | Qt::AlignBottom;
    if ( name == "Bottom" )      return Qt::AlignHCenter | Qt::AlignBottom;
    if ( name == "BottomRight" ) return Qt::AlignRight   | Qt::AlignBottom;
    if ( name == "Right" )       return Qt::AlignRight   | Qt::AlignVCenter;
    if ( name == "TopRight" )    return Qt::AlignRight   | Qt::AlignTop;
    if ( name == "Top" )         return Qt::AlignHCenter | Qt::AlignTop;
    if ( name == "TopLeft" )     return Qt::AlignLeft    | Qt::AlignTop;
    if ( name == "Left" )        return Qt::AlignLeft    | Qt::AlignVCenter;
    return Qt::AlignCenter;
}

const QString MainWindow::alignmentToScreenName( const Qt::Alignment& align ) const
{
    if ( align == Qt::AlignCenter )                       return "Center";
    if ( align == (Qt::AlignLeft    | Qt::AlignBottom) )  return "BottomLeft";
    if ( align == (Qt::AlignHCenter | Qt::AlignBottom) )  return "Bottom";
    if ( align == (Qt::AlignRight   | Qt::AlignBottom) )  return "BottomRight";
    if ( align == (Qt::AlignRight   | Qt::AlignVCenter) ) return "Right";
    if ( align == (Qt::AlignRight   | Qt::AlignTop) )     return "TopRight";
    if ( align == (Qt::AlignHCenter | Qt::AlignTop) )     return "Top";
    if ( align == (Qt::AlignLeft    | Qt::AlignTop) )     return "TopLeft";
    if ( align == (Qt::AlignLeft    | Qt::AlignVCenter) ) return "Left";
    return "Center";
}

void MainWindow::populateWidgets()
{
    const DataValueAttributes da( attributes() );
    const TextAttributes ta( da.textAttributes() );
    const RelativePosition posPos( da.positivePosition() );
    const RelativePosition negPos( da.negativePosition() );

    paintValuesCB->setChecked( da.isVisible() && ta.isVisible() );
    fontCombo->setCurrentFont( ta.font() );
    relativeSizeSB->setValue( static_cast<int>(ta.fontSize().value()) );
    minimumSizeSB->setValue(  static_cast<int>(ta.minimalFontSize().value()) );
    rotationSB->setValue( static_cast<int>(ta.rotation()) );

    posPosCombo->setCurrentIndex( posPosCombo->findText(
            positionToScreenName( posPos.referencePosition() ) ) );
    posAlignCombo->setCurrentIndex( posAlignCombo->findText(
            alignmentToScreenName( posPos.alignment() ) ) );
    posPadHoriSB->setValue( static_cast<int>(posPos.horizontalPadding().value()) );
    posPadVertSB->setValue( static_cast<int>(posPos.verticalPadding().value()) );

    negPosCombo->setCurrentIndex( negPosCombo->findText( positionToScreenName(
            negPos.referencePosition() ) ) );
    negAlignCombo->setCurrentIndex( negAlignCombo->findText(
            alignmentToScreenName( negPos.alignment() ) ) );
    negPadHoriSB->setValue( static_cast<int>(negPos.horizontalPadding().value()) );
    negPadVertSB->setValue( static_cast<int>(negPos.verticalPadding().value()) );

    labelLE->setText(  da.dataLabel() );
    prefixLE->setText( da.prefix() );
    suffixLE->setText( da.suffix() );
}
