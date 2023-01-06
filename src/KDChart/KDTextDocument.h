/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDTEXTDOCUMENT_H
#define KDTEXTDOCUMENT_H

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

#include <QSize>
#include <QTextDocument>

/**
 * KDTextDocument is an internally used enhanced QTextDocument
 * \internal
 */
class KDTextDocument : public QTextDocument
{
    Q_OBJECT

public:
    explicit KDTextDocument(QObject *parent = nullptr);
    explicit KDTextDocument(const QString &text, QObject *parent = nullptr);
    ~KDTextDocument() override;

    QSize sizeHint();
    QSize minimumSizeHint();

private:
    QSize sizeForWidth(int width);

private:
    bool mHintValid = false;
    QSize mSizeHint;
    QSize mMinimumSizeHint;
};

#endif /* KDTEXTDOCUMENT_H */
