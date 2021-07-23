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

#ifndef DERIVEDADDLEGENDDIALOG_H
#define DERIVEDADDLEGENDDIALOG_H

#include "ui_addlegenddialog.h"

class DerivedAddLegendDialog : public QDialog, public Ui::AddLegendDialog
{
    Q_OBJECT

public:
    DerivedAddLegendDialog(QWidget *parent = nullptr);
};

#endif // DERIVEDADDLEGENDDIALOG_H
