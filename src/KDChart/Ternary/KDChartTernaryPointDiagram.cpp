/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "KDChartTernaryPointDiagram.h"
#include "KDChartTernaryPointDiagram_p.h"

#include <limits>

#include <QPainter>

#include <KDChartPaintContext.h>

#include "TernaryConstants.h"
#include "TernaryPoint.h"

using namespace KDChart;

#define d d_func()

TernaryPointDiagram::Private::Private()
    : AbstractTernaryDiagram::Private()
{
}

TernaryPointDiagram::TernaryPointDiagram(QWidget *parent,
                                         TernaryCoordinatePlane *plane)
    : AbstractTernaryDiagram(new Private(), parent, plane)
{
    init();
    setDatasetDimensionInternal(3); // the third column is implicit
}

TernaryPointDiagram::~TernaryPointDiagram()
{
}

void TernaryPointDiagram::init()
{
    d->reverseMapper.setDiagram(this);
}

void TernaryPointDiagram::resize(const QSizeF &area)
{
    Q_UNUSED(area);
}

void TernaryPointDiagram::paint(PaintContext *paintContext)
{
    d->reverseMapper.clear();

    d->paint(paintContext);

    // sanity checks:
    if (model() == nullptr)
        return;

    QPainter *p = paintContext->painter();
    PainterSaver s(p);

    auto *plane =
        static_cast<TernaryCoordinatePlane *>(paintContext->coordinatePlane());
    Q_ASSERT(plane);

    qreal x, y, z;

    // for some reason(?) TernaryPointDiagram is using per-diagram DVAs only:
    const DataValueAttributes attrs(dataValueAttributes());

    d->forgetAlreadyPaintedDataValues();

    int columnCount = model()->columnCount(rootIndex());
    for (int column = 0; column < columnCount; column += datasetDimension()) {
        int numrows = model()->rowCount(rootIndex());
        for (int row = 0; row < numrows; row++) {
            QModelIndex base = model()->index(row, column, rootIndex()); // checked
            // see if there is data otherwise skip
            if (!model()->data(base).isNull()) {
                p->setPen(PrintingParameters::scalePen(pen(base)));
                p->setBrush(brush(base));

                // retrieve data
                x = qMax(model()->data(model()->index(row, column + 0, rootIndex())).toReal(), // checked
                         ( qreal )0.0);
                y = qMax(model()->data(model()->index(row, column + 1, rootIndex())).toReal(), // checked
                         ( qreal )0.0);
                z = qMax(model()->data(model()->index(row, column + 2, rootIndex())).toReal(), // checked
                         ( qreal )0.0);

                // fix messed up data values (paint as much as possible)
                qreal total = x + y + z;
                if (fabs(total) > 3 * std::numeric_limits<qreal>::epsilon()) {
                    TernaryPoint tPunkt(x / total, y / total);
                    QPointF diagramLocation = translate(tPunkt);
                    QPointF widgetLocation = plane->translate(diagramLocation);

                    paintMarker(p, model()->index(row, column, rootIndex()), widgetLocation); // checked
                    QString text = tr("(%1, %2, %3)")
                                       .arg(x * 100, 0, 'f', 0)
                                       .arg(y * 100, 0, 'f', 0)
                                       .arg(z * 100, 0, 'f', 0);
                    d->paintDataValueText(p, attrs, widgetLocation, true, text, true);
                } else {
                    // ignore and do not paint this point, garbage data
                    qDebug() << "TernaryPointDiagram::paint: data point x/y/z:"
                             << x << "/" << y << "/" << z << "ignored, unusable.";
                }
            }
        }
    }
}

const QPair<QPointF, QPointF> TernaryPointDiagram::calculateDataBoundaries() const
{
    // this is a constant, because we defined it to be one:
    static QPair<QPointF, QPointF> Boundaries(
        TriangleBottomLeft,
        QPointF(TriangleBottomRight.x(), TriangleHeight));
    return Boundaries;
}
