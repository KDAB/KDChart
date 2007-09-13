#ifndef KDCHARTABSTRACTPROXYMODEL_H
#define KDCHARTABSTRACTPROXYMODEL_H

/* -*- Mode: C++ -*-
   KDChart - a multi-platform charting engine
   */

/****************************************************************************
 ** Copyright (C) 2007 Klaraelvdalens Datakonsult AB.  All rights reserved.
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

#include <QAbstractProxyModel>

#include "KDChartGlobal.h"

namespace KDChart
{
    /**
      * @brief Base class for all proxy models used inside KD Chart
      * \internal
      */
    class KDCHART_EXPORT AbstractProxyModel : public QAbstractProxyModel
    {
        Q_OBJECT
    public:
        explicit AbstractProxyModel( QObject* parent = 0 );

        /*! \reimpl */ 
        QModelIndex mapFromSource( const QModelIndex & sourceIndex ) const;
        /*! \reimpl */ 
        QModelIndex mapToSource( const QModelIndex &proxyIndex ) const;

        /*! \reimpl */
        QModelIndex index( int row, int col, const QModelIndex& index ) const;
        /*! \reimpl */ 
        QModelIndex parent( const QModelIndex& index ) const;
    };
}

#endif /* KDCHARTABSTRACTPROXYMODEL_H */
