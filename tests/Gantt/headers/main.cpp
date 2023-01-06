/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include <QApplication>
#include <QHeaderView>
#include <QPainter>
#include <QStandardItemModel>
#include <QTreeView>

#include <KDGanttDateTimeGrid>
#include <KDGanttView>

using namespace KDGantt;

/* A custom formatter that displays tabs for every 10 minutes.
 */
class MyDateTimeScaleFormatter : public DateTimeScaleFormatter
{
public:
    MyDateTimeScaleFormatter();

    /*reimp*/ QDateTime nextRangeBegin(const QDateTime &datetime) const override;
    /*reimp*/ QDateTime currentRangeBegin(const QDateTime &datetime) const override;

    /*reimp*/ QString text(const QDateTime &dt) const override;
};

MyDateTimeScaleFormatter::MyDateTimeScaleFormatter()
    : DateTimeScaleFormatter(Hour, "hh")
{
}

QDateTime MyDateTimeScaleFormatter::nextRangeBegin(const QDateTime &datetime) const
{
    return currentRangeBegin(datetime).addSecs(60 * 10);
}

QDateTime MyDateTimeScaleFormatter::currentRangeBegin(const QDateTime &datetime) const
{
    QDateTime dt(datetime);
    dt.setTime(QTime(dt.time().hour(), (dt.time().minute() / 10) * 10, 0, 0));
    return dt;
}

QString MyDateTimeScaleFormatter::text(const QDateTime &dt) const
{
    return QObject::tr(":%1\nXX").arg(dt.time().toString("mm"));
}

/* A custom headerview that is taller than standard
 * so we can fit more lines into it on the graphicsview
 * side.
 */
class MyHeaderView : public QHeaderView
{
public:
    explicit MyHeaderView(QWidget *parent = 0)
        : QHeaderView(Qt::Horizontal, parent)
    {
    }

    /*reimp*/ QSize sizeHint() const override
    {
        QSize s = QHeaderView::sizeHint();
        s.rheight() *= 3;
        return s;
    }
};

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QStandardItemModel model(1, 1);
    model.setHeaderData(0, Qt::Horizontal, QObject::tr("Task"));

    /*** A view with some alternative header labels ***/
    View view1;
    DateTimeGrid grid1;
    grid1.setUserDefinedUpperScale(new DateTimeScaleFormatter(DateTimeScaleFormatter::Year,
                                                              QString::fromLatin1("yyyy"),
                                                              QString::fromLatin1("In the year %1."),
                                                              Qt::AlignLeft));
    grid1.setUserDefinedLowerScale(new DateTimeScaleFormatter(DateTimeScaleFormatter::Month,
                                                              QString::fromLatin1("MMMM"),
                                                              QString::fromLatin1("In the month %1."),
                                                              Qt::AlignRight));
    grid1.setScale(DateTimeGrid::ScaleUserDefined);
    grid1.setDayWidth(6.);
    view1.setGrid(&grid1);
    view1.setModel(&model);
    view1.show();

    /*** A view with header and vertical grid lines for every 10 minutes */
    View view2;
    auto *tw = qobject_cast<QTreeView *>(view2.leftView());
    if (tw)
        tw->setHeader(new MyHeaderView);
    DateTimeGrid grid2;
    grid2.setDayWidth(5000);
    grid2.setUserDefinedUpperScale(new DateTimeScaleFormatter(DateTimeScaleFormatter::Hour, QString::fromLatin1("hh")));
    grid2.setUserDefinedLowerScale(new MyDateTimeScaleFormatter);
    grid2.setScale(DateTimeGrid::ScaleUserDefined);
    view2.setGrid(&grid2);
    view2.setModel(&model);
    view2.show();

    return app.exec();
}
