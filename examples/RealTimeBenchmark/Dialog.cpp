//------------------------------------------------------------------------
// COPYRIGHT (C) 2010
// THIS SOFTWARE IS THE PROPERTY OF ROCHE DIAGNOSTICS GRAZ GMBH, AUSTRIA:
// ALL RIGHTS RESERVED NO PART OF THIS SOFTWARE MAY BE USED OR COPIED IN
// ANY WAY WITHOUT THE PRIOR WRITTEN PERMISSION OF ROCHE DIAGNOSTICS GRAZ
// GMBH, AUSTRIA
//------------------------------------------------------------------------

/*!
 * \file %FILENAME%
 * \brief $MODULE$ Class
 * \author $AUTHOR$ <$EMAIL$>
 */

#include "Dialog.h"
#include "ui_Dialog.h"

#include <KDChartChart>
#include <KDChartLineDiagram>
#include <KDChartPlotter>
#include <KDChartLegend>
#include <KDChartLineDiagram>
#include <KDChartCartesianAxis>
#include <KDChartGridAttributes>
#include <KDChartBackgroundAttributes>
#include <KDChartLegend>
#include <KDChartCartesianCoordinatePlane>

#include <QVariant>
#include <QTimer>
#include <QVector>
#include <QDebug>

#include <cmath>

class TestDataModelDynamic : public QAbstractTableModel
{
Q_OBJECT
public:
    TestDataModelDynamic(QObject * parent ) : QAbstractTableModel(parent), _dataValues(96), _count(0)
    {
        _pTimer = new QTimer(this);
        connect(_pTimer, SIGNAL(timeout()), this, SLOT(addData()));
        _pTimer->setInterval(50);
    }

    //! \reimpl
    virtual int columnCount ( const QModelIndex & parent = QModelIndex() ) const
    { Q_UNUSED(parent); return 96*2; }
    //! \reimpl
    virtual int rowCount ( const QModelIndex & parent = QModelIndex() ) const
    { Q_UNUSED(parent); return _timeValues.count();}

    //! \reimpl
    virtual QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const
    {
        if (index.isValid()
                && index.row() < rowCount()
                && index.column() < columnCount() )
        {
            switch(role)
            {
            case Qt::DisplayRole:
            {
                if (index.column() % 2 == 0)
                {

                    if (index.row() < _timeValues.size())
                        return QVariant(_timeValues[index.row()]);
                }
                else
                {
                    if (index.row() < _dataValues[0].size())
                    {
                        return QVariant(_dataValues[index.column()/2][index.row()]);
                    }
                }
            }
            break;
            default:
            break;
            }
        }
        return QVariant();
    }

signals:
    void stopped();

public slots:
    void onStart()
    {
        _timeValues.clear();
        for (int i=0; i < 96; ++i)
            _dataValues[i].clear();

        _count = 0;
        _pTimer->start();
    }
    void onStop()
    {
        _count = 0;
        _pTimer->stop();
        emit stopped();
    }

private slots:
    void addData()
    {
        //qDebug() << "addData()";
        if (_count > 99)
        {
            onStop();
            return;
        }

        beginInsertRows(QModelIndex(),_count, _count);
        _timeValues.append(qreal(_count));
        for (int i=0; i < 96; ++i)
        {
            qreal value;
            if (_count > i)
                value = 10.0 + 40.0 * (0.5 + (cos(M_PI  + M_PI *(qreal(_count-i)/100.0))/2.0));
            else
                value = 10.0;

            _dataValues[i].append(value);
        }
        endInsertRows();
        //emit dataChanged(index(_count, 0), index(_count, 2*96-1));
        ++_count;
    }

private:
    QVector<qreal>             _timeValues;
    QVector<QVector<qreal> >   _dataValues;
    int                         _count;
    QTimer*                     _pTimer;
};

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::Dialog)
{
    _ui->setupUi(this);
    setupWidget();
}

Dialog::~Dialog()
{
    delete _ui;
}

void Dialog::setupWidget()
{
    connect(_ui->buttonStart, SIGNAL(clicked()), this, SLOT(startSimulation()));
    connect(_ui->buttonStop, SIGNAL(clicked()), this, SLOT(stopSimulation()));

    _ui->buttonStop->setDisabled(true);
    _ui->buttonStart->setDisabled(false);

    _pChart = new KDChart::Chart(_ui->diagramPlaceholder);

    QGridLayout *gridLayout = new QGridLayout(_ui->diagramPlaceholder);
    gridLayout->setContentsMargins(8, 8, 8, 8);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout2"));
    gridLayout->setHorizontalSpacing(12);
    gridLayout->setVerticalSpacing(4);
    gridLayout->addWidget(_pChart, 0, 0, 1, 1);

    _pLineDiagram = new KDChart::Plotter();

    KDChart::CartesianAxis *pLeftAxis = new KDChart::CartesianAxis(_pLineDiagram);
    KDChart::CartesianAxis *pBottomAxis = new KDChart::CartesianAxis(_pLineDiagram);

    pLeftAxis->setPosition(KDChart::CartesianAxis::Left);
    pBottomAxis->setPosition(KDChart::CartesianAxis::Bottom);

    _pLineDiagram->addAxis(pLeftAxis);
    _pLineDiagram->addAxis(pBottomAxis);
    _pLineDiagram->setAntiAliasing(false);

    _pChart->coordinatePlane()->replaceDiagram(_pLineDiagram);


    KDChart::CartesianCoordinatePlane* pPlane = dynamic_cast <KDChart::CartesianCoordinatePlane*>( _pLineDiagram->coordinatePlane() );
    if (pPlane)
    {
        pPlane->setVerticalRange(QPair<qreal, qreal>(0, 65));
        pPlane->setHorizontalRange(QPair<qreal, qreal>(0, 110));
    }

    setupModel();
}

void Dialog::setupModel()
{
    TestDataModelDynamic * pModel = new TestDataModelDynamic(this);
    connect(this, SIGNAL(start()), pModel, SLOT(onStart()));
    connect(this, SIGNAL(stop()), pModel, SLOT(onStop()));
    connect(pModel, SIGNAL(stopped()), this, SLOT(onStopped()));
    _pLineDiagram->setModel(pModel);
    connect( pModel, SIGNAL( rowsInserted( QModelIndex, int, int ) ), _pChart, SLOT( update() ) );
}

void Dialog::onStopped()
{
    _ui->buttonStop->setDisabled(true);
    _ui->buttonStart->setDisabled(false);
}

void Dialog::startSimulation()
{
    _ui->buttonStop->setDisabled(false);
    _ui->buttonStart->setDisabled(true);
    emit start();
}

void Dialog::stopSimulation()
{
    _ui->buttonStop->setDisabled(true);
    _ui->buttonStart->setDisabled(false);
    emit stop();
}

void Dialog::retranslateUi()
{
    setWindowTitle(QApplication::translate("TestDiagram", "Diagram Demo"));
    _ui->buttonStart->setText(QApplication::translate("TestDiagram", "Start"));
    _ui->buttonStop->setText(QApplication::translate("TestDiagram", "Stop"));
}

#include "moc_Dialog.cpp"
#include "Dialog.moc"
