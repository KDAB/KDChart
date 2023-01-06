/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHARTHEADERFOOTER_P_H
#define KDCHARTHEADERFOOTER_P_H

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

#include "KDChartChart.h"
#include "KDChartHeaderFooter.h"
#include "KDChartTextArea_p.h"
#include <QMap>

#include <KDABLibFakes>

class KDTextDocument;

namespace KDChart {
class Chart;
class TextAttributes;
}

/**
 * \internal
 */
class KDChart::HeaderFooter::Private : public KDChart::TextArea::Private
{
    friend class KDChart::HeaderFooter;

public:
    explicit Private();
    ~Private() override;

    Private(const Private &rhs)
        : TextArea::Private(rhs)
        , type(rhs.type)
        , position(rhs.position)
    {
    }

    void updateTextDoc();

private:
    // user-settable
    HeaderFooterType type = Header;
    Position position;
};

inline KDChart::HeaderFooter::HeaderFooter(Private *d, KDChart::Chart *parent)
    : TextArea(d)
{
    setParent(parent);
    init();
}
inline KDChart::HeaderFooter::Private *KDChart::HeaderFooter::d_func()
{
    return static_cast<Private *>(TextArea::d_func());
}
inline const KDChart::HeaderFooter::Private *KDChart::HeaderFooter::d_func() const
{
    return static_cast<const Private *>(TextArea::d_func());
}

#endif /* KDCHARTHEADERFOOTER_P_H */
