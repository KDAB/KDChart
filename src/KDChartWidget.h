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
#ifndef __KDCHARTWIDGET_H__
#define __KDCHARTWIDGET_H__

#include <QPair>
#include <QVector>
#include <QWidget>

#include "KDChartHeaderFooter.h"
#include "KDChartLegend.h"

namespace KDChart {

    // some forward declarations
    class AbstractDiagram;
    class Chart;
    class AbstractCoordinatePlane;
    class TableModel;

    /**
     * \class Widget KDChartWidget.h
     * \brief The KD Chart widget for usage without InterView.
     *
     * If you want to use KD Chart with InterView, use KDChart::Chart instead.
     */
    class KDCHART_EXPORT Widget : public QWidget
    {
        Q_OBJECT

        Q_DISABLE_COPY( Widget )
        KDCHART_DECLARE_PRIVATE_BASE_POLYMORPHIC_QWIDGET( Widget )

    public:
        /**
         * Default Constructor
         *
         * Creates a new widget with all data initialized empty.
         *
         * \param parent the widget parent; passed on to QWidget
         */
        Widget( QWidget* parent = NULL );

        /** Destructor. */
        ~Widget();
        /** Sets the data in the given column using a QVector of double. */
        void setDataset( int column, const QVector< double > & data, const QString& title = QString()  );
        /** Sets the data in the given column using a QVector of QPairs
         *  of double. */
        void setDataset( int column, const QVector< QPair< double, double > > &  data, const QString& title = QString() );

    public slots:
        /** Sets all global leadings (borders). */
        void setGlobalLeading( int left, int top, int right, int bottom );
        /** Sets the left leading (border). */
        void setGlobalLeadingLeft( int leading );
        /** Sets the top leading (border). */
        void setGlobalLeadingTop( int leading );
        /** Sets the right leading (border). */
        void setGlobalLeadingRight( int leading );
        /** Sets the bottom leading (border). */
        void setGlobalLeadingBottom( int leading );

    public:
        /** Returns the left leading (border). */
        int globalLeadingLeft() const;
        /** Returns the top leading (border). */
        int globalLeadingTop() const;
        /** Returns the right leading (border). */
        int globalLeadingRight() const;
        /** Returns the bottom leading (border). */
        int globalLeadingBottom() const;

        /** Returns the first of all headers. */
        KDChart::HeaderFooter* firstHeaderFooter() const;
        /** Returns a list with all headers. */
        const QList<KDChart::HeaderFooter*> allHeadersFooters() const;
        /** Returns the count over all headers. */
        int headerFooterCount() const;
        /** Adds a new header/footer with the given text to the position. */
        void addHeaderFooter( const QString& text,
                              HeaderFooter::HeaderFooterType type,
                              Position position );
        /** Adds an existing header / footer object. */
        void addHeaderFooter( HeaderFooter* header );

        void replaceHeaderFooter( HeaderFooter* header, int position = 0 );
        void removeHeaderFooter( int position = 0 );

        /** Returns the first of all legends. */
        KDChart::Legend* firstLegend() const;
        /** Returns a list with all legends. */
        const QList<KDChart::Legend*> allLegends() const;
        /** Returns the count over all legends. */
        int legendCount() const;
        /** Adds an empty legend on the given position. */
        void addLegend( Position position );
        /** Adds a new, already existing, legend. */
        void addLegend (Legend* legend );

        void replaceLegend( Legend* legend, int position = 0 );
	void removeLegend( int position = 0 );


        /** Returns a pointer to the current diagram. */
        AbstractDiagram* diagram() const;
        /** Returns a pointer to the current coordinate plane. */
        AbstractCoordinatePlane* coordinatePlane() const;


	enum ChartType { NoType, Bar, Line, Pie, Ring, Polar };

        /** Returns the type of the chart. */
        ChartType type() const;

    public slots:
        /** Sets the type of the chart. */
        void setType( ChartType chartType );

    protected:
        /** Justifies the model, so that the given rows and columns fit into it. */
        void justifyModelSize( int rows, int columns );

    private:
        /** Checks, wheter the given width matches with the one used until now. */
        bool checkDatasetWidth( int width );
    };
}

#endif // KDChartWidget_H
