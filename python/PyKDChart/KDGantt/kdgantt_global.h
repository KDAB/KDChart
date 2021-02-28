/****************************************************************************
** Copyright (C) 2019-2021 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com.
** All rights reserved.
**
** This file is part of the KD Chart library.
**
** Licensees holding valid commercial KD Chart licenses may use this file in
** accordance with the KD Chart Commercial License Agreement provided with
** the Software.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 and version 3 as published by the
** Free Software Foundation and appearing in the file LICENSE.GPL.txt included.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** Contact info@kdab.com if any conditions of this licensing are not
** clear to you.
**
**********************************************************************/

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
