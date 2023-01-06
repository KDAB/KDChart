/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHARTPLOTTER_P_H
#define KDCHARTPLOTTER_P_H

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

#include "KDChartPlotter.h"

#include <QPainterPath>

#include "KDChartAbstractCartesianDiagram_p.h"
#include "KDChartCartesianDiagramDataCompressor_p.h"
#include "KDChartPlotterDiagramCompressor.h"
#include "KDChartThreeDLineAttributes.h"

#include <KDABLibFakes>

namespace KDChart {

class PaintContext;

/**
 * \internal
 */
class Plotter::Private : public QObject, public AbstractCartesianDiagram::Private
{
    Q_OBJECT
    friend class Plotter;
    friend class PlotterType;

public:
    Private();
    Private(const Private &rhs);
    ~Private() override;

    void setCompressorResolution(
        const QSizeF &size,
        const AbstractCoordinatePlane *plane);

    PlotterType *implementor = nullptr; // the current type
    PlotterType *normalPlotter = nullptr;
    PlotterType *percentPlotter = nullptr;
    PlotterDiagramCompressor plotterCompressor;
    Plotter::CompressionMode useCompression;
    qreal mergeRadiusPercentage;

protected:
    void init();
public Q_SLOTS:
    void changedProperties();
};

KDCHART_IMPL_DERIVED_DIAGRAM(Plotter, AbstractCartesianDiagram, CartesianCoordinatePlane)

class Plotter::PlotterType
{
public:
    explicit PlotterType(Plotter *d)
        : m_private(d->d_func())
    {
        m_private->init();
    }
    virtual ~PlotterType()
    {
    }
    virtual Plotter::PlotType type() const = 0;
    virtual const QPair<QPointF, QPointF> calculateDataBoundaries() const = 0;
    virtual void paint(PaintContext *ctx) = 0;
    Plotter *diagram() const;

    Plotter::CompressionMode useCompression() const;
    void setUseCompression(Plotter::CompressionMode value);
    PlotterDiagramCompressor &plotterCompressor() const;

    Plotter::Private *plotterPrivate() const
    {
        return m_private;
    }

protected:
    // make some elements of m_private available to derived classes:
    AttributesModel *attributesModel() const;
    QModelIndex attributesModelRootIndex() const;
    ReverseMapper &reverseMapper();
    CartesianDiagramDataCompressor &compressor() const;

    int datasetDimension() const;

    Plotter::Private *m_private;
    // TODO: do we need them or not? (currently unused, but maybe there are supposed to be several
    //       compressors
    PlotterDiagramCompressor m_plotterCompressor;
    Plotter::CompressionMode m_useCompression;
};
}

#endif /* KDCHARTLINEDIAGRAM_P_H */
