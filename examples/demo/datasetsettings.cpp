#include "datasetsettings.h"
#include "ui_datasetsettings.h"

#include <KDChart/KDChartAbstractCoordinatePlane>
#include <KDChart/KDChartChart>

#include <QtGui/QColorDialog>

#include <QtGui/QCleanlooksStyle>

#include <QtCore/QObject>

#include <QtCore/QDebug>

#include "gradientdialog.h"

using namespace KDChart;

class DatasetSettings::Private : public QObject
{
    Q_OBJECT
public:
    Private( Chart *chart, DatasetSettings *q, QObject *parent );
    ~Private();

    Ui::DatasetSettings *ui;
    int m_dataset;
    int m_datasetCount;
    KDChart::Chart *m_chart;
    DatasetSettings *qq;
public Q_SLOTS:
    void changeColor();
};

DatasetSettings::Private::Private( Chart *chart, DatasetSettings *q, QObject *parent )
    : QObject( parent )
    , ui( new Ui::DatasetSettings )
    , m_dataset( 0 )
    , m_datasetCount( 0 )
    , m_chart( chart )
    , qq( q )
{

}

DatasetSettings::Private::~Private()
{
    delete ui;
}

void DatasetSettings::Private::changeColor()
{
    const int index = ui->datasetSelector->currentIndex();
    if ( ui->Color->isChecked() )
    {
        QBrush setBrush = m_chart->coordinatePlane()->diagram()->brush( index );
        const QColor color = QColorDialog::getColor( setBrush.color(), qq, tr( "Choose new color" ) );
        if ( !color.isValid() )
            return;
        m_chart->coordinatePlane()->diagram()->setBrush( index, color );
        QPalette palette = ui->colorDisplay->palette();
        palette.setBrush( QPalette::Button, color );
        ui->colorDisplay->setPalette( palette );
    }
    else
    {
        QBrush setBrush = m_chart->coordinatePlane()->diagram()->brush( index );
        QGradient grad;
        QLinearGradient lGrad;
        lGrad.setColorAt( 0, Qt::black );
        lGrad.setColorAt( 1, setBrush.color() );
        grad = lGrad;

        if ( setBrush.gradient() )
            grad = *setBrush.gradient();
        const QGradient &color = GradientDialog::getGradient( grad, qq, tr( "Choose new color" ) );
        m_chart->coordinatePlane()->diagram()->setBrush( index, color );
        QPalette palette = ui->colorDisplay->palette();
        palette.setBrush( QPalette::Button, QBrush( color ) );
        ui->colorDisplay->setPalette( palette );
    }
}

DatasetSettings::DatasetSettings( Chart *chart, QWidget *parent )
    : QWidget( parent )
    , d( new Private( chart, this, this ) )
{
    d->ui->setupUi(this);
#ifdef Q_OS_LINUX
    d->ui->colorDisplay->setStyle( new QCleanlooksStyle );
#endif
    connect( d->ui->datasetSelector, SIGNAL( currentIndexChanged( int ) ), this, SLOT( indexChanged( int ) ) );
    connect( d->ui->colorDisplay, SIGNAL( clicked() ), d, SLOT( changeColor() ) );
}

DatasetSettings::~DatasetSettings()
{
    delete d;
}

int DatasetSettings::datasetCount() const
{
    return d->m_datasetCount;
}
void DatasetSettings::setDatasetCount( int value )
{
    if ( d->m_datasetCount != value )
    {
        d->m_datasetCount = value;
        QStringList list;
        for ( int i = 0; i < value; ++i )
        {
            list << tr( "Dataset %1" ).arg( i );
        }
        d->ui->datasetSelector->clear();
        d->ui->datasetSelector->addItems( list );
        Q_EMIT datasetCountChanged();
    }
}

void DatasetSettings::indexChanged( int index )
{
    if ( d->m_chart && index >= 0 )
    {
        QBrush setBrush = d->m_chart->coordinatePlane()->diagram()->brush( index );
        QPalette palette = d->ui->colorDisplay->palette();
        if ( setBrush.gradient() )
            d->ui->radioButton_2->setChecked( true );
        else
            d->ui->Color->setChecked( true );
        palette.setBrush( QPalette::Button, setBrush );
        d->ui->colorDisplay->setPalette( palette );
    }
}

#include "datasetsettings.moc"
