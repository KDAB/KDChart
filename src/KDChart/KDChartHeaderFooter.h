/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2021 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDAB-KDChart OR LicenseRef-KDAB-KDChart-US
**
** Licensees holding valid commercial KD Chart licenses may use this file in
** accordance with the KD Chart Commercial License Agreement provided with
** the Software.
**
** Contact info@kdab.com if any conditions of this licensing are not
** clear to you.
**
****************************************************************************/

#ifndef KDCHARTHEADERFOOTER_H
#define KDCHARTHEADERFOOTER_H

#include "KDChartTextArea.h"
#include "KDChartPosition.h"

namespace KDChart {

class Chart;
class TextAttributes;

/**
  * @brief A header or footer displaying text above or below charts
  */
class KDCHART_EXPORT HeaderFooter : public TextArea
{
    Q_OBJECT

    KDCHART_DECLARE_PRIVATE_DERIVED_PARENT(HeaderFooter, Chart *)

public:
    HeaderFooter(Chart *parent = nullptr);
    ~HeaderFooter() override;

    virtual HeaderFooter *clone() const;

    bool compare(const HeaderFooter &other) const;

    enum HeaderFooterType {
        Header,
        Footer
    };

    void setType(HeaderFooterType type);
    HeaderFooterType type() const;

    void setPosition(Position position);
    Position position() const;

    void setParent(QObject *parent);

Q_SIGNALS:
    void destroyedHeaderFooter(HeaderFooter *);
    void positionChanged(HeaderFooter *);

}; // End of class HeaderFooter
}

#endif // KDCHARTHEADERFOOTER_H
