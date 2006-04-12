/* -*- Mode: C++ -*-
   KDChart - a multi-platform charting engine
   */

/****************************************************************************
 ** Copyright (C) 2001-2003 Klarävdalens Datakonsult AB.  All rights reserved.
 **
 ** This file is part of the KDChart library.
 **
 ** This file may be distributed and/or modified under the terms of the
 ** GNU General Public License version 2 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.
 **
 ** Licensees holding valid commercial KDChart licenses may use this file in
 ** accordance with the KDChart Commercial License Agreement provided with
 ** the Software.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** See http://www.klaralvdalens-datakonsult.se/?page=products for
 **   information about KDChart Commercial License Agreements.
 **
 ** Contact info@klaralvdalens-datakonsult.se if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/

#include <KDChartAbstractDiagram.h>
#include <KDChartDiagramObserver.h>

#include <QDebug>

namespace KDChart {
    DiagramObserver::DiagramObserver( AbstractDiagram* diagram, QObject* parent )
    : QObject( parent ), diagram(diagram)
    {
       connect( diagram, SIGNAL(destroyed( QObject* )), SLOT(destroyed()));
       connect( diagram->model(), SIGNAL(dataChanged( const QModelIndex&, const QModelIndex&)), SLOT(dataChanged()));
       connect( diagram->model(), SIGNAL(headerDataChanged( Qt::Orientation, int, int)), SLOT(dataChanged()));
    }

    void DiagramObserver::destroyed()
    {
        emit diagramDestroyed( diagram );
    }

    void DiagramObserver::dataChanged()
    {
        emit diagramDataChanged( diagram );
    }
}
