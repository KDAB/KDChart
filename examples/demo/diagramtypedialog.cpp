#include "diagramtypedialog.h"
#include "ui_diagramtypedialog.h"

#include <KDChart/KDChartChart>
#include <KDChart/KDChartAbstractCoordinatePlane>
#include <KDChart/KDChartCartesianCoordinatePlane>
#include <KDChart/KDChartPolarCoordinatePlane>

#include <KDChart/KDChartBarDiagram>
#include <KDChart/KDChartLineDiagram>
#include <KDChart/KDChartPieDiagram>

#include <QDebug>

using namespace KDChart;

class DiagramTypeDialog::Private : public QObject
{
    Q_OBJECT
public:
    Private( KDChart::Chart *chart, DiagramTypeDialog * q );
    void init();
    void createPlanes();

    int lastIndex;
    Chart *m_chart;
    DiagramTypeDialog::DiagramType type;
    DiagramTypeDialog::Subtype subType;
    QHash< DiagramTypeDialog::DiagramType, QAbstractItemModel* > m_defaultModels;
    QHash< DiagramTypeDialog::DiagramType, AbstractCoordinatePlane* > m_planes;
    DiagramTypeDialog *qq;
    Ui::DiagramTypeDialog ui;
public Q_SLOTS:
    void typeChanged( int index );
};

DiagramTypeDialog::Private::Private( KDChart::Chart *chart, DiagramTypeDialog * q )
    : lastIndex( 0 )
    , m_chart( chart )
    , type( DiagramTypeDialog::Bar )
    , subType( DiagramTypeDialog::Normal )
    , qq( q )
{

}

void DiagramTypeDialog::Private::init()
{
    ui.setupUi( qq );
    ui.typeSelector->addItem( QIcon(), tr( "BarDiagram" ), DiagramTypeDialog::Bar );
    ui.typeSelector->addItem( QIcon(), tr( "Lying BarDiagram" ), DiagramTypeDialog::LyingBar );
    ui.typeSelector->addItem( QIcon(), tr( "LineDiagram" ), DiagramTypeDialog::Line );
    ui.typeSelector->addItem( QIcon(), tr( "Plotter" ), DiagramTypeDialog::Plotter );
    ui.typeSelector->addItem( QIcon(), tr( "PieDiagram" ), DiagramTypeDialog::Pie );

    ui.subtypeSelector->addItem( QIcon(), tr( "Normal" ), DiagramTypeDialog::Normal );
    ui.subtypeSelector->addItem( QIcon(), tr( "Stacked" ), DiagramTypeDialog::Stacked );
    ui.subtypeSelector->addItem( QIcon(), tr( "Percent" ), DiagramTypeDialog::Percent );

    createPlanes();

    connect( ui.typeSelector, SIGNAL( currentIndexChanged( int ) ), this, SLOT( typeChanged( int ) ) );
}

void DiagramTypeDialog::Private::createPlanes()
{
    m_planes[ DiagramTypeDialog::Bar ] = m_chart->coordinatePlane();
    m_planes[ DiagramTypeDialog::LyingBar ] = m_chart->coordinatePlane();
    CartesianCoordinatePlane *linePlane = new CartesianCoordinatePlane;
    LineDiagram *lineDiagram = new LineDiagram;
    linePlane->addDiagram( lineDiagram );
    m_planes[ DiagramTypeDialog::Line ] = linePlane;
    PolarCoordinatePlane *piePlane = new PolarCoordinatePlane;
    PieDiagram *pieDiagram = new PieDiagram;
    piePlane->addDiagram( pieDiagram );
    m_planes[ DiagramTypeDialog::Pie ] = piePlane;
}

void DiagramTypeDialog::Private::typeChanged( int index )
{
    if ( index == lastIndex )
        return;
    const DiagramTypeDialog::DiagramType type = static_cast< DiagramTypeDialog::DiagramType >( ui.typeSelector->itemData( index ).toInt() );
    const DiagramTypeDialog::DiagramType lastType = static_cast< DiagramTypeDialog::DiagramType >( ui.typeSelector->itemData( lastIndex ).toInt() );
    if ( m_planes.contains( type ) )
    {
        if ( type == DiagramTypeDialog::LyingBar )
        {
            BarDiagram * diag = qobject_cast< BarDiagram* >( m_planes[ type ]->diagram() );
            diag->setOrientation( Qt::Horizontal );
        }
        else if ( type == DiagramTypeDialog::Bar )
        {
            BarDiagram * diag = qobject_cast< BarDiagram* >( m_planes[ type ]->diagram() );
            diag->setOrientation( Qt::Vertical );
        }
        this->type = type;
        m_chart->addCoordinatePlane( m_planes[ type ] );
        m_chart->takeCoordinatePlane( m_planes[ lastType ] );

        lastIndex = index;
        Q_EMIT qq->diagramTypeChanged( type, subType );
    }
    else
    {
        ui.typeSelector->setCurrentIndex( lastIndex );
    }
}

DiagramTypeDialog::DiagramTypeDialog( KDChart::Chart *chart, QWidget *parent )
    : QDialog( parent )
    , d( new Private( chart, this ) )
{
    d->init();
}

DiagramTypeDialog::~DiagramTypeDialog()
{
    delete d;
}

void DiagramTypeDialog::setDefaultModels( QHash< DiagramType, QAbstractItemModel* > models )
{
    d->m_defaultModels = models;
    for ( QHash< DiagramType, AbstractCoordinatePlane* >::iterator it = d->m_planes.begin(); it != d->m_planes.end(); ++it )
    {
        AbstractDiagram * diagram = it.value()->diagram();
        diagram->setModel( d->m_defaultModels[ it.key() ] );
    }
}

DiagramTypeDialog::DiagramType DiagramTypeDialog::type() const
{
    return d->type;
}
DiagramTypeDialog::Subtype DiagramTypeDialog::subType() const
{
    return d->subType;
}
void DiagramTypeDialog::setType( DiagramType type )
{
    if ( d->type == type )
    {
        d->type = type;
        Q_EMIT diagramTypeChanged( d->type, d->subType );
    }
}
void DiagramTypeDialog::setSubType( Subtype type )
{
    if ( d->subType == type )
    {
        d->subType = type;
        Q_EMIT diagramTypeChanged( d->type, d->subType );
    }
}

#include "diagramtypedialog.moc"
