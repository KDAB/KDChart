#ifndef KDCHARTAREA_P_H
#define KDCHARTAREA_P_H

#include "KDChartAbstractArea.h"
#include "KDChartTextAttributes.h"
#include "KDChartFrameAttributes.h"
#include "KDChartBackgroundAttributes.h"

namespace KDChart {

class AbstractArea::Private
{
    friend class AbstractArea;
public:
    explicit Private();
    virtual ~Private();

protected:
    void init();

private:
    bool visible;
    QString text;
    KDChart::TextAttributes textAttributes;
    KDChart::FrameAttributes frameAttributes;
    KDChart::BackgroundAttributes backgroundAttributes;
};

inline AbstractArea::AbstractArea( AbstractArea::Private * p, QWidget* parent ) : QWidget( parent ),_d( p ) { init(); }

}
#endif /* KDCHARTDIAGRAM_P_H */

