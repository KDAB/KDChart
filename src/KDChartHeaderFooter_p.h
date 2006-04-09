#ifndef KDCHARTHEADERFOOTER_P_H
#define KDCHARTHEADERFOOTER_P_H

#include "KDChartHeaderFooter.h"
#include "KDChartArea_p.h"
#include <QMap>
class KDTextDocument;

namespace KDChart {
    class TextAttributes;
    class HeaderFooter;
}
using KDChart::TextAttributes;
using KDChart::HeaderFooter;

class KDChart::HeaderFooter::Private : public KDChartArea::Private
{
    friend class KDChart::HeaderFooter;
public:
    Private();
    ~Private();

private:
    // user-settable
    HeaderFooterPosition position;
    QString text;
    TextAttributes textAttributes;

    // internal
    mutable KDTextDocument* textDoc;
    QRect rect;
};

inline KDChart::HeaderFooter::HeaderFooter( Private* d, QWidget* parent )
    : KDChartArea( d, parent ) { init(); }
inline KDChart::HeaderFooter::Private * KDChart::HeaderFooter::d_func()
{ return static_cast<Private*>( KDChartArea::d_func() ); }
inline const KDChart::HeaderFooter::Private * KDChart::HeaderFooter::d_func() const
{ return static_cast<const Private*>( KDChartArea::d_func() ); }




#endif /* KDCHARTHEADERFOOTER_P_H */

