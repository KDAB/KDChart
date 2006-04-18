#/* -*- Mode: C++ -*-
   KDChart - a multi-platform charting engine
   */

/****************************************************************************
 ** Copyright (C) 2001-2006 Klarälvdalens Datakonsult AB.  All rights reserved.
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
#include <QtDebug>
#include <QPen>
#include "KDChartDefaultsModel.h"
#include "KDChartAttributesModel.h"
#include "KDChartPalette.h"
#include "KDChartDataValueAttributes.h"


using namespace KDChart;

DefaultsModel::DefaultsModel( QObject * parent/* = 0 */ )
  : QAbstractProxyModel( parent ), mDefaultPaletteType( PaletteTypeDefault )
{
}

DefaultsModel::~DefaultsModel()
{
}

/*virtual*/
QModelIndex DefaultsModel::mapFromSource ( const QModelIndex & sourceIndex ) const
{
    return createIndex( sourceIndex.row(), sourceIndex.column(), sourceIndex.internalPointer() );
}

/*virtual*/
QModelIndex DefaultsModel::mapToSource ( const QModelIndex & proxyIndex ) const
{
    return createIndex( proxyIndex.row(), proxyIndex.column(), proxyIndex.internalPointer() );
}


QVariant DefaultsModel::headerData ( int section,
                                     Qt::Orientation orientation,
                                     int role/* = Qt::DisplayRole */ ) const
{

  QVariant sourceData = sourceModel()->headerData( section, orientation, role );
  if ( sourceData.isValid() ) return sourceData;

  // the source model didn't have data set, let's use defaults
  switch ( role ) {
    case Qt::DisplayRole:
      {
        QString header = ( ( orientation == Qt::Vertical ) ?  "Series " : "Item " ) + QString::number( section );
        return header;
        break;
      }
    case KDChart::DatasetBrushRole:
      {
        if ( mDefaultPaletteType == PaletteTypeSubdued )
            return Palette::subduedPalette().getBrush( section );
        else if ( mDefaultPaletteType == PaletteTypeRainbow )
            return Palette::rainbowPalette().getBrush( section );
        else if ( mDefaultPaletteType == PaletteTypeDefault )
            return Palette::defaultPalette().getBrush( section );
        else
            qWarning("Unknown type of fallback palette!");
      }
    case KDChart::DatasetPenRole:
      {
          // default to the color set for the brush (or it's defaults)
          // but only if no per model override was set
          if ( !attributesModel()->modelData( role ).isValid() ) {
              QBrush brush = headerData( section, orientation, DatasetBrushRole ).value<QBrush>();
              return QPen( brush.color() );
          }
      }

    default:
      break;
  }
  return QVariant();
}

QVariant DefaultsModel::data( const QModelIndex& index, int role ) const
{
  // Check the underlying attributes model, and real source model
  // If they have something suitable for this index, fine.
  // Otherwise use defaults.
  QVariant sourceData = sourceModel()->data( index, role );
  if ( sourceData.isValid() ) return sourceData;

  if ( attributesModel()->isKnownAttributesRole( role ) ) {
      // check if there is something set for the column (dataset)
      QVariant v = headerData( index.column(), Qt::Vertical, role );
      if ( !v.isValid() )
          v = attributesModel()->modelData( role );
      if ( !v.isValid() )
          v = defaultsForRole( role );
      return v;
  }

  return QVariant();
}

QVariant DefaultsModel::defaultsForRole( int role ) const
{
    switch ( role ) {
        case KDChart::DataValueLabelAttributesRole:
            return DataValueAttributes::defaultAttributesAsVariant();
            // for the below there isn't a per-value default, since there's a per-column one
        case KDChart::DatasetBrushRole:
        case KDChart::DatasetPenRole:
        default:
            break;
    }
    return QVariant();
}

bool DefaultsModel::setData ( const QModelIndex & index, const QVariant & value, int role )
{
    return sourceModel()->setData( index, value, role );
}

bool DefaultsModel::setHeaderData ( int section, Qt::Orientation orientation,
                                      const QVariant & value, int role )
{
    return sourceModel()->setHeaderData( section, orientation, value, role );
}

QModelIndex DefaultsModel::index( int row, int col, const QModelIndex& index ) const
{
    return sourceModel()->index( row, col, index );
}

QModelIndex DefaultsModel::parent( const QModelIndex& index ) const
{
    return sourceModel()->parent( index );
}

int DefaultsModel::rowCount( const QModelIndex& index ) const
{
    return sourceModel()->rowCount( index );
}

int DefaultsModel::columnCount( const QModelIndex& index ) const
{
    return sourceModel()->columnCount( index );
}


const AttributesModel* DefaultsModel::attributesModel() const
{
    return static_cast<AttributesModel*>( sourceModel() );
}

void DefaultsModel::setDefaultPaletteType( DefaultPaletteType type )
{
    mDefaultPaletteType = type;
}

void DefaultsModel::setSourceModel( QAbstractItemModel* sourceModel )
{
    if( this->sourceModel() != NULL ) {
        disconnect( this->sourceModel(), SIGNAL( dataChanged( const QModelIndex&, const QModelIndex&)),
                                   this, SIGNAL( dataChanged( const QModelIndex&, const QModelIndex&)));
        if( this->sourceModel()->metaObject()->indexOfSignal( QMetaObject::normalizedSignature( "attributesChanged( const QModelIndex&, const QModelIndex& )" ) ) != -1 )
            disconnect( this->sourceModel(), SIGNAL( attributesChanged( const QModelIndex&, const QModelIndex&)),
                        this, SIGNAL( attributesChanged( const QModelIndex&, const QModelIndex&)));
    }

    QAbstractProxyModel::setSourceModel( sourceModel );
    if( this->sourceModel() != NULL ) {
        connect( this->sourceModel(), SIGNAL( dataChanged( const QModelIndex&, const QModelIndex&)),
                                this, SIGNAL( dataChanged( const QModelIndex&, const QModelIndex&)));
        if( this->sourceModel()->metaObject()->indexOfSignal( QMetaObject::normalizedSignature( "attributesChanged( const QModelIndex&, const QModelIndex& )" ) ) != -1 )
            connect( this->sourceModel(), SIGNAL( attributesChanged( const QModelIndex&, const QModelIndex&)),
                     this, SIGNAL( attributesChanged( const QModelIndex&, const QModelIndex&)));
    }
}

