/****************************************************************************
 ** Copyright (C) 2006 Klarälvdalens Datakonsult AB.  All rights reserved.
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
 **   information about KDChart Commercial License Agreements.
 **
 ** Contact info@kdab.net if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/

#ifndef KDCHARTLEGEND_P_H
#define KDCHARTLEGEND_P_H

#include "KDChartLegend.h"
#include "KDChartAbstractArea_p.h"
#include <KDChartTextAttributes.h>
#include <KDChartMarkerAttributes.h>
#include <QMap>
#include <QAbstractTextDocumentLayout>
#include <QPainter>
#include <QVector>
class QGridLayout;
class KDTextDocument;
class QTextDocument;

namespace KDChart {
    class AbstractDiagram;
    class DiagramObserver;
    class LayoutItem;
}

using KDChart::AbstractDiagram;

class KDChart::Legend::Private : public AbstractArea::Private
{
    friend class KDChart::Legend;
public:
    Private();
    ~Private();

private:
    // user-settable
    const QWidget* referenceArea;
    Position position;
    bool useHorizontalSpace;
    bool useVerticalSpace;
    Qt::Orientation orientation;
    bool showLines;
    QMap<uint,QString> texts;
    QMap<uint,QBrush> brushes;
    QMap<uint,QPen> pens;
    QMap<uint, MarkerAttributes> markerAttributes;
    TextAttributes textAttributes;
    QString titleText;
    TextAttributes titleTextAttributes;
    uint spacing;
    KDChart::AbstractDiagram* diagram;

    // internal
    QRectF rect;
    mutable QStringList modelLabels;
    mutable QList<QBrush> modelBrushes;
    mutable QList<QPen> modelPens;
    mutable QList<MarkerAttributes> modelMarkers;
    QVector<KDChart::LayoutItem*> layoutItems;
    QGridLayout* layout;
    KDChart::DiagramObserver* observer;
    bool blockBuildLegend;
};

inline KDChart::Legend::Legend( Private* p, QWidget* parent )
    : AbstractArea( p, parent ) { init(); }
inline KDChart::Legend::Private * KDChart::Legend::d_func()
{ return static_cast<Private*>( AbstractArea::d_func() ); }
inline const KDChart::Legend::Private * KDChart::Legend::d_func() const
{ return static_cast<const Private*>( AbstractArea::d_func() ); }




#endif /* KDCHARTLEGEND_P_H */

