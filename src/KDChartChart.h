#ifndef KDCHARTCHART_H
#define KDCHARTCHART_H

#include <QWidget>

#include "kdchart_export.h"

namespace KDChart {

    class AbstractDiagram;
    class CoordinatePlane;
    class HeaderFooter;
    class Legend;

    class CoordinatePlaneList : public QList<CoordinatePlane*> {};
    class HeaderFooterList : public QList<HeaderFooter*> {};
    class LegendList : public QList<Legend*> {};

    class KDCHART_EXPORT Chart : public QWidget
    {
        Q_OBJECT

    public:
        class Private;

        explicit Chart ( QWidget* parent = 0 );
        virtual ~Chart();

        CoordinatePlane* coordinatePlane();
        void addCoordinatePlane( CoordinatePlane* );
        void replaceCoordinatePlane( CoordinatePlane*, int position = 0 );
        CoordinatePlaneList coordinatePlanes();

        void setGlobalLeading( int left, int top, int right, int bottom );

        void setGlobalLeadingLeft( int leading );
        int globalLeadingLeft() const;
        void setGlobalLeadingTop( int leading );
        int globalLeadingTop() const;
        void setGlobalLeadingRight( int leading );
        int globalLeadingRight() const;
        void setGlobalLeadingBottom( int leading );
        int globalLeadingBottom() const;

        virtual void addHeaderFooter( HeaderFooter* headerFooter );
        virtual void replaceHeaderFooter ( HeaderFooter* headerFooter, int position = 0 );
        virtual void removeHeaderFooter( int position = 0 );
        HeaderFooter* headerFooter();
        HeaderFooterList headerFooters();

        virtual void addLegend( Legend* legend );
        virtual void replaceLegend ( Legend* legend, int position = 0 );
        virtual void removeLegend( int position = 0 );
        Legend* legend();
        LegendList legends();

    protected:
        void paintEvent( QPaintEvent* );

    private:
        Private* p;
    };

}

#endif
