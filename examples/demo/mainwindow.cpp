/****************************************************************************
** Copyright (C) 2001-2013 Klaralvdalens Datakonsult AB.  All rights reserved.
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

#include "mainwindow.h"

#include <KDChart/KDChartChart>
#include <KDChart/KDChartCartesianCoordinatePlane>
#include <KDChart/KDChartAbstractCoordinatePlane>
#include <KDChart/KDChartCartesianAxis>
#include <KDChart/KDChartCartesianGrid>
#include <KDChart/KDChartAbstractDiagram>
#include <KDChart/KDChartBarDiagram>
#include <KDChart/KDChartLineDiagram>
#include <KDChart/KDChartPlotter>
#include <KDChart/KDChartPieDiagram>
#include <KDChart/KDChartPolarCoordinatePlane>
#include <KDChart/KDChartLegend>
#include <KDChart/KDChartDataValueAttributes>
#include <KDChart/KDChartMarkerAttributes>
#include <KDChart/KDChartTextAttributes>
#include <KDChart/KDChartGridAttributes>
#include <KDChart/KDChartRulerAttributes>
#include <KDChart/KDChartPieAttributes>

#include <TableModel.h>

#include <QDockWidget>

#include "datasetsettings.h"
#include "diagramsettings.h"
#include "datavaluesettings.h"
#include "diagramtypedialog.h"
#include "axissettings.h"

using namespace KDChart;

class MainWindow::Private : public QObject
{
    Q_OBJECT
    Q_PROPERTY( int datasetCount READ datasetCount NOTIFY datasetCountChanged )
public:
    int datasetCount() const;
    enum ChartType{ Bar, Line, Plotter, Polar, Pie };
    Private( MainWindow *q = 0 );
    Chart *m_chartWin;
    CartesianCoordinatePlane *m_cartPlane;
    TableModel *m_model;
    AbstractDiagram *m_curDiagram;
    ChartType m_type;
    QHash< DiagramTypeDialog::DiagramType, QAbstractItemModel* > m_modelTable;
    MainWindow *qq;
Q_SIGNALS:
    void datasetCountChanged( int );
public Q_SLOTS:
    void changedChartType();
};

MainWindow::Private::Private( MainWindow *q )
    : m_chartWin( new Chart( q ) )
    , m_cartPlane( new CartesianCoordinatePlane )
    , m_model( new TableModel( q ) )
    , m_curDiagram( new BarDiagram( q, m_cartPlane ) )
    , m_type( Bar )
    , qq( q )
{
    m_chartWin->replaceCoordinatePlane( m_cartPlane );
    m_model->loadFromCSV( ":/data/barSimple.csv" );
    m_cartPlane->replaceDiagram( m_curDiagram );
    m_curDiagram->setModel( m_model );
    m_modelTable[ DiagramTypeDialog::Bar ] = m_model;
    m_modelTable[ DiagramTypeDialog::LyingBar ] = m_model;
    TableModel *lineModel = new TableModel( q );
    lineModel->loadFromCSV( ":/data/lineSimple.csv" );
    m_modelTable[ DiagramTypeDialog::Line ] = lineModel;
    m_modelTable[ DiagramTypeDialog::Plotter ] = lineModel;
    TableModel *pieModel = new TableModel( q );
    pieModel->loadFromCSV( ":/data/pieSimple.csv" );
    m_modelTable[ DiagramTypeDialog::Pie ] = pieModel;

    DiagramTypeDialog *diagramTypeSettings = new DiagramTypeDialog( m_chartWin, qq );
    diagramTypeSettings->setDefaultModels( m_modelTable );
    QDockWidget *diagramTypeSettingsDock = new QDockWidget( tr( "DiagramType" ), qq );
    diagramTypeSettingsDock->setWidget( diagramTypeSettings );
    diagramTypeSettingsDock->setFloating( false );
    connect( diagramTypeSettings, SIGNAL( diagramTypeChanged( DiagramType, Subtype ) ), this, SLOT( changedChartType() ) );
    q->addDockWidget( Qt::LeftDockWidgetArea, diagramTypeSettingsDock );

    AxisSettings *axisSettings = new AxisSettings( m_chartWin, qq );
    QDockWidget *axisSettingsDock = new QDockWidget( tr( "AxisSettings" ), qq );
    axisSettingsDock->setWidget( axisSettings );
    axisSettingsDock->setFloating( false );
    q->addDockWidget( Qt::LeftDockWidgetArea, axisSettingsDock );
    connect( diagramTypeSettings, SIGNAL( diagramTypeChanged( DiagramType, Subtype ) ), axisSettings, SLOT( diagramTypeChanged() ) );

    DatasetSettings *setSettings = new DatasetSettings( m_chartWin, qq );
    QDockWidget *setSettingsDock = new QDockWidget( tr( "DatasetSettings" ), qq );
    setSettingsDock->setWidget( setSettings );
    setSettingsDock->setFloating( false );
    q->addDockWidget( Qt::LeftDockWidgetArea, setSettingsDock );
    connect( this, SIGNAL( datasetCountChanged( int ) ), setSettings, SLOT( setDatasetCount( int ) ) );
    connect( diagramTypeSettings, SIGNAL( diagramTypeChanged( DiagramType, Subtype ) ), setSettings, SLOT( diagramTypeChanged() ) );
    setSettings->setDatasetCount( m_model->columnCount() );

    DiagramSettings *diagSettings = new DiagramSettings( m_chartWin, qq );
    QDockWidget *diagSettingsDock = new QDockWidget( tr( "DiagramSettings" ), qq );
    diagSettingsDock->setWidget( diagSettings );
    diagSettingsDock->setFloating( false );
    q->addDockWidget( Qt::LeftDockWidgetArea, diagSettingsDock );
    connect( diagramTypeSettings, SIGNAL( diagramTypeChanged( DiagramType, Subtype ) ), diagSettings, SLOT( refreshSettings() ) );

    DataValueSettings *dataValueSettings = new DataValueSettings( m_chartWin, qq );
    QDockWidget *dataValueSettingsDock = new QDockWidget( tr( "DataValueSettings" ), qq );
    dataValueSettingsDock->setWidget( dataValueSettings );
    dataValueSettingsDock->setFloating( false );
    q->addDockWidget( Qt::RightDockWidgetArea, dataValueSettingsDock );
    connect( diagramTypeSettings, SIGNAL( diagramTypeChanged( DiagramType, Subtype ) ), dataValueSettings, SLOT( refresh() ) );

}

int MainWindow::Private::datasetCount() const
{
    if ( m_chartWin->coordinatePlane() && m_chartWin->coordinatePlane()->diagram() )
    {
        const AbstractDiagram *diag = m_chartWin->coordinatePlane()->diagram();
        if ( !diag->model() )
            return 0;
        return diag->model()->columnCount() / diag->datasetDimension();
    }
    return 0;
}

void MainWindow::Private::changedChartType()
{
    Q_EMIT datasetCountChanged( datasetCount() );
}

MainWindow::MainWindow( QWidget *parent )
    : QMainWindow( parent )
    , d( new Private( this ) )
{
    setCentralWidget( d->m_chartWin );
}

MainWindow::~MainWindow()
{
    delete d;
}

#include "mainwindow.moc"
