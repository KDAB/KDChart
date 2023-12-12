/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef NULL_PAINT_DEVICE_H
#define NULL_PAINT_DEVICE_H

#include <QPaintDevice>
#include <QPaintEngine>

namespace KDChart {
class NullPaintEngine : public QPaintEngine
{
public:
    bool begin(QPaintDevice * /*pdev*/) override
    {
        return true;
    }
    void drawEllipse(const QRectF & /*rect*/) override
    {
    }
    void drawEllipse(const QRect & /*rect*/) override
    {
    }
    void drawImage(const QRectF & /*rectangle*/, const QImage & /*image*/, const QRectF & /*sr*/, Qt::ImageConversionFlags /*flags*/) override
    {
    }
    void drawLines(const QLineF * /*lines*/, int /*lineCount*/) override
    {
    }
    void drawLines(const QLine * /*lines*/, int /*lineCount*/) override
    {
    }
    void drawPath(const QPainterPath & /*path*/) override
    {
    }
    void drawPixmap(const QRectF & /*r*/, const QPixmap & /*pm*/, const QRectF & /*sr*/) override
    {
    }
    void drawPoints(const QPointF * /*points*/, int /*pointCount*/) override
    {
    }
    void drawPoints(const QPoint * /*points*/, int /*pointCount*/) override
    {
    }
    void drawPolygon(const QPointF * /*points*/, int /*pointCount*/, PolygonDrawMode /*mode*/) override
    {
    }
    void drawPolygon(const QPoint * /*points*/, int /*pointCount*/, PolygonDrawMode /*mode*/) override
    {
    }
    void drawRects(const QRectF * /*rects*/, int /*rectCount*/) override
    {
    }
    void drawRects(const QRect * /*rects*/, int /*rectCount*/) override
    {
    }
    void drawTextItem(const QPointF & /*p*/, const QTextItem & /*textItem*/) override
    {
    }
    void drawTiledPixmap(const QRectF & /*rect*/, const QPixmap & /*pixmap*/, const QPointF & /*p*/) override
    {
    }
    bool end() override
    {
        return true;
    }

    Type type() const override
    {
        return QPaintEngine::User;
    }
    void updateState(const QPaintEngineState & /*state*/) override
    {
    }
};

class NullPaintDevice : public QPaintDevice
{
public:
    NullPaintDevice(const QSize &size)
        : m_size(size)
    {
    }
    ~NullPaintDevice() override
    {
    }

    int metric(PaintDeviceMetric metric) const override
    {
        switch (metric) {
        case QPaintDevice::PdmWidth:
            return m_size.width();
        case QPaintDevice::PdmHeight:
            return m_size.height();
        case QPaintDevice::PdmWidthMM:
            return 1;
        case QPaintDevice::PdmHeightMM:
            return 1;
        case QPaintDevice::PdmNumColors:
            return int(( uint )(-1));
        case QPaintDevice::PdmDepth:
            return 1;
        case QPaintDevice::PdmDpiX:
            return 1;
        case QPaintDevice::PdmDpiY:
            return 1;
        case QPaintDevice::PdmPhysicalDpiX:
            return 1;
        case QPaintDevice::PdmPhysicalDpiY:
            return 1;
        case QPaintDevice::PdmDevicePixelRatio:
            return 1;
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
        case QPaintDevice::PdmDevicePixelRatioScaled:
            return 1;
#endif
        }
        return 1;
    }

    QPaintEngine *paintEngine() const override
    {
        static NullPaintEngine nullPaintEngine;
        return &nullPaintEngine;
    }

private:
    QSize m_size;
};
}

#endif
