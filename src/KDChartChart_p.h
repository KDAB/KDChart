#ifndef KDCHARTCHART_P_H
#define KDCHARTCHART_P_H

#include <QObject>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "KDChartChart.h"

namespace KDChart {


class Chart::Private : public QObject
{
    Q_OBJECT
    public:
        CoordinatePlaneList coordinatePlanes;
        HeaderFooterList headerFooters;
        LegendList legends;

        Chart* chart;
        QHBoxLayout* layout;
        QVBoxLayout* vLayout;
        QBoxLayout* planesLayout;
        QGridLayout* headerLayout;
        QGridLayout* footerLayout;
        QGridLayout* dataAndLegendLayout;

        int globalLeadingLeft, globalLeadingRight, globalLeadingTop, globalLeadingBottom;

        Private ( Chart* );

        virtual ~Private() {}

        void createLayouts( QWidget * parent );
        void layoutLegends();
        void layoutHeadersAndFooters();

        struct AxisInfo {
            AxisInfo()
                :plane(0)
            {}
            AbstractCoordinatePlane *plane;
        };

        struct PlaneInfo {
            PlaneInfo()
                : referencePlane(0)
                    , horizontalOffset(1)
                    , verticalOffset(1)
                    , grid( 0 )
                    {}
                    AbstractCoordinatePlane *referencePlane;
                    int horizontalOffset;
                    int verticalOffset;
                    QGridLayout *grid;
        };

        QHash<AbstractCoordinatePlane*, PlaneInfo> buildPlaneLayoutInfos();

    public slots:
        void slotLayoutPlanes();
        void slotRelayout();
        void slotUnregisterDestroyedLegend( Legend * legend );
        void slotUnregisterDestroyedHeaderFooter( HeaderFooter* headerFooter );
        void slotUnregisterDestroyedPlane( AbstractCoordinatePlane* plane );

};

}

#endif
