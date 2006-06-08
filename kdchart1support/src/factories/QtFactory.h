#ifndef QTFACTORY_H
#define QTFACTORY_H

#include <qobject.h>
#include <qcolor.h>
#include <qbrush.h>
#include <qfont.h>
#include <qrect.h>
#include <qpen.h>
#include <qstringlist.h>
#include <qpixmap.h>
#include <qlabel.h>

class QtFactory :public QObject
{
    Q_OBJECT
    Q_ENUMS( BrushStyle )
    Q_ENUMS( PenStyle )

public:
    enum PenStyle {
        NoPen = Qt::NoPen,
        SolidLine = Qt::SolidLine,
        DashLine = Qt::DashLine,
        DotLine = Qt::DotLine,
        DashDotLine = Qt::DashDotLine,
        DashDotDotLine = Qt::DashDotDotLine,
        MPenStyle = Qt::MPenStyle
    };

    enum BrushStyle {
        NoBrush = Qt::NoBrush,
        SolidPattern = Qt::SolidPattern,
        Dense1Pattern = Qt::Dense1Pattern,
        Dense2Pattern = Qt::Dense2Pattern,
        Dense3Pattern = Qt::Dense3Pattern,
        Dense4Pattern = Qt::Dense4Pattern,
        Dense5Pattern = Qt::Dense5Pattern,
        Dense6Pattern = Qt::Dense6Pattern,
        Dense7Pattern = Qt::Dense7Pattern,
        HorPattern = Qt::HorPattern,
        VerPattern = Qt::VerPattern,
        CrossPattern = Qt::CrossPattern,
        BDiagPattern = Qt::BDiagPattern,
        FDiagPattern = Qt::FDiagPattern,
        DiagCrossPattern = Qt::DiagCrossPattern,
        CustomPattern = Qt::CustomPattern
    };

public slots:

    //--------------------------------------------------------------------------------
    //                                Brush
    //--------------------------------------------------------------------------------
    QBrush brush( const QColor& color, const QPixmap& pixmap) { return QBrush( color, pixmap ); }
    QBrush brush( const QColor& color, BrushStyle style = SolidPattern ) { return QBrush( color, (Qt::BrushStyle)style ); }
    QBrush brush( BrushStyle style ) { return QBrush( (Qt::BrushStyle) style ); }

    //--------------------------------------------------------------------------------
    //                                QPen
    //--------------------------------------------------------------------------------
    QPen pen( PenStyle style ) { return QPen( (Qt::PenStyle) style ); }
    QPen pen( const QColor & color, uint width = 0, PenStyle style = SolidLine ) {
        return QPen( color, width, (Qt::PenStyle)style );
    }
    QPen pen( const QColor & cl, uint w, PenStyle s, PenCapStyle c, PenJoinStyle j ) {
        return QPen( cl, w, (Qt::PenStyle)s, c, j );
    }
};


#endif /* QTFACTORY_H */

