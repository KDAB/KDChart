/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHARTHEADERFOOTER_H
#define KDCHARTHEADERFOOTER_H

#include "KDChartPosition.h"
#include "KDChartTextArea.h"

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

    enum HeaderFooterType
    {
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
