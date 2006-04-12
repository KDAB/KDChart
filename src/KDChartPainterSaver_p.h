#ifndef KDPAINTERSAVER_P_H
#define KDPAINTERSAVER_P_H

#include <qpainter.h>

namespace KDChart {

/**
   @short Resource Allocation Is Initialization for QPainter::save and
   restore

   Usage:

   Instead of
   \code
   painter.save();
   // ...
   painter.restore();
   \endcode

   Use this:

   \code
   const QPainterSaver saver( &painter );
   // ...
   \endcode

   which makes sure that restore() is called when exiting from guard
   clauses, or when exceptions are thrown.
*/
class PainterSaver {
    QPainter* const painter;
public:
    PainterSaver( QPainter* p ) : painter( p ) { p->save(); }
    ~PainterSaver() { painter->restore(); }

private:
#ifdef Q_DISABLE_COPY
    PainterSaver( const PainterSaver& );
    PainterSaver& operator=( const PainterSaver& );
#endif
};

}

#endif /* KDPAINTERSAVER_P_H */

