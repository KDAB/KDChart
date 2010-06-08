#include "KDChartChartSerializer.h"


#include <QDebug>
#include <QtDesignerUI4>


using namespace KDChart;

ChartSerializer::ChartSerializer( )
{
}

ChartSerializer::~ChartSerializer( )
{
}

bool ChartSerializer::saveExtraInfo( const Chart * chart, DomWidget * ui_widget )
{
    Q_UNUSED( chart)

    QList<DomItem*> items;
    DomItem* item = new DomItem;
    item->setText(QLatin1String("Chart Details"));
    items.append( item );

    // PENDING(kalle) Save planes
    // PENDING(kalle) Save legends
    // PENDING(kalle) Save header footers
    // PENDING(kalle) Save global leadings


    ui_widget->setElementItem( items );

    return true;
}


bool ChartSerializer::loadExtraInfo( Chart * chart, DomWidget * ui_widget )
{
    Q_UNUSED( chart)

    const QList<DomItem*> items = ui_widget->elementItem();

    for ( QList<DomItem*>::const_iterator itemit = items.begin(), itemsend = items.end() ;
          itemit != itemsend ; ++itemit )
    {
        DomItem* item = (*itemit);
        QList<DomProperty*> properties = item->elementProperty();

        for( QList<DomProperty*>::const_iterator propsit = properties.begin()
             , propsend = properties.end()
             ; propsit != propsend; ++propsit )
        {
            DomProperty *p = (*propsit);
            if ( p->text() == QLatin1String("Chart Details"))
                qDebug() << "Found a KDChart::Chart";
        }
    }
    return true;
}

