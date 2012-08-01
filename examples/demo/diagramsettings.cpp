#include "diagramsettings.h"
#include "ui_diagramsettings.h"

#include "gradientdialog.h"

#include <QtGui/QColorDialog>
#include <QtGui/QCleanlooksStyle>

#include <KDChart/KDChartThreeDBarAttributes>

#include <KDChart/KDChartChart>
#include <KDChart/KDChartBarDiagram>
#include <KDChart/KDChartCartesianCoordinatePlane>

#include <KDChart/KDChartBackgroundAttributes>

using namespace KDChart;

class DiagramSettings::Private : public QObject
{
    Q_OBJECT
public:
    Private( Chart *chart = 0, DiagramSettings *q = 0 );
    ~Private();
    void init();
    Chart *m_chart;
    Ui::DiagramSettings *ui;
    DiagramSettings *qq;
public Q_SLOTS:
    void changeThreeD();
    void changeBackgroundColor();
    void changeBackgroundVisibility();
    void changeAutoGradient();
};

DiagramSettings::Private::Private( Chart *chart, DiagramSettings *q )
    : QObject( q )
    , m_chart( chart )
    , ui(new Ui::DiagramSettings)
    , qq( q )
{

}

DiagramSettings::Private::~Private()
{

}

void DiagramSettings::Private::changeAutoGradient()
{

    BarDiagram *bars = qobject_cast< BarDiagram* >( m_chart->coordinatePlane()->diagram() );
    if ( bars )
    {
        ThreeDBarAttributes td( bars->threeDBarAttributes() );
        td.setEnabled( ui->threeDSelector->checkState() == Qt::Checked );
        td.setUseShadowColors( true );
        td.setDepth( ui->barHeightInput->value() );
        td.setThreeDBrushEnabled( ui->autoGradient->checkState() == Qt::Checked );
        bars->setThreeDBarAttributes( td );
        m_chart->update();
    }
}

void DiagramSettings::Private::changeThreeD()
{
    if ( m_chart && m_chart->coordinatePlane() && m_chart->coordinatePlane()->diagram() )
    {
        BarDiagram *bars = qobject_cast< BarDiagram* >( m_chart->coordinatePlane()->diagram() );
        if ( bars )
        {
            ThreeDBarAttributes td( bars->threeDBarAttributes() );
            td.setEnabled( ui->threeDSelector->checkState() == Qt::Checked );
            td.setUseShadowColors( true );
            td.setDepth( ui->barHeightInput->value() );
            td.setThreeDBrushEnabled( ui->autoGradient->checkState() == Qt::Checked );
            bars->setThreeDBarAttributes( td );
            m_chart->update();
        }
    }
}
void DiagramSettings::Private::changeBackgroundColor()
{
    if ( m_chart && m_chart->coordinatePlane() && m_chart->coordinatePlane()->diagram() )
    {
        BackgroundAttributes bat = m_chart->coordinatePlane()->backgroundAttributes();
        bat.setVisible( true );
        ui->visibleBtn->setChecked( true );

        if ( ui->color->isChecked() )
        {

            QBrush setBrush = bat.brush();
            QColor color = QColorDialog::getColor( setBrush.color(), qq, tr( "Choose new color" ) );
            bat.setBrush( color );
            QPalette palette = ui->diagramBackground->palette();
            palette.setBrush( QPalette::Button, color );
            ui->diagramBackground->setPalette( palette );
        }
        else
        {
            QBrush setBrush = bat.brush();
            QGradient grad;
            QLinearGradient lGrad;
            lGrad.setColorAt( 0, Qt::black );
            lGrad.setColorAt( 1, setBrush.color() );
            grad = lGrad;

            if ( setBrush.gradient() )
                grad = *setBrush.gradient();
            const QGradient &color = GradientDialog::getGradient( grad, qq, tr( "Choose new color" ) );
            bat.setBrush( color );
            QPalette palette = ui->diagramBackground->palette();
            palette.setBrush( QPalette::Button, QBrush( color ) );
            ui->diagramBackground->setPalette( palette );
        }
        bat.setVisible( true );
        m_chart->coordinatePlane()->setBackgroundAttributes( bat );
        qq->update();
    }
}

void DiagramSettings::Private::changeBackgroundVisibility()
{
    if ( m_chart && m_chart->coordinatePlane() && m_chart->coordinatePlane()->diagram() )
    {
        BackgroundAttributes bat = m_chart->coordinatePlane()->backgroundAttributes();
        bat.setVisible( ui->visibleBtn->checkState() == Qt::Checked );
        m_chart->coordinatePlane()->setBackgroundAttributes( bat );
        m_chart->update();
    }
}

void DiagramSettings::Private::init()
{
    ui->setupUi( qq );
#ifdef Q_OS_LINUX
    ui->diagramBackground->setStyle( new QCleanlooksStyle );
#endif
    BackgroundAttributes bat = m_chart->coordinatePlane()->backgroundAttributes();
    QBrush setBrush = bat.brush();
    QPalette palette = ui->diagramBackground->palette();
    if ( bat.isVisible() )
        palette.setBrush( QPalette::Button, setBrush );
    else
        palette.setBrush( QPalette::Button, qq->palette().brush( QPalette::Button ) );
    ui->diagramBackground->setPalette( palette );
    connect( ui->threeDSelector, SIGNAL( toggled( bool ) ), this, SLOT( changeThreeD() ) );
    connect( ui->diagramBackground, SIGNAL( clicked() ), this, SLOT( changeBackgroundColor() ) );
    connect( ui->visibleBtn, SIGNAL( toggled( bool ) ), this, SLOT( changeBackgroundVisibility() ) );
    connect( ui->barHeightInput, SIGNAL( valueChanged( int ) ), this, SLOT( changeThreeD() ) );
    connect( ui->autoGradient, SIGNAL( toggled( bool ) ), this, SLOT( changeAutoGradient() ) );

    if ( m_chart && m_chart->coordinatePlane() && m_chart->coordinatePlane()->diagram() )
    {
        BarDiagram *bars = qobject_cast< BarDiagram* >( m_chart->coordinatePlane()->diagram() );
        if ( bars )
        {
            const ThreeDBarAttributes td( bars->threeDBarAttributes() );
            ui->barHeightInput->setValue( td.depth() );
        }
    }
}

DiagramSettings::DiagramSettings( Chart *chart, QWidget *parent )
    : QWidget( parent )
    , d( new Private( chart, this ) )
{
    d->init();
}

DiagramSettings::~DiagramSettings()
{
    delete d;
}

#include "diagramsettings.moc"
