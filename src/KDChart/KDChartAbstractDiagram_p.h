/****************************************************************************
** Copyright (C) 2001-2011 Klaralvdalens Datakonsult AB.  All rights reserved.
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

#ifndef KDCHARTABSTRACTDIAGRAM_P_H
#define KDCHARTABSTRACTDIAGRAM_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the KD Chart API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "KDChartAbstractDiagram.h"
#include "KDChartAbstractCoordinatePlane.h"
#include "KDChartDataValueAttributes.h"
#include "KDChartBackgroundAttributes"
#include "KDChartRelativePosition.h"
#include "KDChartPosition.h"
#include "KDChartPaintContext.h"
#include "KDChartPrintingParameters.h"
#include "KDChartChart.h"
#include <KDChartCartesianDiagramDataCompressor_p.h>
#include "ReverseMapper.h"
#include "KDChartPainterSaver_p.h"

#include <QMap>
#include <QPoint>
#include <QPointer>
#include <QFont>
#include <QFontMetrics>
#include <QPaintDevice>
#include <QModelIndex>


namespace KDChart {
    class AttributesModel;

    class DataValueTextInfo {
    public:
        DataValueTextInfo();
        DataValueTextInfo( const QModelIndex& _index, const DataValueAttributes& _attrs,
                           const QPointF& _pos, const QPointF& _markerPos, double _value );
        DataValueTextInfo( const DataValueTextInfo& other );
        QModelIndex index;
        DataValueAttributes attrs;
        QPointF pos;
        QPointF markerPos;
        double value;
    };

    typedef QVector<DataValueTextInfo> DataValueTextInfoList;
    typedef QVectorIterator<DataValueTextInfo> DataValueTextInfoListIterator;


/**
 * \internal
 */
    class KDChart::AbstractDiagram::Private
    {
        friend class AbstractDiagram;
    public:
        explicit Private();
        virtual ~Private();

        Private( const Private& rhs );

        void setAttributesModel( AttributesModel* );

        bool usesExternalAttributesModel()const;

        // FIXME: Optimize if necessary
        virtual qreal calcPercentValue( const QModelIndex & index );

        void appendDataValueTextInfoToList(
            AbstractDiagram * diagram,
            DataValueTextInfoList & list,
            const QModelIndex & index,
            const CartesianDiagramDataCompressor::CachePosition * position,
            const PositionPoints& points,
            const Position& autoPositionPositive,
            const Position& autoPositionNegative,
            const qreal value,
            qreal favoriteAngle = 0.0 );

        const QFontMetrics * cachedFontMetrics( const QFont& font, QPaintDevice * paintDevice);
        const QFontMetrics cachedFontMetrics() const;

        QString roundValues( double value,
                             const int decimalPos,
                             const int decimalDigits ) const;

        void clearListOfAlreadyDrawnDataValueTexts();

        void paintDataValueTextsAndMarkers( AbstractDiagram* diag,
                                            PaintContext* ctx,
                                            const DataValueTextInfoList & list,
                                            bool paintMarkers,
                                            bool justCalculateRect=false,
                                            QRectF* cumulatedBoundingRect=0 );


        void paintDataValueText( const AbstractDiagram* diag,
                                 QPainter* painter,
                                 const QModelIndex& index,
                                 const QPointF& pos,
                                 double value,
                                 bool justCalculateRect=false,
                                 QRectF* cumulatedBoundingRect=0 );


        void paintDataValueText( const AbstractDiagram* diag,
                                 QPainter* painter,
                                 const DataValueAttributes& attrs,
                                 const QPointF& pos,
                                 QString text,
                                 bool valueIsPositive,
                                 bool justCalculateRect=false,
                                 QRectF* cumulatedBoundingRect=0 );

        virtual QModelIndex indexAt( const QPoint& point ) const;

        QModelIndexList indexesAt(  const QPoint& point ) const;

        QModelIndexList indexesIn( const QRect& rect ) const;

        virtual CartesianDiagramDataCompressor::DataValueAttributesList aggregatedAttrs(
                AbstractDiagram * diagram,
                const QModelIndex & index,
                const CartesianDiagramDataCompressor::CachePosition * position ) const;

        /**
         * Sets arbitrary attributes of a data set.
         */
        void setDatasetAttrs( int dataset, QVariant data, int role );

        /**
         * Retrieves arbitrary attributes of a data set.
         */
        QVariant datasetAttrs( int dataset, int role ) const;

        /**
         * Resets an attribute of a dataset back to its default.
         */
        void resetDatasetAttrs( int dataset, int role );

    protected:
        void init();
        void init( AbstractCoordinatePlane* plane );
        QPointer<AbstractCoordinatePlane> plane;
        mutable QModelIndex attributesModelRootIndex;
        QPointer<AttributesModel> attributesModel;
        bool allowOverlappingDataValueTexts;
        bool antiAliasing;
        bool percent;
        int datasetDimension;
        mutable QPair<QPointF,QPointF> databoundaries;
        mutable bool databoundariesDirty;
        ReverseMapper reverseMapper;

        QMap< Qt::Orientation, QString > unitSuffix;
        QMap< Qt::Orientation, QString > unitPrefix;
        QMap< int, QMap< Qt::Orientation, QString > > unitSuffixMap;
        QMap< int, QMap< Qt::Orientation, QString > > unitPrefixMap;
        QList< QPainterPath > alreadyDrawnDataValueTexts;

    private:
        QString lastRoundedValue;
        qreal lastX;
        QFontMetrics   mCachedFontMetrics;
        QFont          mCachedFont;
        QPaintDevice * mCachedPaintDevice;
    };

    inline AbstractDiagram::AbstractDiagram( Private * p ) : _d( p )
    {
        init();
    }
    inline AbstractDiagram::AbstractDiagram(
        Private * p, QWidget* parent, AbstractCoordinatePlane* plane )
        : QAbstractItemView( parent ), _d( p )
    {
        _d->init( plane );
        init();
    }


    class LineAttributesInfo {
        public :
        LineAttributesInfo();
        LineAttributesInfo( const QModelIndex _index, const QPointF& _value, const QPointF& _nextValue );

        QModelIndex index;
        QPointF value;
        QPointF nextValue;
    };

    typedef QVector<LineAttributesInfo> LineAttributesInfoList;
    typedef QVectorIterator<LineAttributesInfo> LineAttributesInfoListIterator;

}
#endif /* KDCHARTDIAGRAM_P_H */