/* -*- Mode: C++ -*-
   KDChart - a multi-platform charting engine
   */

/****************************************************************************
 ** Copyright (C) 2005-2006 Klar�vdalens Datakonsult AB.  All rights reserved.
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

#ifndef KDCHARTLEGEND_H
#define KDCHARTLEGEND_H

#include "KDChartAbstractArea.h"
#include "KDChartMarkerAttributes.h"

class QTextTable;
class QDomDocumentFragment;

namespace KDChart {

    class AbstractDiagram;

class KDCHART_EXPORT Legend : public AbstractArea
{
    Q_OBJECT

    Q_DISABLE_COPY( Legend )
    KDCHART_DECLARE_PRIVATE_DERIVED_QWIDGET( Legend )
public:
    Legend( QWidget* parent );
    Legend( KDChart::AbstractDiagram* diagram, QWidget* parent );
    virtual ~Legend();

    virtual QDomDocumentFragment toXML() const;

    virtual Legend * clone() const;

    QSize calcSizeHint() const;
    virtual void paintEvent( QPaintEvent* );

    enum LegendPosition { WestNorthWest,
                          NorthWest,
                          NorthNorthWest,
                          North,
                          NorthNorthEast,
                          NorthEast,
                          EastNorthEast,
                          East,
                          EastSouthEast,
                          SouthEast,
                          SouthSouthEast,
                          South,
                          SouthSouthWest,
                          SouthWest,
                          WestSouthWest,
                          West };

    void setDiagram( KDChart::AbstractDiagram* diagram );
    KDChart::AbstractDiagram* diagram() const;

    void setPosition( LegendPosition position );
    LegendPosition position() const;

    void setOrientation( Qt::Orientation orientation );
    Qt::Orientation orientation() const;

    void setShowLines( bool legendShowLines );
    bool showLines() const;

    void resetTexts();
    void setText( uint dataset, const QString& text );
    QString text( uint dataset ) const;

    uint datasetCount() const;

    void setDefaultColors();
    void setRainbowColors();
    void setSubduedColors( bool ordered = false );

    void setBrushesFromDiagram( KDChart::AbstractDiagram* diagram );

    void setColor( uint dataset, const QColor& color );
    void setBrush( uint dataset, const QBrush& brush );
    QBrush brush( uint dataset ) const;

    void setPen( uint dataset, const QPen& pen );
    QPen pen( uint dataset ) const;

    void setMarkerAttributes( uint dataset, const MarkerAttributes& );
    MarkerAttributes markerAttributes( uint dataset ) const;

    void setTextAttributes( const TextAttributes &a );
    TextAttributes textAttributes() const;

    void setTitleText( const QString& text );
    QString titleText() const;

    void setTitleTextAttributes( const TextAttributes &a );
    TextAttributes titleTextAttributes() const;

    // FIXME same as frameSettings()->padding()?
    void setSpacing( uint space );
    uint spacing() const;

/*public static*/
    static LegendPosition fromString( QString name, bool* ok=0 );

signals:
    void destroyedLegend( Legend* );

private slots:
    void resetDiagram();
    void buildLegend();
}; // End of class Legend

}


#endif // KDCHARTLEGEND_H
