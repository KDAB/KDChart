/****************************************************************************
** Copyright (C) 2001-2021 Klaralvdalens Datakonsult AB.  All rights reserved.
**
** This file is part of the KD Chart library.
**
** Licensees holding valid commercial KD Chart licenses may use this file in
** accordance with the KD Chart Commercial License Agreement provided with
** the Software.
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 and version 3 as published by the
** Free Software Foundation and appearing in the file LICENSE.GPL.txt included.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** Contact info@kdab.com if any conditions of this licensing are not
** clear to you.
**
**********************************************************************/

#ifndef KDGANTTDATETIMEGRID_H
#define KDGANTTDATETIMEGRID_H

#include "kdganttabstractgrid.h"

#include <QDateTime>
#include <QSet>

namespace KDGantt
{
class DateTimeScaleFormatter;

class KDGANTT_EXPORT DateTimeGrid : public AbstractGrid
{
    Q_OBJECT
    KDGANTT_DECLARE_PRIVATE_DERIVED(DateTimeGrid)
public:
    enum Scale { ScaleAuto, ScaleHour, ScaleDay, ScaleWeek, ScaleMonth, ScaleUserDefined };

    DateTimeGrid();
    ~DateTimeGrid() override;

    [[nodiscard]] QDateTime startDateTime() const;
    void setStartDateTime(const QDateTime &dt);

    [[nodiscard]] qreal dayWidth() const;
    void setDayWidth(qreal);

    [[nodiscard]] qreal mapFromDateTime(const QDateTime &dt) const;
    [[nodiscard]] QDateTime mapToDateTime(qreal x) const;

    void setWeekStart(Qt::DayOfWeek);
    [[nodiscard]] Qt::DayOfWeek weekStart() const;

    void setFreeDays(const QSet<Qt::DayOfWeek> &fd);
    [[nodiscard]] QSet<Qt::DayOfWeek> freeDays() const;

    void setFreeDaysBrush(const QBrush brush);
    [[nodiscard]] QBrush freeDaysBrush() const;

    void setScale(Scale s);
    [[nodiscard]] Scale scale() const;

    void setUserDefinedLowerScale(DateTimeScaleFormatter *lower);
    void setUserDefinedUpperScale(DateTimeScaleFormatter *upper);
    [[nodiscard]] DateTimeScaleFormatter *userDefinedLowerScale() const;
    [[nodiscard]] DateTimeScaleFormatter *userDefinedUpperScale() const;

    [[nodiscard]] bool rowSeparators() const;
    void setRowSeparators(bool enable);

    void setNoInformationBrush(const QBrush &brush);
    [[nodiscard]] QBrush noInformationBrush() const;

    /*reimp*/ [[nodiscard]] Span mapToChart(const QModelIndex &idx) const override;
    /*reimp*/ bool mapFromChart(const Span &span,
                                const QModelIndex &idx,
                                const QList<Constraint> &constraints = QList<Constraint>()) const override; // not really const
    /*reimp*/ [[nodiscard]] qreal mapToChart(const QVariant &value) const override;
    /*reimp*/ [[nodiscard]] QVariant mapFromChart(qreal x) const override;
    /*reimp*/ void paintGrid(QPainter *painter,
                             const QRectF &sceneRect,
                             const QRectF &exposedRect,
                             AbstractRowController *rowController = nullptr,
                             QWidget *widget = nullptr) override;
    /*reimp*/ void paintHeader(QPainter *painter, const QRectF &headerRect, const QRectF &exposedRect, qreal offset, QWidget *widget = nullptr) override;

protected:
    virtual void paintHourScaleHeader(QPainter *painter, const QRectF &headerRect, const QRectF &exposedRect, qreal offset, QWidget *widget = nullptr);
    virtual void paintDayScaleHeader(QPainter *painter, const QRectF &headerRect, const QRectF &exposedRect, qreal offset, QWidget *widget = nullptr);
    virtual void paintWeekScaleHeader(QPainter *painter, const QRectF &headerRect, const QRectF &exposedRect, qreal offset, QWidget *widget = nullptr);
    virtual void paintMonthScaleHeader(QPainter *painter, const QRectF &headerRect, const QRectF &exposedRect, qreal offset, QWidget *widget = nullptr);

    virtual void paintUserDefinedHeader(QPainter *painter,
                                        const QRectF &headerRect,
                                        const QRectF &exposedRect,
                                        qreal offset,
                                        const DateTimeScaleFormatter *formatter,
                                        QWidget *widget = nullptr);

    virtual void drawDayBackground(QPainter *painter, const QRectF &rect, const QDate &date);
    virtual void drawDayForeground(QPainter *painter, const QRectF &rect, const QDate &date);

    [[nodiscard]] QRectF computeRect(const QDateTime &from, const QDateTime &to, const QRectF &rect) const;
    [[nodiscard]] QPair<QDateTime, QDateTime> dateTimeRange(const QRectF &rect) const;

    /* reimp */ void drawBackground(QPainter *paint, const QRectF &rect) override;
    /* reimp */ void drawForeground(QPainter *paint, const QRectF &rect) override;
};

class KDGANTT_EXPORT DateTimeScaleFormatter
{
    KDGANTT_DECLARE_PRIVATE_BASE_POLYMORPHIC(DateTimeScaleFormatter)
public:
    enum Range { Second, Minute, Hour, Day, Week, Month, Year };

    DateTimeScaleFormatter(Range range, const QString &formatString, Qt::Alignment alignment = Qt::AlignCenter);
    DateTimeScaleFormatter(Range range, const QString &formatString, const QString &templ, Qt::Alignment alignment = Qt::AlignCenter);
    DateTimeScaleFormatter(const DateTimeScaleFormatter &other);
    virtual ~DateTimeScaleFormatter();

    DateTimeScaleFormatter &operator=(const DateTimeScaleFormatter &other);

    [[nodiscard]] QString format() const;
    [[nodiscard]] Range range() const;
    [[nodiscard]] Qt::Alignment alignment() const;

    [[nodiscard]] virtual QDateTime nextRangeBegin(const QDateTime &datetime) const;
    [[nodiscard]] virtual QDateTime currentRangeBegin(const QDateTime &datetime) const;

    [[nodiscard]] QString format(const QDateTime &datetime) const;
    [[nodiscard]] virtual QString text(const QDateTime &datetime) const;
};
}

#ifndef QT_NO_DEBUG_STREAM
QDebug KDGANTT_EXPORT operator<<(QDebug dbg, KDGantt::DateTimeScaleFormatter::Range);
#endif

#endif /* KDGANTTDATETIMEGRID_H */
