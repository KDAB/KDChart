/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2019-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#define QT_ANNOTATE_ACCESS_SPECIFIER(a) __attribute__((annotate(#a)))

#include <kdganttabstractgrid.h>
#include <kdganttabstractrowcontroller.h>
#include <kdganttconstraint.h>
#include <kdganttconstraintgraphicsitem.h>
#include <kdganttconstraintmodel.h>
#include <kdganttconstraintproxy.h>
#include <kdganttdatetimegrid.h>
#include <kdganttforwardingproxymodel.h>
#include <kdganttglobal.h>
#include <kdganttgraphicsitem.h>
#include <kdganttgraphicsscene.h>
#include <kdganttgraphicsview.h>
#include <kdganttitemdelegate.h>
#include <kdganttlegend.h>
#include <kdganttlistviewrowcontroller.h>
#include <kdganttproxymodel.h>
#include <kdganttstyleoptionganttitem.h>
#include <kdganttsummaryhandlingproxymodel.h>
#include <kdgantttreeviewrowcontroller.h>
#include <kdganttview.h>
