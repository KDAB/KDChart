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

#ifndef GRADIENTDIALOG_H
#define GRADIENTDIALOG_H

#include <QDialog>
#include <QGradient>

class GradientDialog : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(QGradient gradient READ gradient WRITE setGradient NOTIFY gradientChanged())

public:
    explicit GradientDialog(QWidget *parent = nullptr);
    ~GradientDialog() override;
    QGradient gradient() const;
    void setGradient(const QGradient &gradient);
    static QGradient getGradient(const QGradient &gradient, QWidget *parent = nullptr, const QString &title = QString());

Q_SIGNALS:
    void gradientChanged();

private:
    class Private;
    Private *d;
};

#endif // GRADIENTDIALOG_H
