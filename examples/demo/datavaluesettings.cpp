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

#include "datavaluesettings.h"
#include "ui_datavaluesettings.h"

#include <KDChart/KDChartChart>
#include <KDChart/KDChartAbstractCoordinatePlane>
#include <KDChart/KDChartBarDiagram>
#include <KDChart/KDChartTextAttributes>
#include <KDChart/KDChartRelativePosition>
#include <KDChart/KDChartPosition>
#include <KDChartDataValueAttributes>
#include <KDChart/KDChartAbstractDiagram>


#include <QDebug>
#include <QPainter>

using namespace KDChart;

class DataValueSettings::Private : public QObject
{
    Q_OBJECT
public:
    Private( KDChart::Chart *chart, DataValueSettings *q );
    DataValueSettings *qq;
    void init();
public Q_SLOTS:
    void on_scopeOneBarRB_toggled(  bool checked );
    void on_scopeBarDatasetSB_valueChanged( int i );
    void on_scopeBarItemSB_valueChanged(    int i );
    void on_scopeDatasetRB_toggled( bool checked );
    void on_scopeDatasetSB_valueChanged( int i );
    void on_scopeCommonRB_toggled( bool checked );

    void on_paintValuesCB_toggled( bool checked );

    void on_fontCombo_currentIndexChanged( const QString & text );
    void on_relativeSizeSB_valueChanged( int i );
    void on_minimumSizeSB_valueChanged(  int i );
    void on_rotationSB_valueChanged( int i );

    void on_posPosCombo_currentIndexChanged(   const QString & text );
    void on_posAlignCombo_currentIndexChanged( const QString & text );
    void on_posPadHoriSB_valueChanged( int i );
    void on_posPadVertSB_valueChanged( int i );
    void on_negPosCombo_currentIndexChanged(   const QString & text );
    void on_negAlignCombo_currentIndexChanged( const QString & text );
    void on_negPadHoriSB_valueChanged( int i );
    void on_negPadVertSB_valueChanged( int i );

    void on_labelLE_textEdited(  const QString & text );
    void on_prefixLE_textEdited( const QString & text );
    void on_suffixLE_textEdited( const QString & text );
public:
    const QModelIndex currentIndex() const;
    const KDChart::DataValueAttributes attributes() const;
    const char* positionToScreenName( const KDChart::Position& pos ) const;
    const Qt::Alignment alignmentFromScreeName( const QString& name ) const;
    const QString alignmentToScreenName( const Qt::Alignment& align ) const;
    void populateWidgets();
    void setAttributes( const KDChart::DataValueAttributes& da );
    void connectWidgets();

    KDChart::Chart* m_chart;
    Ui::DataValueSettings *ui;
};

DataValueSettings::Private::Private( KDChart::Chart *chart, DataValueSettings *q )
    : qq ( q )
    , m_chart( chart )
    , ui( new Ui::DataValueSettings )
{

}

void DataValueSettings::Private::init()
{
    ui->setupUi( qq );
    ui->scopeCommonRB->setFocus( Qt::OtherFocusReason );
    ui->scopeCommonRB->setChecked( true );
    ui->paintValuesCB->setChecked( true );
    populateWidgets();
    connectWidgets();
}

void DataValueSettings::Private::connectWidgets()
{
    connect( ui->scopeBarDatasetSB, SIGNAL( valueChanged( int ) ), this, SLOT( on_scopeBarDatasetSB_valueChanged( int ) ) );
    connect( ui->scopeBarItemSB, SIGNAL( valueChanged( int ) ), this, SLOT( on_scopeBarItemSB_valueChanged( int ) ) );
    connect( ui->scopeDatasetSB, SIGNAL( valueChanged( int ) ), this, SLOT( on_scopeDatasetSB_valueChanged( int ) ) );
    connect( ui->scopeOneBarRB, SIGNAL( toggled( bool ) ), this, SLOT( on_scopeOneBarRB_toggled( bool ) ) );
    connect( ui->scopeDatasetRB, SIGNAL( toggled( bool ) ), this, SLOT( on_scopeDatasetRB_toggled( bool ) ) );
    connect( ui->scopeCommonRB, SIGNAL( toggled( bool ) ), this, SLOT( on_scopeCommonRB_toggled( bool ) ) );
    connect( ui->paintValuesCB, SIGNAL( toggled( bool ) ), this, SLOT( on_paintValuesCB_toggled( bool ) ) );
    connect( ui->fontCombo, SIGNAL( currentIndexChanged( QString ) ), this , SLOT( on_fontCombo_currentIndexChanged( QString ) ) );
    connect( ui->relativeSizeSB, SIGNAL( valueChanged( int ) ), this, SLOT( on_relativeSizeSB_valueChanged( int ) ) );
    connect( ui->minimumSizeSB, SIGNAL( valueChanged( int ) ), this, SLOT( on_minimumSizeSB_valueChanged( int ) ) );
    connect( ui->rotationSB, SIGNAL( valueChanged( int ) ), this, SLOT( on_rotationSB_valueChanged( int ) ) );
    connect( ui->posPosCombo, SIGNAL( currentIndexChanged( QString ) ), this, SLOT( on_posPosCombo_currentIndexChanged( QString ) ) );
    connect( ui->posAlignCombo, SIGNAL( currentIndexChanged( QString ) ), this, SLOT( on_posAlignCombo_currentIndexChanged( QString ) ) );
    connect( ui->posPadHoriSB, SIGNAL( valueChanged(int) ), this, SLOT( on_posPadHoriSB_valueChanged(int) ) );
    connect( ui->posPadVertSB, SIGNAL( valueChanged(int) ), this, SLOT( on_posPadVertSB_valueChanged(int) ) );
    connect( ui->negPosCombo, SIGNAL( currentIndexChanged( QString ) ), this, SLOT( on_negPosCombo_currentIndexChanged( QString ) ) );
    connect( ui->negAlignCombo, SIGNAL( currentIndexChanged( QString ) ), this, SLOT( on_negAlignCombo_currentIndexChanged( QString ) ) );
    connect( ui->negPadHoriSB, SIGNAL( valueChanged( int ) ), this, SLOT( on_negPadHoriSB_valueChanged( int ) ) );
    connect( ui->negPadVertSB, SIGNAL( valueChanged( int ) ), this, SLOT( on_negPadVertSB_valueChanged( int ) ) );
    connect( ui->labelLE, SIGNAL( textEdited( QString ) ), this, SLOT( on_labelLE_textEdited( QString ) ) );
    connect( ui->prefixLE, SIGNAL( textEdited( QString ) ), this, SLOT( on_prefixLE_textEdited( QString ) ) );
    connect( ui->suffixLE, SIGNAL( textEdited( QString ) ), this, SLOT( on_suffixLE_textEdited( QString ) ) );
}

DataValueSettings::DataValueSettings( KDChart::Chart *chart, QWidget* parent )
    : QWidget( parent )
    , d( new Private( chart, this ) )
{
    d->init();
}

DataValueSettings::~DataValueSettings()
{
    delete d;
}

void DataValueSettings::refresh()
{
    d->populateWidgets();
}


void DataValueSettings::Private::on_scopeOneBarRB_toggled(  bool checked )
{
    if ( checked ) {
        ui->scopeBarDatasetSB->setDisabled( false );
        ui->scopeBarItemSB->setDisabled( false );
        ui->scopeDatasetSB->setDisabled( true );
        populateWidgets();
    }
}

void DataValueSettings::Private::on_scopeBarDatasetSB_valueChanged( int i )
{
    Q_UNUSED(i)
    populateWidgets();
}

void DataValueSettings::Private::on_scopeBarItemSB_valueChanged( int i )
{
    Q_UNUSED(i)
    populateWidgets();
}

void DataValueSettings::Private::on_scopeDatasetRB_toggled( bool checked )
{
    if ( checked ) {
        ui->scopeBarDatasetSB->setDisabled( true );
        ui->scopeBarItemSB->setDisabled( true );
        ui->scopeDatasetSB->setDisabled( false );
        populateWidgets();
    }
}

void DataValueSettings::Private::on_scopeDatasetSB_valueChanged( int i )
{
    Q_UNUSED(i)
    populateWidgets();
}

void DataValueSettings::Private::on_scopeCommonRB_toggled( bool checked )
{
    if ( checked ) {
        ui->scopeBarDatasetSB->setDisabled( true );
        ui->scopeBarItemSB->setDisabled( true );
        ui->scopeDatasetSB->setDisabled( true );
        populateWidgets();
    }
}

void DataValueSettings::Private::on_paintValuesCB_toggled( bool checked )
{
    DataValueAttributes da( attributes() );
    da.setVisible( checked );
    setAttributes( da );

    m_chart->update();
}

void DataValueSettings::Private::on_fontCombo_currentIndexChanged( const QString & text )
{
    DataValueAttributes da( attributes() );
    TextAttributes ta( da.textAttributes() );
    QFont font( text );
    ta.setFont( font );
    da.setTextAttributes( ta );
    setAttributes( da );

    m_chart->update();
}

void DataValueSettings::Private::on_relativeSizeSB_valueChanged( int i )
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

void DataValueSettings::Private::on_minimumSizeSB_valueChanged( int i )
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

void DataValueSettings::Private::on_rotationSB_valueChanged( int i )
{
    DataValueAttributes da( attributes() );
    TextAttributes ta( da.textAttributes() );
    ta.setRotation( i );
    da.setTextAttributes( ta );
    setAttributes( da );

    m_chart->update();
}

void DataValueSettings::Private::on_posPosCombo_currentIndexChanged( const QString & text )
{
    DataValueAttributes da( attributes() );
    RelativePosition relPos( da.positivePosition() );
    relPos.setReferencePosition( Position::fromName( qPrintable( text ) ) );
    da.setPositivePosition( relPos );
    setAttributes( da );

    m_chart->update();
}

void DataValueSettings::Private::on_posAlignCombo_currentIndexChanged( const QString & text )
{
    DataValueAttributes da( attributes() );
    RelativePosition relPos( da.positivePosition() );
    relPos.setAlignment( alignmentFromScreeName( text ) );
    da.setPositivePosition( relPos );
    setAttributes( da );

    m_chart->update();
}

void DataValueSettings::Private::on_posPadHoriSB_valueChanged( int i )
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

void DataValueSettings::Private::on_posPadVertSB_valueChanged( int i )
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

void DataValueSettings::Private::on_negPosCombo_currentIndexChanged( const QString & text )
{
    DataValueAttributes da( attributes() );
    RelativePosition relPos( da.negativePosition() );
    relPos.setReferencePosition( Position::fromName( qPrintable( text ) ) );
    da.setNegativePosition( relPos );
    setAttributes( da );

    m_chart->update();
}

void DataValueSettings::Private::on_negAlignCombo_currentIndexChanged( const QString & text )
{
    DataValueAttributes da( attributes() );
    RelativePosition relPos( da.negativePosition() );
    relPos.setAlignment( alignmentFromScreeName( text ) );
    da.setNegativePosition( relPos );
    setAttributes( da );

    m_chart->update();
}

void DataValueSettings::Private::on_negPadHoriSB_valueChanged( int i )
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

void DataValueSettings::Private::on_negPadVertSB_valueChanged( int i )
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

void DataValueSettings::Private::on_labelLE_textEdited( const QString & text )
{
    DataValueAttributes da( attributes() );
    da.setDataLabel( text.isEmpty() ? QString() : text );
    setAttributes( da );

    m_chart->update();
}

void DataValueSettings::Private::on_prefixLE_textEdited( const QString & text )
{
    DataValueAttributes da( attributes() );
    da.setPrefix( text.isEmpty() ? QString() : text );
    setAttributes( da );

    m_chart->update();
}

void DataValueSettings::Private::on_suffixLE_textEdited( const QString & text )
{
    DataValueAttributes da( attributes() );
    da.setSuffix( text.isEmpty() ? QString() : text );
    setAttributes( da );

    m_chart->update();
}

const QModelIndex DataValueSettings::Private::currentIndex() const
{
    const int dataset = ui->scopeBarDatasetSB->value();
    const int item    = ui->scopeBarItemSB->value();
    Q_ASSERT( m_chart );
    Q_ASSERT( m_chart->coordinatePlane() );
    AbstractDiagram *diag = m_chart->coordinatePlane()->diagram();
    return diag->model()->index( item, dataset, QModelIndex() );
}

const KDChart::DataValueAttributes DataValueSettings::Private::attributes() const
{
    Q_ASSERT( m_chart );
    Q_ASSERT( m_chart->coordinatePlane() );
    AbstractDiagram *diag = m_chart->coordinatePlane()->diagram();
    if ( ui->scopeOneBarRB->isChecked() ) {
        //qDebug() << "attributes() returns settings for one single bar";
        return diag->dataValueAttributes( currentIndex() );
    }
    if ( ui->scopeDatasetRB->isChecked() ) {
        //qDebug() << "attributes() returns settings for a dataset";
        return diag->dataValueAttributes( ui->scopeDatasetSB->value() );
    }
    //qDebug() << "attributes() returns common settings";
    return diag->dataValueAttributes();
}

void DataValueSettings::Private::setAttributes( const KDChart::DataValueAttributes& da )
{
    Q_ASSERT( m_chart );
    Q_ASSERT( m_chart->coordinatePlane() );
    AbstractDiagram *diag = m_chart->coordinatePlane()->diagram();
    if ( ui->scopeOneBarRB->isChecked() )
        diag->setDataValueAttributes( currentIndex(), da );
    else if ( ui->scopeDatasetRB->isChecked() )
        diag->setDataValueAttributes( ui->scopeDatasetSB->value(), da );
    else
        diag->setDataValueAttributes( da );
}

// just a convenience method:
// In the combo box we have the text "( Default Value )" instead of "Unknown Position"
// because by setting a position to unknown we get KD Chart to use the
// diagram-specific default positions.
const char* DataValueSettings::Private::positionToScreenName( const Position& pos ) const
{
    static const char* defaultPositionName = "( Default Value )";
    if ( pos.isUnknown() )
        return defaultPositionName;
    return pos.name();
}

const Qt::Alignment DataValueSettings::Private::alignmentFromScreeName( const QString& name ) const
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

const QString DataValueSettings::Private::alignmentToScreenName( const Qt::Alignment& align ) const
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

void DataValueSettings::Private::populateWidgets()
{
    const DataValueAttributes da( attributes() );
    const TextAttributes ta( da.textAttributes() );
    const RelativePosition posPos( da.positivePosition() );
    const RelativePosition negPos( da.negativePosition() );

    ui->paintValuesCB->setChecked( da.isVisible() && ta.isVisible() );
    ui->fontCombo->setCurrentFont( ta.font() );
    ui->relativeSizeSB->setValue( static_cast<int>(ta.fontSize().value()) );
    ui->minimumSizeSB->setValue( static_cast<int>(ta.minimalFontSize().value()) );
    ui->rotationSB->setValue( static_cast<int>(ta.rotation()) );

    ui->posPosCombo->setCurrentIndex( ui->posPosCombo->findText(
            positionToScreenName( posPos.referencePosition() ) ) );
    ui->posAlignCombo->setCurrentIndex( ui->posAlignCombo->findText(
            alignmentToScreenName( posPos.alignment() ) ) );
    ui->posPadHoriSB->setValue( static_cast<int>(posPos.horizontalPadding().value()) );
    ui->posPadVertSB->setValue( static_cast<int>(posPos.verticalPadding().value()) );

    ui->negPosCombo->setCurrentIndex( ui->negPosCombo->findText( positionToScreenName(
            negPos.referencePosition() ) ) );
    ui->negAlignCombo->setCurrentIndex( ui->negAlignCombo->findText(
            alignmentToScreenName( negPos.alignment() ) ) );
    ui->negPadHoriSB->setValue( static_cast<int>(negPos.horizontalPadding().value()) );
    ui->negPadVertSB->setValue( static_cast<int>(negPos.verticalPadding().value()) );

    ui->labelLE->setText( da.dataLabel() );
    ui->prefixLE->setText( da.prefix() );
    ui->suffixLE->setText( da.suffix() );
}

#include "datavaluesettings.moc"
