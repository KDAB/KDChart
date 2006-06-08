/* -*- Mode: C++ -*-
   KDChart1 - a multi-platform charting engine
   */

/****************************************************************************
 ** Copyright (C) 2001-2003 Klarälvdalens Datakonsult AB.  All rights reserved.
 **
 ** This file is part of the KDChart1 library.
 **
 ** This file may be distributed and/or modified under the terms of the
 ** GNU General Public License version 2 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.
 **
 ** Licensees holding valid commercial KDChart1 licenses may use this file in
 ** accordance with the KDChart1 Commercial License Agreement provided with
 ** the Software.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** See http://www.klaralvdalens-datakonsult.se/?page=products for
 **   information about KDChart1 Commercial License Agreements.
 **
 ** Contact info@klaralvdalens-datakonsult.se if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/
#include <qpainter.h>
#include <qbitmap.h>
#include <qpixmap.h>
#include <math.h>
#include <limits.h>

#include <KDDrawText1.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void KDDrawText1::drawRotatedText( QPainter* painter,
        float  degrees,
        QPoint anchor,
        const QString& text,
        const QFont* font,
        int align,
        bool showAnchor,
        const QFontMetrics* fontMet,
        bool noFirstrotate,
        bool noBackrotate,
        KDDrawText1RegionAndTrueRect* infos,
        bool optimizeOutputForScreen )
{
    drawRotatedTxt( painter,
                    optimizeOutputForScreen,
                    degrees,
                    anchor,
                    text,
                    font,
                    align,
                    showAnchor,
                    INT_MAX,
                    INT_MAX,
                    fontMet,
                    false,
                    0 != infos,
                    noFirstrotate,
                    noBackrotate,
                    infos );
}


KDDrawText1RegionAndTrueRect KDDrawText1::measureRotatedText(
        QPainter* painter,
        float  degrees,
        QPoint anchor,
        const QString& text,
        const QFont* font,
        int align,
        const QFontMetrics* fontMet,
        bool noFirstrotate,
        bool noBackrotate,
        int addPercentOfHeightToRegion )
{
    KDDrawText1RegionAndTrueRect infos;
    drawRotatedTxt( painter,
            false,
            degrees,
            anchor,
            text,
            font,
            align,
            false,
            INT_MAX,
            INT_MAX,
            fontMet,
            true,
            false,
            noFirstrotate,
            noBackrotate,
            &infos,
            addPercentOfHeightToRegion );
    return infos;
}


void KDDrawText1::drawRotatedTxt( QPainter* painter,
        bool optimizeOutputForScreen,
        float  degrees,
        QPoint anchor,
        const QString& text,
        const QFont* font,
        int align,
        bool showAnchor,
        int txtWidth,
        int txtHeight,
        const QFontMetrics* fontMet,
        bool calculateOnly,
        bool doNotCalculate,
        bool noFirstrotate,
        bool noBackrotate,
        KDDrawText1RegionAndTrueRect* infos,
        int addPercentOfHeightToRegion )
{

    // this flag no longer needed:
    optimizeOutputForScreen = false;

//    showAnchor=true;
  //qDebug("\nanchor: "+ text + " / "+QString::number(anchor.x())
  //         +" / "+QString::number(anchor.y()));
    bool useInfos = doNotCalculate && infos;
    bool fontChanged = ( 0 != font );
    QFont oldFont;
    if( fontChanged ) {
        oldFont = painter->font();
        painter->setFont( *font );
    }
    else
        font = &painter->font();

    bool mustBackrotate = false;
    if( !optimizeOutputForScreen && !noFirstrotate ){
        painter->rotate( degrees );
        if( !noBackrotate )
            mustBackrotate = true;
    }

    //COMPAT LIB REMOVE: QPoint pos = useInfos ? infos->pos : painter->xFormDev( anchor );
    QPoint pos = useInfos ? infos->pos : anchor;

    if( useInfos )
    {
        txtWidth  = infos->width;
        txtHeight = infos->height;
    }
    else
    {
        int newHeight=0;

        // a bug in the AIX 5.2 compiler means using (?:) syntax doesn't work here
        // therefor we do it the following way:
        QFontMetrics* pFM=0;
        if( ! pFM ) {
            pFM = new QFontMetrics( painter->fontMetrics() );
        } else {
            pFM = const_cast<QFontMetrics*>(fontMet);
        }

        int nLF = text.count('\n');
        if( INT_MAX == txtWidth ) {
            if( nLF ){
                int tw;
                txtWidth = 0;
                int i0  = 0;
                int iLF = text.indexOf('\n');
                while( -1 != iLF ){
                    const QRect r(pFM->boundingRect( text.mid(i0, iLF-i0) ));
                    tw = r.width()+ 2;
                    newHeight = r.height();
                    if( tw > txtWidth )
                        txtWidth = tw;
                    i0 = iLF+1;
                    iLF = text.indexOf('\n', i0);
                }
                if( iLF < (int)text.length() ){
                    const QRect r(pFM->boundingRect( text.mid( i0 ) ));
                    tw = r.width()+2;
                    newHeight = r.height();
                    if( tw > txtWidth )
                        txtWidth = tw;
                    i0 = iLF+1;
                }
            }else{
                const QRect r(painter->boundingRect( 0,0,1,1, Qt::AlignCenter, text ));
                // correct width and height before painting with 2 unit to avoid truncating.
                // PENDING Michel - improve
                txtWidth  = r.width()+2;
                newHeight = r.height()+2;
            }
        }
        if( INT_MAX == txtWidth || INT_MAX == txtHeight ) {
            txtHeight = newHeight ? newHeight : pFM->height() * (1+nLF);
        }
        if( pFM != fontMet )
            delete pFM;
        if( infos ) {
            infos->pos    = pos;
            // PENDING infos
            infos->width = txtWidth;
            infos->height = txtHeight;
        }
    }
    if( showAnchor ) {
        int d = txtHeight/4;
        QPen savePen = painter->pen();
        painter->setPen( QColor( Qt::darkRed ) );
        painter->drawLine( pos.x(),   pos.y()-d,
                           pos.x(),   pos.y()+d );
        painter->drawLine( pos.x()-d, pos.y(),
                           pos.x()+d, pos.y() );
        painter->setPen( savePen );
    }
    int x = useInfos ? infos->x : pos.x();
    int y = useInfos ? infos->y : pos.y();
    //qDebug("1.:     (x / y) :" +  text + " / "+QString::number(x)
    //       +" / "+QString::number(y));
    //qDebug("2.:     (posx / posy) :" +  text );
    // qDebug ( "%d", pos.x() ); qDebug ( "%d", pos.y() );
    //qDebug("3.:     (infosx / infosy) :" +  text + " / "+QString::number(infos->x)
    //           +" / "+QString::number(infos->y));

    if( !useInfos && !optimizeOutputForScreen ) {
        switch( align & ( Qt::AlignLeft | Qt::AlignRight | Qt::AlignHCenter ) ) {
              case Qt::AlignLeft:
                break;
            case Qt::AlignRight:
//qDebug( QPaintDeviceMetrics::logicalDpiX() );
                x -= txtWidth;
                break;
            case Qt::AlignHCenter:
                x -= txtWidth - txtWidth/2;
                break;
        }
        switch( align & ( Qt::AlignTop | Qt::AlignBottom | Qt::AlignVCenter ) ) {
            case Qt::AlignTop:
                break;
            case Qt::AlignBottom:
                y -= txtHeight;
                break;
            case Qt::AlignVCenter:
                y -= txtHeight/2;
                break;
        }
    }
    if( infos && !useInfos ) {
         //COMPAT LIB REMOVE: painter->xForm( pos );
        infos->x = x - 4;
        infos->y = y - 4;
        //PENDING Michel updating info using x , y from pos
        //qDebug("4.:     (infosx / infosy) :" +  text + " / "+QString::number(infos->x)
	//+" / "+QString::number(infos->y));
        //qDebug("5.:  (x / y) :" +  text + " / "+QString::number(x)
	//   +" / "+QString::number(y));
	//qDebug("6.:  (anchorx /anchory) :" +  text + " / "+QString::number(x)
	//   +" / "+QString::number(y));
        QRect rect( painter->boundingRect( x, y,
                    txtWidth, txtHeight,
                    Qt::AlignLeft + Qt::AlignTop,
                    text ) );
        //painter->fillRect (rect, Qt::blue );

        /*COMPAT LIB REMOVE:
        QPoint topLeft(     painter->xForm( rect.topLeft()     ) );
        QPoint topRight(    painter->xForm( rect.topRight()    ) );
        QPoint bottomRight( painter->xForm( rect.bottomRight() ) );
        QPoint bottomLeft(  painter->xForm( rect.bottomLeft()  ) );
        */
        QPoint topLeft(     rect.topLeft()     );
        QPoint topRight(    rect.topRight()    );
        QPoint bottomRight( rect.bottomRight() );
        QPoint bottomLeft(  rect.bottomLeft()  );

        int additor = addPercentOfHeightToRegion * txtHeight / 100;
        QPolygon points;
        points.setPoints( 4, topLeft.x()-additor,     topLeft.y()-additor,
                topRight.x()+additor,    topRight.y()-additor,
                bottomRight.x()+additor, bottomRight.y()+additor,
                bottomLeft.x()-additor,  bottomLeft.y()+additor );
        infos->region = QRegion( points );
    }

    // When the Qt initialization bug is fixed the following scope
    // will be put into an "if( showAnchor )" entirely.
    {
        int d = txtHeight/4;
        QPen savePen = painter->pen();
        if( showAnchor ) {
            painter->setPen( QColor( Qt::blue ) );
            painter->drawLine( x,   y-d,
                               x,   y+d );
            painter->drawLine( x-d, y,
                               x+d, y );
            painter->setPen( QColor( Qt::darkGreen ) );
            painter->drawRect(x,y,txtWidth,txtHeight);
            //painter->drawText( x, y-d, text);

/*
        }else{
            // Working around a strange Qt bug: Rotated painter must be
            // initialized by drawing before text can be painted there.
            painter->setPen( QColor( Qt::white ) );
            painter->drawLine( 30000,0,30001,0 );
*/
        }
        painter->setPen( savePen );
    }

    if( mustBackrotate && optimizeOutputForScreen ){
        painter->rotate( -degrees );
        mustBackrotate = false;
    }

    if( !calculateOnly ){
        //qDebug("txtWidth: %i  txtHeight: %i", txtWidth, txtHeight);
        if( !optimizeOutputForScreen ){
/*
            painter->drawText( x, y,
                               txtWidth, txtHeight,
                               Qt::AlignLeft + Qt::AlignTop,
                               text );
*/
            painter->drawText( x, y,
                               txtWidth, txtHeight,
                               Qt::AlignLeft + Qt::AlignTop,
                               text );
/*
            painter->drawText( x, y,
                               text,
                               -1,
                               Qt::AlignRight + Qt::AlignTop );
*/
        }else{
            /*
            this code no longer needed:

            // new code (rotating the text ourselves for better quality on screens)
            QPixmap pm( txtWidth+2, txtHeight+2, 1 );
            // note: When using colored axis labels it will be necessary
            //       to change this code and use a 256 color pixmap instead
            //       of a monochrome one.                 (khz, 2002/08/15)
            pm.fill(Qt::color0);
            QPainter p;
            p.begin( &pm );
            if( showAnchor ){
                p.drawRect(0,0, txtWidth,txtHeight);
                p.drawLine(0,0, txtWidth,txtHeight);
                p.drawLine(0,txtHeight, txtWidth,0);
            }
            p.setFont(painter->font());

            p.drawText( 0, 0, txtWidth, txtHeight,
                        Qt::AlignLeft + Qt::AlignTop,
                        text );

            QBitmap mask;
            mask = pm;
            pm.setMask( mask );
            QWMatrix m;
            m.rotate( degrees );
            QPixmap theRotatedPixmap = pm.xForm(m);

            // where are our four corner points now:
            double degreesRad = degrees;
            while( degreesRad > 360 )
                degreesRad -= 360;
            degreesRad *= M_PI / 180.0;
            double cosA = cos( degreesRad );
            double sinA = sin( degreesRad );
            QPoint pTopLeft(  0,
                              0 );
            QPoint pBotLeft(  static_cast < int > ( 0         * cosA  -  txtHeight * sinA ),
                              static_cast < int > ( txtHeight * cosA  +  0         * sinA  ) );
            QPoint pTopRight( static_cast < int > ( txtWidth  * cosA  -  0         * sinA ),
                              static_cast < int > ( 0         * cosA  +  txtWidth  * sinA  ) );
            QPoint pBotRight( static_cast < int > ( txtWidth  * cosA  -  txtHeight * sinA ),
                              static_cast < int > ( txtHeight * cosA  +  txtWidth  * sinA  ) );

            // make our four corner points relative
            // to the bounding rect of the rotated pixmap
            {
                QPoint pDeltaTL( qMin(0, qMin(pBotLeft.x(), qMin(pTopRight.x(), pBotRight.x()))),
                                 qMin(0, qMin(pBotLeft.y(), qMin(pTopRight.y(), pBotRight.y()))) );
                pTopLeft  -= pDeltaTL;
                pBotLeft  -= pDeltaTL;
                pTopRight -= pDeltaTL;
                pBotRight -= pDeltaTL;
            }


            // The horizontal and vertical alignment together define one of
            // NINE possible points: this point must be moved on the anchor.
            int hAlign = align & ( Qt::AlignLeft | Qt::AlignRight  | Qt::AlignHCenter );
            int vAlign = align & ( Qt::AlignTop  | Qt::AlignBottom | Qt::AlignVCenter );

            QPoint pixPoint;
            switch( hAlign ) {
                case Qt::AlignLeft:
                    switch( vAlign ) {
                        case Qt::AlignTop:
                            pixPoint = pTopLeft;
                            break;
                        case Qt::AlignBottom:
                            pixPoint = pBotLeft;
                            break;
                        case Qt::AlignVCenter:
                        default:
                            pixPoint = QPoint( (pTopLeft.x() + pBotLeft.x()) / 2,
                                               (pTopLeft.y() + pBotLeft.y()) / 2 );
                            break;
                    }
                    break;
                case Qt::AlignRight:
                    switch( vAlign ) {
                        case Qt::AlignTop:
                            pixPoint = pTopRight;
                            break;
                        case Qt::AlignBottom:
                            pixPoint = pBotRight;
                            break;
                        case Qt::AlignVCenter:
                        default:
                            pixPoint = QPoint( (pTopRight.x() + pBotRight.x()) / 2,
                                               (pTopRight.y() + pBotRight.y()) / 2 );
                            break;
                    }
                    break;
                case Qt::AlignHCenter:
                default:
                    switch( vAlign ) {
                        case Qt::AlignTop:
                            pixPoint = QPoint( (pTopLeft.x() + pTopRight.x()) / 2,
                                               (pTopLeft.y() + pTopRight.y()) / 2 );
                            break;
                        case Qt::AlignBottom:
                            pixPoint = QPoint( (pBotLeft.x() + pBotRight.x()) / 2,
                                               (pBotLeft.y() + pBotRight.y()) / 2 );
                            break;
                        case Qt::AlignVCenter:
                        default:
                            pixPoint = QPoint( (pTopLeft.x() + pBotRight.x()) / 2,
                                               (pTopLeft.y() + pBotRight.y()) / 2 );
                            break;
                    }
                    break;
            }
	    //qDebug("2.:     (x / y) : "+QString::number(x)
	    //             +" / "+QString::number(y));
            painter->drawPixmap( QPoint( x - pixPoint.x(),
                                         y - pixPoint.y() ),
                                 theRotatedPixmap );
            p.end();
            */
        }
    }

    if( mustBackrotate )
        painter->rotate( -degrees );

    if( fontChanged )
        painter->setFont( oldFont );
}


