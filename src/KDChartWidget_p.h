#ifndef __KDCHARTWIDGET_P_H__
#define __KDCHARTWIDGET_P_H__

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

#include <KDChartWidget.h>
#include <KDChartChart.h>
#include <KDChartCartesianCoordinatePlane.h>
#include <KDChartPolarCoordinatePlane.h>

#include <KDABLibFakes>

#include <QGridLayout>
#include <QStandardItemModel>

/**
 * \internal
 */
class KDChart::Widget::Private
{
    friend class ::KDChart::Widget;
    Widget * const q;
public:
    explicit Private( Widget * qq );
    ~Private(); // non-virtual, since nothing inherits this

protected:
    QGridLayout layout;
    QStandardItemModel m_model;
    Chart m_chart;
    CartesianCoordinatePlane m_cartPlane;
    PolarCoordinatePlane m_polPlane;

    int usedDatasetWidth;
};


#endif // KDChartWidget_p_H
