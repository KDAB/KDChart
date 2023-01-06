/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "KDChartPlotter_p.h"
#include "KDChartPlotter.h"

#include "KDChartPainterSaver_p.h"
#include "KDChartValueTrackerAttributes.h"
#include "PaintingHelpers_p.h"

using namespace KDChart;

Plotter::Private::Private(const Private &rhs)
    : QObject()
    , AbstractCartesianDiagram::Private(rhs)
    , useCompression(rhs.useCompression)
{
}

void Plotter::Private::init()
{
    AbstractCartesianDiagram::Private::init();
    useCompression = Plotter::NONE;
}

void Plotter::Private::setCompressorResolution(
    const QSizeF &size,
    const AbstractCoordinatePlane *plane)
{
    compressor.setResolution(static_cast<int>(size.width() * plane->zoomFactorX()),
                             static_cast<int>(size.height() * plane->zoomFactorY()));
}

void Plotter::Private::changedProperties()
{
    if (auto *plane = dynamic_cast<CartesianCoordinatePlane *>(diagram->coordinatePlane())) {
        QPair<qreal, qreal> verticalRange = plane->verticalRange();
        if (verticalRange.first != verticalRange.second)
            implementor->plotterCompressor().setForcedDataBoundaries(verticalRange, Qt::Vertical);
        QPair<qreal, qreal> horizontalRange = plane->horizontalRange();
        if (verticalRange.first != horizontalRange.second)
            implementor->plotterCompressor().setForcedDataBoundaries(horizontalRange, Qt::Horizontal);
    }
}

AttributesModel *Plotter::PlotterType::attributesModel() const
{
    return m_private->attributesModel;
}

ReverseMapper &Plotter::PlotterType::reverseMapper()
{
    return m_private->reverseMapper;
}

Plotter *Plotter::PlotterType::diagram() const
{
    return static_cast<Plotter *>(m_private->diagram);
}

CartesianDiagramDataCompressor &Plotter::PlotterType::compressor() const
{
    return m_private->compressor;
}

PlotterDiagramCompressor &Plotter::PlotterType::plotterCompressor() const
{
    return m_private->plotterCompressor;
}

Plotter::CompressionMode Plotter::PlotterType::useCompression() const
{
    return m_private->useCompression;
}

void Plotter::PlotterType::setUseCompression(Plotter::CompressionMode value)
{
    m_private->useCompression = value;
}
