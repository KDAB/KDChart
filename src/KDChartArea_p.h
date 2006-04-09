#ifndef KDCHARTAREA_P_H
#define KDCHARTAREA_P_H

#include <KDChartArea.h>
#include <KDChartTextAttributes.h>
#include <KDChartFrameAttributes.h>
#include <KDChartBackgroundAttributes.h>

class KDChartArea::Private
{
    friend class ::KDChartArea;
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

inline KDChartArea::KDChartArea( KDChartArea::Private * p, QWidget* parent ) : QWidget( parent ),_d( p ) { init(); }


#endif /* KDCHARTDIAGRAM_P_H */

