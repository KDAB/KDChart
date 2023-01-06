/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "mainwindow.h"

#include <KDChart/KDChartAbstractCoordinatePlane>
#include <KDChart/KDChartAbstractDiagram>
#include <KDChart/KDChartBarDiagram>
#include <KDChart/KDChartCartesianAxis>
#include <KDChart/KDChartCartesianCoordinatePlane>
#include <KDChart/KDChartCartesianGrid>
#include <KDChart/KDChartChart>
#include <KDChart/KDChartDataValueAttributes>
#include <KDChart/KDChartGridAttributes>
#include <KDChart/KDChartLegend>
#include <KDChart/KDChartLineDiagram>
#include <KDChart/KDChartMarkerAttributes>
#include <KDChart/KDChartPieAttributes>
#include <KDChart/KDChartPieDiagram>
#include <KDChart/KDChartPlotter>
#include <KDChart/KDChartPolarCoordinatePlane>
#include <KDChart/KDChartRulerAttributes>
#include <KDChart/KDChartTextAttributes>

#include <TableModel.h>

#include <QDockWidget>

#include "axissettings.h"
#include "datasetsettings.h"
#include "datavaluesettings.h"
#include "diagramsettings.h"
#include "diagramtypedialog.h"

using namespace KDChart;

class MainWindow::Private : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int datasetCount READ datasetCount NOTIFY datasetCountChanged)
public:
    int datasetCount() const;
    enum ChartType
    {
        Bar,
        Line,
        Plotter,
        Polar,
        Pie
    };
    Private(MainWindow *q = nullptr);
    Chart *m_chartWin;
    CartesianCoordinatePlane *m_cartPlane;
    TableModel *m_model;
    AbstractDiagram *m_curDiagram;
    ChartType m_type = Bar;
    QHash<DiagramTypeDialog::DiagramType, QAbstractItemModel *> m_modelTable;
    MainWindow *qq;
Q_SIGNALS:
    void datasetCountChanged(int);
public Q_SLOTS:
    void changedChartType();
};

MainWindow::Private::Private(MainWindow *q)
    : m_chartWin(new Chart(q))
    , m_cartPlane(new CartesianCoordinatePlane)
    , m_model(new TableModel(q))
    , m_curDiagram(new BarDiagram(q, m_cartPlane))
    , qq(q)
{
    m_chartWin->replaceCoordinatePlane(m_cartPlane);
    m_model->loadFromCSV(":/data/barSimple.csv");
    m_cartPlane->replaceDiagram(m_curDiagram);
    m_curDiagram->setModel(m_model);
    m_modelTable[DiagramTypeDialog::Bar] = m_model;
    m_modelTable[DiagramTypeDialog::LyingBar] = m_model;
    auto *lineModel = new TableModel(q);
    lineModel->loadFromCSV(":/data/lineSimple.csv");
    m_modelTable[DiagramTypeDialog::Line] = lineModel;
    m_modelTable[DiagramTypeDialog::Plotter] = lineModel;
    auto *pieModel = new TableModel(q);
    pieModel->loadFromCSV(":/data/pieSimple.csv");
    m_modelTable[DiagramTypeDialog::Pie] = pieModel;

    auto *diagramTypeSettings = new DiagramTypeDialog(m_chartWin, qq);
    diagramTypeSettings->setDefaultModels(m_modelTable);
    auto *diagramTypeSettingsDock = new QDockWidget(tr("DiagramType"), qq);
    diagramTypeSettingsDock->setWidget(diagramTypeSettings);
    diagramTypeSettingsDock->setFloating(false);
    connect(diagramTypeSettings, SIGNAL(diagramTypeChanged(DiagramType, Subtype)), this, SLOT(changedChartType()));
    q->addDockWidget(Qt::LeftDockWidgetArea, diagramTypeSettingsDock);

    auto *axisSettings = new AxisSettings(m_chartWin, qq);
    auto *axisSettingsDock = new QDockWidget(tr("AxisSettings"), qq);
    axisSettingsDock->setWidget(axisSettings);
    axisSettingsDock->setFloating(false);
    q->addDockWidget(Qt::LeftDockWidgetArea, axisSettingsDock);
    connect(diagramTypeSettings, SIGNAL(diagramTypeChanged(DiagramType, Subtype)), axisSettings, SLOT(diagramTypeChanged()));

    auto *setSettings = new DatasetSettings(m_chartWin, qq);
    auto *setSettingsDock = new QDockWidget(tr("DatasetSettings"), qq);
    setSettingsDock->setWidget(setSettings);
    setSettingsDock->setFloating(false);
    q->addDockWidget(Qt::LeftDockWidgetArea, setSettingsDock);
    connect(this, SIGNAL(datasetCountChanged(int)), setSettings, SLOT(setDatasetCount(int)));
    connect(diagramTypeSettings, SIGNAL(diagramTypeChanged(DiagramType, Subtype)), setSettings, SLOT(diagramTypeChanged()));
    setSettings->setDatasetCount(m_model->columnCount());

    auto *diagSettings = new DiagramSettings(m_chartWin, qq);
    auto *diagSettingsDock = new QDockWidget(tr("DiagramSettings"), qq);
    diagSettingsDock->setWidget(diagSettings);
    diagSettingsDock->setFloating(false);
    q->addDockWidget(Qt::LeftDockWidgetArea, diagSettingsDock);
    connect(diagramTypeSettings, SIGNAL(diagramTypeChanged(DiagramType, Subtype)), diagSettings, SLOT(refreshSettings()));

    auto *dataValueSettings = new DataValueSettings(m_chartWin, qq);
    auto *dataValueSettingsDock = new QDockWidget(tr("DataValueSettings"), qq);
    dataValueSettingsDock->setWidget(dataValueSettings);
    dataValueSettingsDock->setFloating(false);
    q->addDockWidget(Qt::RightDockWidgetArea, dataValueSettingsDock);
    connect(diagramTypeSettings, SIGNAL(diagramTypeChanged(DiagramType, Subtype)), dataValueSettings, SLOT(refresh()));
}

int MainWindow::Private::datasetCount() const
{
    if (m_chartWin->coordinatePlane() && m_chartWin->coordinatePlane()->diagram()) {
        const AbstractDiagram *diag = m_chartWin->coordinatePlane()->diagram();
        if (!diag->model())
            return 0;
        return diag->model()->columnCount() / diag->datasetDimension();
    }
    return 0;
}

void MainWindow::Private::changedChartType()
{
    Q_EMIT datasetCountChanged(datasetCount());
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , d(new Private(this))
{
    setCentralWidget(d->m_chartWin);
}

MainWindow::~MainWindow()
{
    delete d;
}

#include "mainwindow.moc"
