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
#include <QPushButton>
#include <QSplitter>
#include <QStandardItemModel>
#include <QTimer>
#include <QTreeView>
#include <QWidget>

#include <math.h>

#include <KDChartChart>
#include <KDChartPlotter>

#include "timeaxis.h"
#include "timechartmodel.h"

class ChartWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChartWidget(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        auto *splitter = new QSplitter(this);
        auto *l = new QHBoxLayout(this);
        setLayout(l);
        l->addWidget(splitter);

        auto *leftWidget = new QWidget(splitter);
        auto *leftLayout = new QVBoxLayout(leftWidget);
        leftWidget->setLayout(leftLayout);

        auto *button = new QPushButton("Animate", leftWidget);
        leftLayout->addWidget(button);
        button->setCheckable(true);
        connect(button, SIGNAL(toggled(bool)), this, SLOT(buttonToggled(bool)));

        auto *tv = new QTreeView(leftWidget);
        leftLayout->addWidget(tv);

        m_chart = new KDChart::Chart(splitter);

        m_model = new QStandardItemModel(365, 2, this);
        for (int i = 0; i < 365; ++i) {
            const QDateTime dt = QDateTime(QDate(2010, 1, 1), QTime());
            m_model->setData(m_model->index(i, 0), dt.addDays(i));
            m_model->setData(m_model->index(i, 1), sin(i / 10.0) * 10);
        }

        auto *proxy = new TimeChartModel(this);
        proxy->setSourceModel(m_model);
        proxy->setVisibleRange(QDateTime(QDate(2010, 2, 1), QTime()),
                               QDateTime(QDate(2010, 3, 31), QTime()));

        auto *plotter = new KDChart::Plotter;
        m_chart->coordinatePlane()->replaceDiagram(plotter);

        tv->setModel(proxy);
        tv->show();

        auto *axis = new TimeAxis(plotter);
        axis->setPosition(TimeAxis::Bottom);
        plotter->addAxis(axis);

        plotter->setModel(proxy);

        connect(proxy, SIGNAL(rowsInserted(QModelIndex, int, int)),
                m_chart->coordinatePlane(), SLOT(adjustRangesToData()));
        connect(proxy, SIGNAL(rowsRemoved(QModelIndex, int, int)),
                m_chart->coordinatePlane(), SLOT(adjustRangesToData()));

        proxy->setVisibleRange(QDateTime(QDate(2010, 3, 15), QTime()),
                               QDateTime(QDate(2010, 5, 18), QTime()));
        qobject_cast<KDChart::CartesianCoordinatePlane *>(m_chart->coordinatePlane())->adjustRangesToData();

        m_timer = new QTimer(this);
        connect(m_timer, SIGNAL(timeout()), this, SLOT(slotTimeout()));
    }
private slots:
    void slotTimeout()
    {
        // An ugly hack to prevent the QAbstractItemModel from emitting dataChanged
        // for every single call to setData which would result in a full relayout
        // every time. That's horrible for performance and what we do is to prevent
        // QAbstractItemModel to emit dataChanged, collect what data changed and
        // emit the signal at the end ourself.
        m_model->blockSignals(true);
        QModelIndexList indexes;

        QVariant v1, v2;
        for (int i = 0; i < 365; ++i) {
            QModelIndex idx = m_model->index(i, 1);
            indexes.append(idx);
            m_model->setData(idx, sin(i / 10.0 + m_counter) * 10);
        }

        m_model->blockSignals(false);
        if (!indexes.isEmpty()) {
            m_model->metaObject()->invokeMethod(m_model, "dataChanged", Qt::DirectConnection, Q_ARG(QModelIndex, indexes.first()), Q_ARG(QModelIndex, indexes.last()));
        }

        m_counter += 0.02;
    }
    void buttonToggled(bool checked)
    {
        if (checked)
            m_timer->start(200);
        else
            m_timer->stop();
    }

private:
    KDChart::Chart *m_chart;
    QStandardItemModel *m_model;
    QTimer *m_timer;
    qreal m_counter = 0;
};

/**
 * This example demonstrates how to use time-based plots with timestamp-value data points
 * based on seconds and how to use a proxy model for defining the plotted "window" of the
 * measurement data.
 */
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ChartWidget w;
    w.show();

    return app.exec();
}

#include "main.moc"
