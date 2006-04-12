/* -*- Mode: C++ -*-
   KDChart - a multi-platform charting engine
   */

/****************************************************************************
 ** Copyright (C) 2001-2003 Klar�vdalens Datakonsult AB.  All rights reserved.
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
#ifndef __KDCHART_DEFAULTS_MODEL_H__
#define __KDCHART_DEFAULTS_MODEL_H__

#include <QAbstractProxyModel>

#include <KDChartGlobal.h>

namespace KDChart {

  class AttributesModel;

class KDCHART_EXPORT DefaultsModel : public QAbstractProxyModel
{
    Q_OBJECT

public:
    DefaultsModel ( QObject * parent = 0 );
    virtual ~DefaultsModel ( );

    enum DefaultPaletteType {
        PaletteTypeDefault = 0,
        PaletteTypeRainbow = 1,
        PaletteTypeSubdued = 2
    };

    void setDefaultPaletteType( DefaultPaletteType type );

    /** \reimpl */
    QModelIndex mapFromSource ( const QModelIndex & sourceIndex ) const;
    /** \reimpl */
    QModelIndex mapToSource ( const QModelIndex & proxyIndex ) const;
    /** \reimpl */
    QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    /** \reimpl */
    QModelIndex index(int, int, const QModelIndex& ) const;
    /** \reimpl */
    QModelIndex parent(const QModelIndex& ) const;
    /** \reimpl */
    int rowCount(const QModelIndex& ) const;
    /** \reimpl */
    int columnCount(const QModelIndex& ) const;
    /** \reimpl */
    QVariant data(const QModelIndex&, int ) const;
    /** \reimpl */
    bool setData ( const QModelIndex & index, const QVariant & value, int role );
    /** \reimpl */
    bool setHeaderData ( int section, Qt::Orientation orientation, const QVariant & value, int role );
    /** \reimpl */
    void setSourceModel( QAbstractItemModel* sourceModel );

signals:
    void attributesChanged( const QModelIndex&, const QModelIndex& );

protected:
    const AttributesModel* attributesModel() const;
    // helper
    QVariant defaultsForRole( int role ) const;
  private:
      DefaultPaletteType mDefaultPaletteType;
};

}

#endif
