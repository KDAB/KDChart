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
#ifndef __KDCHART1TEXTPIECE_H__
#define __KDCHART1TEXTPIECE_H__

#include <qstring.h>
#include <qfont.h>
#include <qregion.h>
#include <qpalette.h>
#include <qobject.h>
#include <kdchart1_export.h>

class QBrush;
class QPainter;
class QSimpleRichText;
class QFontMetrics;

class KDCHART1_EXPORT KDChart1TextPiece :public QObject
{
    Q_OBJECT

    public:
        KDChart1TextPiece();
        KDChart1TextPiece( const QString& text, const QFont& font );
        KDChart1TextPiece( QPainter* painter, const QString& text, const QFont& font );
        virtual ~KDChart1TextPiece();
        /**
          Copy the settings of text piece \c source into this box.

          \note Use this method instead of using the assignment operator.

          \sa clone
          */
        void deepCopy( const KDChart1TextPiece* source );


        /**
          Create a new text piece on the heap, copy the settings stored by
          this text piece into the newly created text piece and return
          the pointer to the new text piece.

          \note Use this method instead of using the copy constructor.

          \sa deepCopy
          */
        const KDChart1TextPiece* clone() const;

    private:
        KDChart1TextPiece( const KDChart1TextPiece& ) : QObject(0) {}

    public slots:
        virtual int width() const;
        virtual int height() const;
        virtual int fontLeading() const;
        virtual QRect rect( QPainter *p, const QRect& clipRect) const;
        virtual void draw( QPainter *p, int x, int y, const QRect& clipRect,
                const QColor& color, const QBrush* paper = 0 ) const;
        virtual void draw( QPainter *p, int x, int y, const QRegion& clipRegion,
                const QColor& color, const QBrush* paper = 0 ) const;

        QString text() const;
        QFont font() const;
        bool isRichText() const;


    protected:
        bool _isRichText;
        /*QSimpleRichText* _richText; // used when _isRichText == true*/
        QString _text; // used when _isRichText == false
        QFont _font; // used when _isRichText == false
        QFontMetrics* _metrics;
        bool _dirtyMetrics;
};

#endif
