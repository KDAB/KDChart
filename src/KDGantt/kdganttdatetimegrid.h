/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDGANTTDATETIMEGRID_H
#define KDGANTTDATETIMEGRID_H

#include "kdganttabstractgrid.h"

#include <QDateTime>
#include <QSet>

namespace KDGantt {

class DateTimeScaleFormatter;

class KDGANTT_EXPORT DateTimeGrid : public AbstractGrid
{
    Q_OBJECT
    KDGANTT_DECLARE_PRIVATE_DERIVED(DateTimeGrid)
public:
    enum Scale
    {
        ScaleAuto,
        ScaleHour,
        ScaleDay,
        ScaleWeek,
        ScaleMonth,
        ScaleUserDefined
    };

    DateTimeGrid();
    ~DateTimeGrid() override;

    QDateTime startDateTime() const;
    void setStartDateTime(const QDateTime &dt);

    qreal dayWidth() const;
    void setDayWidth(qreal);

    qreal mapFromDateTime(const QDateTime &dt) const;
    QDateTime mapToDateTime(qreal x) const;

    void setWeekStart(Qt::DayOfWeek);
    Qt::DayOfWeek weekStart() const;

    void setFreeDays(const QSet<Qt::DayOfWeek> &fd);
    QSet<Qt::DayOfWeek> freeDays() const;

    void setFreeDaysBrush(const QBrush brush);
    QBrush freeDaysBrush() const;

    void setScale(Scale s);
    Scale scale() const;

    void setUserDefinedLowerScale(DateTimeScaleFormatter *lower);
    void setUserDefinedUpperScale(DateTimeScaleFormatter *upper);
    DateTimeScaleFormatter *userDefinedLowerScale() const;
    DateTimeScaleFormatter *userDefinedUpperScale() const;

    bool rowSeparators() const;
    void setRowSeparators(bool enable);

    void setNoInformationBrush(const QBrush &brush);
    QBrush noInformationBrush() const;

    /*reimp*/ Span mapToChart(const QModelIndex &idx) const override;
    /*reimp*/ bool mapFromChart(const Span &span, const QModelIndex &idx,
                                const QList<Constraint> &constraints = QList<Constraint>()) const override;
    /*reimp*/ qreal mapToChart(const QVariant &value) const override;
    /*reimp*/ QVariant mapFromChart(qreal x) const override;
    /*reimp*/ void paintGrid(QPainter *painter,
                             const QRectF &sceneRect, const QRectF &exposedRect,
                             AbstractRowController *rowController = nullptr,
                             QWidget *widget = nullptr) override;
    /*reimp*/ void paintHeader(QPainter *painter,
                               const QRectF &headerRect, const QRectF &exposedRect,
                               qreal offset, QWidget *widget = nullptr) override;

protected:
    virtual void paintHourScaleHeader(QPainter *painter,
                                      const QRectF &headerRect, const QRectF &exposedRect,
                                      qreal offset, QWidget *widget = nullptr);
    virtual void paintDayScaleHeader(QPainter *painter,
                                     const QRectF &headerRect, const QRectF &exposedRect,
                                     qreal offset, QWidget *widget = nullptr);
    virtual void paintWeekScaleHeader(QPainter *painter,
                                      const QRectF &headerRect, const QRectF &exposedRect,
                                      qreal offset, QWidget *widget = nullptr);
    virtual void paintMonthScaleHeader(QPainter *painter,
                                       const QRectF &headerRect, const QRectF &exposedRect,
                                       qreal offset, QWidget *widget = nullptr);

    virtual void paintUserDefinedHeader(QPainter *painter,
                                        const QRectF &headerRect, const QRectF &exposedRect,
                                        qreal offset, const DateTimeScaleFormatter *formatter,
                                        QWidget *widget = nullptr);

    virtual void drawDayBackground(QPainter *painter, const QRectF &rect, const QDate &date);
    virtual void drawDayForeground(QPainter *painter, const QRectF &rect, const QDate &date);

    QRectF computeRect(const QDateTime &from, const QDateTime &to, const QRectF &rect) const;
    QPair<QDateTime, QDateTime> dateTimeRange(const QRectF &rect) const;

    /* reimp */ void drawBackground(QPainter *paint, const QRectF &rect) override;
    /* reimp */ void drawForeground(QPainter *paint, const QRectF &rect) override;
};

class KDGANTT_EXPORT DateTimeScaleFormatter
{
    KDGANTT_DECLARE_PRIVATE_BASE_POLYMORPHIC(DateTimeScaleFormatter)
public:
    enum Range
    {
        Second,
        Minute,
        Hour,
        Day,
        Week,
        Month,
        Year
    };

    DateTimeScaleFormatter(Range range, const QString &formatString,
                           Qt::Alignment alignment = Qt::AlignCenter);
    DateTimeScaleFormatter(Range range, const QString &formatString,
                           const QString &templ, Qt::Alignment alignment = Qt::AlignCenter);
    DateTimeScaleFormatter(const DateTimeScaleFormatter &other);
    virtual ~DateTimeScaleFormatter();

    DateTimeScaleFormatter &operator=(const DateTimeScaleFormatter &other);

    QString format() const;
    Range range() const;
    Qt::Alignment alignment() const;

    virtual QDateTime nextRangeBegin(const QDateTime &datetime) const;
    virtual QDateTime currentRangeBegin(const QDateTime &datetime) const;

    QString format(const QDateTime &datetime) const;
    virtual QString text(const QDateTime &datetime) const;
};
}

#ifndef QT_NO_DEBUG_STREAM
QDebug KDGANTT_EXPORT operator<<(QDebug dbg, KDGantt::DateTimeScaleFormatter::Range);
#endif

#endif /* KDGANTTDATETIMEGRID_H */
