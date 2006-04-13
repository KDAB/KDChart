/* -*- Mode: C++ -*-
   KDChart - a multi-platform charting engine
   */

/****************************************************************************
 ** Copyright (C) 2001-2003 Klarälvdalens Datakonsult AB.  All rights reserved.
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
#include <QDebug>
#include <QPen>
#include "KDChartAttributesModel.h"
#include "KDChartPalette.h"
#include "KDChartDataValueAttributes.h"
#include "KDChartGlobal.h"

using namespace KDChart;

struct AttributesModelRegistryInfo {
  AttributesModel* attributeModel;
  int refcount;
};

static QHash< QAbstractItemModel*, AttributesModelRegistryInfo > s_buddyHash;

/*static*/
AttributesModel * AttributesModel::instanceForModel( QAbstractItemModel* model )
{
    AttributesModelRegistryInfo info;
    if ( s_buddyHash.contains( model ) ) {
        info = s_buddyHash.value( model );
    } else {
        info.attributeModel = new AttributesModel();
    }
    info.refcount++;
    s_buddyHash.insert( model, info );
    return info.attributeModel;
}

/*static*/
void AttributesModel::deref( QAbstractItemModel* model )
{
    if ( s_buddyHash.contains( model ) ) {
        AttributesModelRegistryInfo info = s_buddyHash.value( model );
        info.refcount--;
        if ( info.refcount == 0 ) {
            delete info.attributeModel;
            s_buddyHash.remove( model );
        } else {
            s_buddyHash.insert( model, info );
        }
    }
}

AttributesModel::AttributesModel( QObject * parent/* = 0 */ )
  : QAbstractProxyModel( parent )
{
}

AttributesModel::~AttributesModel()
{
}

/*virtual*/
QModelIndex AttributesModel::mapFromSource ( const QModelIndex & sourceIndex ) const
{
    return createIndex( sourceIndex.row(), sourceIndex.column(), sourceIndex.internalPointer() );
}

/*virtual*/
QModelIndex AttributesModel::mapToSource ( const QModelIndex & proxyIndex ) const
{
    return createIndex( proxyIndex.row(), proxyIndex.column(), proxyIndex.internalPointer() );
}


QVariant AttributesModel::headerData ( int section,
                                       Qt::Orientation orientation,
                                       int role/* = Qt::DisplayRole */ ) const
{
  QVariant sourceData = sourceModel()->headerData( section, orientation, role );
  if ( sourceData.isValid() ) return sourceData;
  // the source model didn't have data set, let's use our stored values
  const QMap<int, QMap<int, QVariant> >& map = orientation == Qt::Horizontal ? mHorizontalHeaderDataMap : mVerticalHeaderDataMap;
  if ( map.contains( section ) ) {
      const QMap<int, QVariant> &dataMap = map[ section ];
      if ( dataMap.contains( role ) ) {
          return dataMap[ role ];
      }
  }
  return QVariant();
}

QVariant AttributesModel::data( const QModelIndex& index, int role ) const
{
  QVariant sourceData = sourceModel()->data( index, role );
  if ( sourceData.isValid() ) return sourceData;
  // check if we are storing a value for this role at this index
  if ( mDataMap.contains( index.column() ) ) {
      const QMap< int,  QMap< int, QVariant> > &colDataMap = mDataMap[ index.column() ];
      if ( colDataMap.contains( index.row() ) ) {
          const QMap<int, QVariant> &dataMap = colDataMap[ index.row() ];
          if ( dataMap.contains( role ) )
              return dataMap[ role ];
      }
  }
  return QVariant();
}

bool AttributesModel::isKnownAttributesRole( int role ) const
{
    bool oneOfOurs = false;
    switch( role ) {
      // fallthrough intended
      case DataValueLabelAttributesRole:
      case DatasetBrushRole:
      case DatasetPenRole:
      case ThreeDAttributesRole:
      case LineAttributesRole:
      case ThreeDLineAttributesRole:
      case BarAttributesRole:
      case ThreeDBarAttributesRole:
      case ExplodeFactorRole:
          oneOfOurs = true;
        default:
        break;
    }
    return oneOfOurs;
}

bool AttributesModel::setData ( const QModelIndex & index, const QVariant & value, int role )
{
    if ( !isKnownAttributesRole( role ) ) {
        return sourceModel()->setData( index, value, role );
    } else {
        QMap< int,  QMap< int, QVariant> > &colDataMap = mDataMap[ index.column() ];
        QMap<int, QVariant> &dataMap = colDataMap[ index.row() ];
        //qDebug() <<  "AttributesModel::setData" <<"role" << role << "value" << value;
        dataMap.insert( role, value );
        emit attributesChanged( index, index );
        return true;
    }
}

bool AttributesModel::setHeaderData ( int section, Qt::Orientation orientation,
                                      const QVariant & value, int role )
{
    if ( !isKnownAttributesRole( role ) ) {
        return sourceModel()->setHeaderData( section, orientation, value, role );
    } else {
        QMap<int,  QMap<int, QVariant> > &sectionDataMap
            = orientation == Qt::Horizontal ? mHorizontalHeaderDataMap : mVerticalHeaderDataMap;
        QMap<int, QVariant> &dataMap = sectionDataMap[ section ];
        dataMap.insert( role, value );
        emit attributesChanged( index( 0, section, QModelIndex() ),
                                index( rowCount( QModelIndex() ), section, QModelIndex() ) );
        return true;
    }
}


bool KDChart::AttributesModel::setModelData( const QVariant value, int role )
{
     mModelDataMap.insert( role, value );
     emit attributesChanged( index( 0, 0, QModelIndex() ),
                             index( rowCount( QModelIndex() ),
                                    columnCount( QModelIndex() ), QModelIndex() ) );
     return true;
}

QVariant KDChart::AttributesModel::modelData( int role ) const
{
    return mModelDataMap.value( role, QVariant() );
}

QModelIndex AttributesModel::index( int row, int col, const QModelIndex& index ) const
{
    Q_ASSERT(sourceModel());
    return sourceModel()->index( row, col, index );
}

QModelIndex AttributesModel::parent( const QModelIndex& index ) const
{
    Q_ASSERT(sourceModel());
    return sourceModel()->parent( index );
}

int AttributesModel::rowCount( const QModelIndex& index ) const
{
    Q_ASSERT(sourceModel());
    return sourceModel()->rowCount( index );
}

int AttributesModel::columnCount( const QModelIndex& index ) const
{
    Q_ASSERT(sourceModel());
    return sourceModel()->columnCount( index );
}

void AttributesModel::setSourceModel( QAbstractItemModel* sourceModel )
{
    if( this->sourceModel() != 0 )
        disconnect( this->sourceModel(), SIGNAL( dataChanged( const QModelIndex&, const QModelIndex&)),
                                   this, SIGNAL( dataChanged( const QModelIndex&, const QModelIndex&)));
    QAbstractProxyModel::setSourceModel( sourceModel );
    if( this->sourceModel() != NULL )
        connect( this->sourceModel(), SIGNAL( dataChanged( const QModelIndex&, const QModelIndex&)),
                                this, SIGNAL( dataChanged( const QModelIndex&, const QModelIndex&)));
}
