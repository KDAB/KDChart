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
#ifndef KDCHART1_PRESENTATION_H
#define KDCHART1_PRESENTATION_H


class QComboBox;
class QListWidget;
class QListWidgetItem;
class QToolButton;
class QSpinBox;
class KDChart1Widget;
class QWidgetStack;

#include <qwidget.h>
#include <KDChart1Params.h>
#include <KDChart1Table.h>


static const QColor __chamois( 255,248,222 );


class KDChart1PresentationWidget : public QWidget
{
    Q_OBJECT
    public:
        KDChart1PresentationWidget( int argc, char **argv );
        ~KDChart1PresentationWidget();
    protected:
        void timerEvent(  QTimerEvent * );
        void resizeEvent( QResizeEvent * );
    private slots:
        void slotMouseLeftPressed(    uint row, uint col );
        /*    void slotMouseLeftReleased(   uint row, uint col );
              void slotMouseLeftClicked(    uint row, uint col );   */
        void slotMouseMiddlePressed(  uint row, uint col );
        /*    void slotMouseMiddleReleased( uint row, uint col );
              void slotMouseMiddleClicked(  uint row, uint col );   */
        void slotMouseRightPressed(   uint row, uint col );
        /*    void slotMouseRightReleased(  uint row, uint col );
              void slotMouseRightClicked(   uint row, uint col );   */
        void setConfiguration ( QListWidgetItem * item, QListWidgetItem * prevItem );
        void setConfiguration( const QString & );
        void runClicked ();
        void sizeEdited(int value);
    private:
        void setConfiguration_0();
        void setConfiguration_1();
        void setConfiguration_2();
        void setConfiguration_3();
        void setConfiguration_4();
        void setConfiguration_5();
        void setConfiguration_6();
        void setConfiguration_7();
        void setConfiguration_8();
        void setConfiguration_9();
        void setConfiguration_10();
        void setConfiguration_11();
        void setConfiguration_12();
        void setConfiguration_13();
        void setConfiguration_14();
        void setConfiguration_15();
        void setConfiguration_16();
        void setConfiguration_17();
        void setConfiguration_18();
        void setConfiguration_19();
        void setConfiguration_20();
        void setConfiguration_21();
        void setConfiguration_22();
        void setConfiguration_23();
        void setConfiguration_24_27( int selected );
        void setConfiguration_27();
        void setConfiguration_28();
        void setConfiguration_29();
        void setConfiguration_30();
        void setConfiguration_31();
        void setConfiguration_32();
        void setConfiguration_33();
        void setConfiguration_34();
        void setConfiguration_35();
        void setConfiguration_36();
        void setConfiguration_37();
        void setConfiguration_38();
        void setConfiguration_39();
        void setConfiguration_40();
        void reportMouseEvent( QString txt, uint row, uint col );
        void resetParams( KDChart1Params::ChartType cType0,
                KDChart1Params::ChartType cType1 = KDChart1Params::NoType,
                QPen                     pen = QPen( Qt::black ),
                QBrush                   brush = QBrush( QColor( __chamois ) ),
                const QPixmap*           backPixmap = 0,
                KDFrame1::BackPixmapMode  backPixmapMode = KDFrame1::PixStretched );
        void resetData( uint rows, uint cols );
        int _argc;
        char **_argv;
        bool               _internalSizeUpdate;
        bool               _auto;
        bool               _splash;
        KDChart1Params    * _p;
        KDChart1TableData * _d;
        QListWidget      * _l;
        QComboBox        * _box;
        QToolButton      * _run;
        QSpinBox         * _size;
        KDChart1Widget    * _kdc;
        int                _tId;
        int                _defaultSize0;
        int                _defaultSize1;
};


#endif
