#ifndef KDCHARTPAINTERSAVER_P_H
#define KDCHARTPAINTERSAVER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the KD Chart API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <qpainter.h>

#include <KDABLibFakes>


namespace KDChart {

/**
   \internal

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
    Q_DISABLE_COPY( PainterSaver )
public:
    explicit PainterSaver( QPainter* p )
        : painter( p ) 
    {
#if defined Q_OS_WIN
        static bool initialized = false;
        static bool isQt4_3_0;
        if( !initialized )
        {
            isQt4_3_0 = ( QString::fromLatin1( qVersion() ) == QString::fromLatin1( "4.3.0" ) );
        }
        initialized = true;
        m_isQt4_3_0 = isQt4_3_0;
#endif
        p->save();
    }

    ~PainterSaver()
    {
#if defined Q_OS_WIN
        // the use of setClipRect is a workaround for a bug in Qt 4.3.0 which could
        // lead to an assert on Windows
        if( m_isQt4_3_0 )
        {
            painter->setClipRect( 0, 0, 2, 2 );
        }
#endif
        painter->restore(); 
    }

private:
#if defined Q_OS_WIN
    bool m_isQt4_3_0;
#endif
    QPainter* const painter;
};

}

#endif /* KDCHARTPAINTERSAVER_P_H */

