#ifndef __KDCHARTTEXTAREASERIALIZER_P_H__
#define __KDCHARTTEXTAREASERIALIZER_P_H__

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

#include <KDChartTextAreaSerializer.h>

namespace KDChart
{
    class HeaderFooter;
}

/**
 * \internal
 */
class KDChart::TextAreaSerializer::Private
{
    friend class ::KDChart::TextAreaSerializer;
    TextAreaSerializer * const q;
public:
    explicit Private( TextAreaSerializer * qq );
    ~Private(); // non-virtual, since nothing inherits this

protected:
    static bool doParseHeaderFooter( const QDomElement& container, HeaderFooter*& ptr );

    static void saveHeaderFooter( QDomDocument& doc, QDomElement& hdFtElement, const KDChart::HeaderFooter& hdFt );

    static bool parseTextArea( const QDomElement& container, TextArea& area );
    static void saveTextArea( QDomDocument& doc, QDomElement& e, const TextArea& area, const QString& title );

    static bool parseTextLayoutItem( const QDomElement& container, TextLayoutItem& item );
    static void saveTextLayoutItem( QDomDocument& doc, QDomElement& e, const TextLayoutItem& item, const QString& title );
};


#endif // KDChartTextAreaSerializer_p_H
