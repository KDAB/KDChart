/* -*- Mode: C++ -*-
   KDChart - a multi-platform charting engine
   */

/****************************************************************************
 ** Copyright (C) 2001-2006 Klarävdalens Datakonsult AB.  All rights reserved.
 **
 ** This file is part of the KD Chart library.
 **
 ** This file may be distributed and/or modified under the terms of the
 ** GNU General Public License version 2 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.
 **
 ** Licensees holding valid commercial KD Chart licenses may use this file in
 ** accordance with the KD Chart Commercial License Agreement provided with
 ** the Software.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** See http://www.kdab.net/kdchart for
 **   information about KD Chart Commercial License Agreements.
 **
 ** Contact info@kdab.net if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/

#include <KDChartAbstractDiagram.h>
#include <KDChartDiagramObserver.h>
#include <KDChartAttributesModel.h>

//FIXME(khz): use an internal libfakes library instead of this internal haeder file
#include "kdchart_platformdef.h"

#include <QDebug>

namespace KDChart {
    DiagramObserver::DiagramObserver( AbstractDiagram& diagram, QObject* parent )
    : QObject( parent ), m_diagram( diagram )
    {
       connect( &m_diagram, SIGNAL(destroyed(QObject*)), SLOT(destroyed()));
       connect( &m_diagram, SIGNAL(modelsChanged()), SLOT(modelsChanged()));
    }

    void DiagramObserver::init()
    {
        if( !m_model.isNull() ) {
            disconnect(m_model);
        }
        if( !m_attributesmodel.isNull() ) {
            disconnect(m_attributesmodel);
        }
        connect( m_diagram.model(), SIGNAL(dataChanged(const QModelIndex&,const QModelIndex&)),
                SLOT(dataChanged()));
        connect( m_diagram.attributesModel(), SIGNAL(attributesChanged(const QModelIndex&,const QModelIndex&)),
                SLOT(attributesChanged()));
        connect( m_diagram.model(), SIGNAL(headerDataChanged(Qt::Orientation,int,int)),
                SLOT(dataChanged()));
        m_model = m_diagram.model();
        m_attributesmodel = m_diagram.attributesModel();
    }

    void DiagramObserver::destroyed()
    {
        emit diagramDestroyed( &m_diagram );
    }

    void DiagramObserver::modelsChanged()
    {
        init();
        dataChanged();
        attributesChanged();
    }

    void DiagramObserver::dataChanged()
    {
        emit diagramDataChanged( &m_diagram );
    }

    void DiagramObserver::attributesChanged()
    {
        emit diagramAttributesChanged( &m_diagram );
    }
}
