/****************************************************************************
 ** Copyright (C) 2007 Klaralvdalens Datakonsult AB.  All rights reserved.
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

#include "KDChartAbstractCoordinatePlane.h"
#include "KDChartDataValueAttributes.h"
#include "KDChartRelativePosition.h"
#include "KDChartPosition.h"
#include "KDChartPainterSaver_p.h"
#include "KDChartPaintContext.h"
#include "KDChartPrintingParameters.h"
#include "KDChartChart.h"
#include "Scenery/ReverseMapper.h"

#include <QPoint>
#include <QPointer>
#include <QFont>
#include <QFontMetrics>
#include <QPaintDevice>
#include <QModelIndex>
#include <QAbstractTextDocumentLayout>
#include <QTextBlock>

#include <KDABLibFakes>


namespace KDChart {
    class AttributesModel;

    class DataValueTextInfo {
    public:
        DataValueTextInfo(){}
        DataValueTextInfo( const QModelIndex& _index, const QPointF& _pos, const QPointF& _markerPos, double _value )
            :index( _index ), pos( _pos ), markerPos( _markerPos ), value( _value )
        {}
        DataValueTextInfo( const DataValueTextInfo& other )
            :index( other.index ), pos( other.pos ), markerPos( other.markerPos ), value( other.value ) {}
        QModelIndex index;
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

        void appendDataValueTextInfoToList(
            AbstractDiagram * diagram,
            DataValueTextInfoList & list,
            const QModelIndex & index,
            const PositionPoints& points,
            const Position& autoPositionPositive,
            const Position& autoPositionNegative,
            const qreal value )
        {
            const DataValueAttributes attrs( diagram->dataValueAttributes( index ) );
            /*if( attrs.isVisible() )*/ {
                const bool bValueIsPositive = (value >= 0.0);
                RelativePosition relPos( attrs.position( bValueIsPositive ) );
                relPos.setReferencePoints( points );
                if( relPos.referencePosition().isUnknown() )
                    relPos.setReferencePosition( bValueIsPositive ? autoPositionPositive : autoPositionNegative );

                const QPointF referencePoint = relPos.referencePoint();
                if( diagram->coordinatePlane()->isVisiblePoint( referencePoint ) ){
                    const qreal fontHeight = cachedFontMetrics( attrs.textAttributes().
                            calculatedFont( plane, KDChartEnums::MeasureOrientationMinimum ), diagram )->height();
                    // Note: When printing data value texts the font height is used as reference size for both,
                    //       horizontal and vertical padding, if the respective padding's Measure is using
                    //       automatic reference area detection.
                    QSizeF relativeMeasureSize( fontHeight, fontHeight );
                    //qDebug()<<"fontHeight"<<fontHeight;

                    // Store the anchor point, that's already shifted according to horiz./vert. padding:
                    list.append( DataValueTextInfo(
                                     index,
                                     relPos.calculatedPoint( relativeMeasureSize ),
                                     referencePoint,
                                     value ) );
                }
            }
        }

        const QFontMetrics * cachedFontMetrics( const QFont& font, QPaintDevice * paintDevice)
        {
            if( (font != mCachedFont) || (paintDevice != mCachedPaintDevice) )
                mCachedFontMetrics = QFontMetrics( font, paintDevice );
            return & mCachedFontMetrics;
        }
        const QFontMetrics cachedFontMetrics() const
        {
            return mCachedFontMetrics;
        }

        QString roundValues( double value,
                             const int decimalPos,
                             const int decimalDigits ) const
        {
            QString digits( QString::number( value ).mid( decimalPos+1 ) );
            QString num( QString::number( value ) );
            num.truncate( decimalPos );
            int count = 0;
            for (  int i = digits.length(); i >= decimalDigits ; --i ) {
                count += 1;
                int lastval = QString( digits.data() [i] ).toInt();
                int val = QString( digits.data() [i-1] ) .toInt();
                if ( lastval >= 5 ) {
                    val += 1;
                    digits.replace( digits.length() - count,1 , QString::number( val ) );
                }
            }

            digits.truncate( decimalDigits );
            num.append( QLatin1Char( '.' ) + digits );

            return num;

        }

        void clearListOfAlreadyDrawnDataValueTexts()
        {
            alreadyDrawnDataValueTexts.clear();
        }

        void paintDataValueTextsAndMarkers( AbstractDiagram* diag, PaintContext* ctx,
                                            const DataValueTextInfoList & list, bool paintMarkers )
        {
            const PainterSaver painterSaver( ctx->painter() );
            ctx->painter()->setClipping( false );
            if( paintMarkers )
            {
                DataValueTextInfoListIterator it( list );
                while ( it.hasNext() ) {
                    const DataValueTextInfo& info = it.next();
                    diag->paintMarker( ctx->painter(), info.index, info.markerPos );
                }
            }
            DataValueTextInfoListIterator it( list );

            Measure m( 18.0, KDChartEnums::MeasureCalculationModeRelative,
                       KDChartEnums::MeasureOrientationMinimum );
            m.setReferenceArea( ctx->coordinatePlane() );
            TextAttributes ta;
            ta.setFontSize( m );
            m.setAbsoluteValue( 6.0 );
            ta.setMinimalFontSize( m );
            clearListOfAlreadyDrawnDataValueTexts();
            while ( it.hasNext() ) {
                const DataValueTextInfo& info = it.next();
                paintDataValueText( diag, ctx->painter(), info.index, info.pos, info.value );

                const QString comment = info.index.data( KDChart::CommentRole ).toString();
                if( comment.isEmpty() )
                    continue;
                TextBubbleLayoutItem item( comment,
                                           ta,
                                           ctx->coordinatePlane()->parent(),
                                           KDChartEnums::MeasureOrientationMinimum,
                                           Qt::AlignHCenter|Qt::AlignVCenter );
                item.setGeometry( QRect( info.pos.toPoint(), item.sizeHint() ) );
                item.paint( ctx->painter() );
            }
        }


        void paintDataValueText( const AbstractDiagram* diag,
                                 QPainter* painter,
                                 const QModelIndex& index,
                                 const QPointF& pos,
                                 double value )
        {
            const DataValueAttributes a( diag->dataValueAttributes( index ) );

            if ( !a.isVisible() ) return;

            // handle decimal digits
            int decimalDigits = a.decimalDigits();
            int decimalPos = QString::number(  value ).indexOf( QLatin1Char( '.' ) );
            QString roundedValue;
            if ( a.dataLabel().isNull() ){
                if ( decimalPos > 0 && value != 0 )
                    roundedValue = roundValues ( value, decimalPos, decimalDigits );
                else
                    roundedValue = QString::number(  value );
            }else{
                roundedValue = a.dataLabel();
            }

            // handle prefix and suffix
            if ( !a.prefix().isNull() )
                roundedValue.prepend( a.prefix() );

            if ( !a.suffix().isNull() )
                roundedValue.append( a.suffix() );

            paintDataValueText( diag, painter, a, pos, roundedValue, value >= 0.0 );
        }


        void paintDataValueText( const AbstractDiagram* diag,
                                 QPainter* painter,
                                 const DataValueAttributes& attrs,
                                 const QPointF& pos,
                                 QString text,
                                 bool valueIsPositive )
        {
            if ( !attrs.isVisible() ) return;

            const TextAttributes ta( attrs.textAttributes() );
            if ( ta.isVisible() ) {
                /* for debugging:
                PainterSaver painterSaver( painter );
                painter->setPen( Qt::black );
                painter->drawLine( pos - QPointF( 2,2), pos + QPointF( 2,2) );
                painter->drawLine( pos - QPointF(-2,2), pos + QPointF(-2,2) );
                */

                QTextDocument doc;
                if( Qt::mightBeRichText( text ) )
                    doc.setHtml( text );
                else
                    doc.setPlainText( text );

                const RelativePosition relPos( attrs.position( valueIsPositive ) );
                const Qt::Alignment alignBottomLeft = Qt::AlignBottom | Qt::AlignLeft;
                const QFont calculatedFont( ta.calculatedFont( plane, KDChartEnums::MeasureOrientationMinimum ) );

                // note: We can not use boundingRect() to retrieve the width, as that returnes a too small value
                const QSizeF plainSize(
                        cachedFontMetrics( calculatedFont, painter->device() )->width( doc.toPlainText() ),
                cachedFontMetrics( calculatedFont, painter->device() )->boundingRect( doc.toPlainText() ).height() );

                // FIXME draw the non-text bits, background, etc

                if ( attrs.showRepetitiveDataLabels() || pos.x() <= lastX || lastRoundedValue != text ) {
                    //qDebug() << text;
                    lastRoundedValue = text;
                    lastX = pos.x();
                    const PainterSaver painterSaver( painter );
                    painter->setPen( PrintingParameters::scalePen( ta.pen() ) );

                    doc.setDefaultFont( calculatedFont );
                    QAbstractTextDocumentLayout::PaintContext context;
                    context.palette = diag->palette();
                    context.palette.setColor(QPalette::Text, ta.pen().color() );

                    QAbstractTextDocumentLayout* const layout = doc.documentLayout();

                    painter->translate( pos );
                    painter->rotate( ta.rotation() );
                    qreal dx = 0.0;
                    qreal dy = 0.0;
                    const Qt::Alignment alignTopLeft = (Qt::AlignLeft | Qt::AlignTop);
                    if(     (relPos.alignment() & alignTopLeft) != alignTopLeft ){
                        if( relPos.alignment() & Qt::AlignRight )
                            dx = - plainSize.width();
                        else if( relPos.alignment() & Qt::AlignHCenter )
                            dx = - 0.5 * plainSize.width();

                        if( relPos.alignment() & Qt::AlignBottom )
                            dy = - plainSize.height();
                        else if( relPos.alignment() & Qt::AlignVCenter )
                            dy = - 0.5 * plainSize.height();
                    }

                    bool drawIt = true;
                    // note: This flag can be set differently for every label text!
                    // In theory a user could e.g. have some small red text on one of the
                    // values that she wants to have written in any case - so we just
                    // do not test if such texts would cover some of the others.
                    if( ! attrs.showOverlappingDataLabels() ){
                        const QRectF br( layout->blockBoundingRect( doc.begin() ) );
                        qreal radRot = DEGTORAD( - ((ta.rotation() < 0) ? ta.rotation()+360 : ta.rotation()) );
                        //qDebug() << radRot;
                        qreal cosRot = cos( radRot );
                        qreal sinRot = sin( radRot );
                        QPolygon pr( br.toRect(), true );
                        // YES, people, the following stuff NEEDS to be done that way!
                        // Otherwise we will not get the texts' individual rotation
                        // and/or the shifting of the texts correctly.
                        // Just believe me - I did tests ..   :-)    (khz, 2008-02-19)
                        for( int i=0; i<pr.count(); ++i ){
                            const QPoint p( pr.point( i ) );
                            const qreal x = p.x()+dx;
                            const qreal y = p.y()+dy;
                            pr.setPoint(i,
                                        static_cast<int>(pos.x() + x*cosRot + y*sinRot),
                                        static_cast<int>(pos.y() - x*sinRot + y*cosRot));
                        }
                        KDAB_FOREACH( QPolygon oldPoly, alreadyDrawnDataValueTexts ) {
                            if( ! oldPoly.intersected( pr ).isEmpty() )
                                drawIt = false;
                        }
                        if( drawIt )
                            alreadyDrawnDataValueTexts << pr;
                    }
                    if( drawIt ){
                        painter->translate( QPointF( dx, dy ) );
                        layout->draw( painter, context );
                    }
                }

            }
        }

        virtual QModelIndex indexAt( const QPoint& point ) const
        {
            QModelIndexList l = indexesAt( point );
            if ( !l.isEmpty() )
                return l.first();
            else
                return QModelIndex();
        }

        QModelIndexList indexesAt(  const QPoint& point ) const
        {
            return reverseMapper.indexesAt(  point ); // which could be empty
        }

        QModelIndexList indexesIn( const QRect& rect ) const
        {
            return reverseMapper.indexesIn( rect );
        }

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
        QList< QPolygon > alreadyDrawnDataValueTexts;

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
        LineAttributesInfo() {}
        LineAttributesInfo( const QModelIndex _index, const QPointF& _value, const QPointF& _nextValue )
            :index( _index ), value ( _value ), nextValue ( _nextValue )  {}

        QModelIndex index;
        QPointF value;
        QPointF nextValue;
    };

    typedef QVector<LineAttributesInfo> LineAttributesInfoList;
    typedef QVectorIterator<LineAttributesInfo> LineAttributesInfoListIterator;

}
#endif /* KDCHARTDIAGRAM_P_H */
