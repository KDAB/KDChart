#include "KDChartWidgetSerializer.h"
#include <QDebug>

#include <QtDesignerUI4>

using namespace KDChart;

WidgetSerializer::WidgetSerializer( )
{
}

WidgetSerializer::~WidgetSerializer( )
{
}

bool KDChart::WidgetSerializer::saveExtraInfo( const Widget * widget, DomWidget * ui_widget )
{
    Q_UNUSED( widget )
    QList<DomItem*> items;
    DomItem* item = new DomItem;
    item->setText(QLatin1String("Widget Details"));
    items.append( item );
    ui_widget->setElementItem( items );
    return true;
}


bool KDChart::WidgetSerializer::loadExtraInfo( Widget * widget, DomWidget * ui_widget )
{
    Q_UNUSED( widget )
    Q_UNUSED( ui_widget )
    return true;
}

