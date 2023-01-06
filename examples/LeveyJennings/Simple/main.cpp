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

#include <KDChartChart>
#include <KDChartLeveyJenningsAxis>
#include <KDChartLeveyJenningsCoordinatePlane>
#include <KDChartLeveyJenningsDiagram>
#include <KDChartLeveyJenningsGridAttributes>

#include <QDateTime>
#include <QSplitter>
#include <QStandardItemModel>
#include <QTableView>
#include <QTimer>

class SelectionAnimator : public QObject
{
    Q_OBJECT
public:
    SelectionAnimator(QAbstractItemView *view)
        : QObject(view)
        , view(view)
    {
        auto *const t = new QTimer(this);
        connect(t, SIGNAL(timeout()), this, SLOT(animate()));
        t->start(1000);
    }

protected Q_SLOTS:
    void animate()
    {
        const int row = (view->selectionModel()->currentIndex().row() + 1) % view->model()->rowCount();
        view->selectionModel()->setCurrentIndex(view->model()->index(row, 0), QItemSelectionModel::ClearAndSelect);
    }

private:
    QAbstractItemView *const view;
};

#include "main.moc"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QStandardItemModel model(14, 6);

    model.setHeaderData(0, Qt::Horizontal, QObject::tr("Lot"));
    model.setHeaderData(1, Qt::Horizontal, QObject::tr("Value"));
    model.setHeaderData(2, Qt::Horizontal, QObject::tr("OK"));
    model.setHeaderData(3, Qt::Horizontal, QObject::tr("Date/Time"));
    model.setHeaderData(4, Qt::Horizontal, QObject::tr("Mean Value"));
    model.setHeaderData(5, Qt::Horizontal, QObject::tr("Standard Deviation"));

    // Lot 1
    model.setData(model.index(0, 0), 1); // lot number
    model.setData(model.index(0, 1), 210); // value
    model.setData(model.index(0, 2), true); // QC value ok/not ok
    model.setData(model.index(0, 3), QDateTime::fromString("2007-07-06T09:00:00", Qt::ISODate));

    model.setData(model.index(1, 0), 1);
    model.setData(model.index(1, 1), 9.5);
    model.setData(model.index(1, 2), true);
    model.setData(model.index(1, 3), QDateTime::fromString("2007-07-06T21:00:00", Qt::ISODate));
    model.setData(model.index(1, 4), 7.5);
    model.setData(model.index(1, 5), 1.0);

    model.setData(model.index(2, 0), 1);
    model.setData(model.index(2, 1), 200);
    model.setData(model.index(2, 2), true);
    model.setData(model.index(2, 3), QDateTime::fromString("2007-07-07T09:00:00", Qt::ISODate));

    // This values should be missing (lot is needed anyway)
    model.setData(model.index(3, 0), 1);
    model.setData(model.index(3, 3), QDateTime::fromString("2007-07-07T21:00:00", Qt::ISODate));

    model.setData(model.index(4, 0), 1);
    model.setData(model.index(4, 1), 180);
    model.setData(model.index(4, 2), true);
    model.setData(model.index(4, 3), QDateTime::fromString("2007-07-08T09:00:00", Qt::ISODate));

    // Lot 2
    model.setData(model.index(5, 0), 2);
    model.setData(model.index(5, 1), 210);
    model.setData(model.index(5, 2), true);
    model.setData(model.index(5, 3), QDateTime::fromString("2007-07-08T21:00:00", Qt::ISODate));

    model.setData(model.index(6, 0), 2);
    model.setData(model.index(6, 1), 195);
    model.setData(model.index(6, 2), true);
    model.setData(model.index(6, 3), QDateTime::fromString("2007-07-09T09:00:00", Qt::ISODate));

    // this value is not OK
    model.setData(model.index(7, 0), 2);
    model.setData(model.index(7, 1), 200);
    model.setData(model.index(7, 2), false);
    model.setData(model.index(7, 3), QDateTime::fromString("2007-07-09T21:00:00", Qt::ISODate));

    model.setData(model.index(8, 0), 2);
    model.setData(model.index(8, 1), 210);
    model.setData(model.index(8, 2), true);
    model.setData(model.index(8, 3), QDateTime::fromString("2007-07-10T09:00:00", Qt::ISODate));

    model.setData(model.index(9, 0), 2);
    model.setData(model.index(9, 1), 180);
    model.setData(model.index(9, 2), true);
    model.setData(model.index(9, 3), QDateTime::fromString("2007-07-10T21:00:00", Qt::ISODate));

    // this values is completely out of bounds and therefore cut/truncated
    model.setData(model.index(10, 0), 2);
    model.setData(model.index(10, 1), 290);
    model.setData(model.index(10, 2), true);
    model.setData(model.index(10, 3), QDateTime::fromString("2007-07-11T09:00:00", Qt::ISODate));

    // this value is ok again
    model.setData(model.index(11, 0), 2);
    model.setData(model.index(11, 1), 210);
    model.setData(model.index(11, 2), true);
    model.setData(model.index(11, 3), QDateTime::fromString("2007-07-11T21:00:00", Qt::ISODate));

    model.setData(model.index(12, 0), 2);
    model.setData(model.index(12, 1), 205);
    model.setData(model.index(12, 2), true);
    model.setData(model.index(12, 3), QDateTime::fromString("2007-07-12T09:00:00", Qt::ISODate));

    model.setData(model.index(13, 0), 2);
    model.setData(model.index(13, 1), 204);
    model.setData(model.index(13, 2), true);
    model.setData(model.index(13, 3), QDateTime::fromString("2007-07-12T21:00:00", Qt::ISODate));

    auto *chart = new KDChart::Chart();

    auto *diagram = new KDChart::LeveyJenningsDiagram;
    diagram->setModel(&model);
    diagram->setExpectedMeanValue(200);
    diagram->setExpectedStandardDeviation(20);
    auto *plane = new KDChart::LeveyJenningsCoordinatePlane;
    chart->replaceCoordinatePlane(plane);
    plane->replaceDiagram(diagram);

    /*diagram->setLotChangedSymbolPosition( Qt::AlignBottom );
    diagram->setSensorChangedSymbolPosition( Qt::AlignTop );
    diagram->setFluidicsPackChangedSymbolPosition( Qt::AlignTop );*/

    // diagram->setScanLinePen( QPen( Qt::green ) );
    // diagram->setSymbol( KDChart::LeveyJenningsDiagram::NotOkDataPoint,
    //                     diagram->symbol( KDChart::LeveyJenningsDiagram::OkDataPoint ) );

    diagram->setFluidicsPackChanges(QVector<QDateTime>() << QDateTime::fromString("2007-07-11T15:00:00", Qt::ISODate));
    diagram->setSensorChanges(QVector<QDateTime>() << QDateTime::fromString("2007-07-10T11:00:00", Qt::ISODate));

    auto *axis = new KDChart::LeveyJenningsAxis(diagram);
    axis->setPosition(KDChart::CartesianAxis::Left);
    diagram->addAxis(axis);

    auto *axis2 = new KDChart::LeveyJenningsAxis(diagram);
    axis2->setPosition(KDChart::CartesianAxis::Right);
    axis2->setType(KDChart::LeveyJenningsGridAttributes::Calculated);
    diagram->addAxis(axis2);

    KDChart::CartesianAxis *axis3 = new KDChart::LeveyJenningsAxis(diagram);
    axis3->setPosition(KDChart::CartesianAxis::Bottom);
    diagram->addAxis(axis3);

    auto *tv = new QTableView;
    tv->setModel(&model);
    tv->setSelectionModel(diagram->selectionModel());

    auto *splitter = new QSplitter;
    splitter->addWidget(tv);
    splitter->addWidget(chart);

    splitter->show();

    new SelectionAnimator(tv); // diagram );

    return app.exec();
}
