#ifndef QFONTFACTORY_H
#define QFONTFACTORY_H
#include <qobject.h>
#include <qfont.h>

class QFontFactory :public QObject
{
    Q_OBJECT
    Q_ENUMS( StyleHint )
    Q_ENUMS( StyleStrategy )
    Q_ENUMS( Weight )
    Q_ENUMS( Stretch )

public:
    enum StyleHint { Helvetica = QFont::Helvetica, SansSerif = QFont::SansSerif, Times = QFont::Times, Serif = QFont::Serif,
                     Courier = QFont::Courier, TypeWriter = QFont::TypeWriter, OldEnglish = QFont::OldEnglish, Decorative = QFont::Decorative,
                     System = QFont::System, AnyStyle = QFont::AnyStyle };

    enum StyleStrategy { PreferDefault = QFont::PreferDefault, PreferBitmap = QFont::PreferBitmap, PreferDevice = QFont::PreferDevice,
                         PreferOutline = QFont::PreferOutline, ForceOutline = QFont::ForceOutline, PreferMatch = QFont::PreferMatch,
                         PreferQuality = QFont::PreferQuality, PreferAntialias = QFont::PreferAntialias, NoAntialias = QFont::NoAntialias,
                         OpenGLCompatible = QFont::OpenGLCompatible };
    enum Weight { Light = QFont::Light, Normal = QFont::Normal, DemiBold = QFont::DemiBold, Bold = QFont::Bold, Black = QFont::Black };
    enum Stretch { UltraCondensed = QFont::UltraCondensed, ExtraCondensed = QFont::ExtraCondensed, Condensed = QFont::Condensed,
                   SemiCondensed = QFont::SemiCondensed, Unstretched = QFont::Unstretched };
};


#endif /* QFONTFACTORY_H */

