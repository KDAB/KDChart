#include "KDChartAbstractAxis.h"
#include "KDChartAbstractAxis_p.h"
#include <QDebug>
#include <KDChartAbstractDiagram.h>
// #include <QtXml/QDomDocumentFragment>
// #include <KDChartTextAttributes.h>
// #include <QStringList>
// #include <qpen.h>
// #include <qvariant.h>
// #include <QDateTime>
// #include <KDChartAxisParams.h>

using namespace KDChart;


AbstractAxis::Private::Private()
{
    // PENDING(miroslav) Code from KDChartAxis::Private::Private goes here
}


AbstractAxis::Private::~Private()
{
    // PENDING(miroslav) Code from KDChartAxis::Private::~Private goes here
}


AbstractAxis::AbstractAxis ( AbstractDiagram* parent )
    : AbstractArea( new Private(), parent )
{   // FIXME decide internal structure, how to save parent
    init();
}

AbstractAxis::~AbstractAxis()
{
    emit destroyedAxis( this );
}


void AbstractAxis::init()
{
}
