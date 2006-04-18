/****************************************************************************
 ** Copyright (C) 2006 Klarälvdalens Datakonsult AB.  All rights reserved.
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
 **   information about KDChart Commercial License Agreements.
 **
 ** Contact info@kdab.net if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/

#ifndef KDCHARTABSTRACTDIAGRAM_P_H
#define KDCHARTABSTRACTDIAGRAM_P_H

#include "KDChartAbstractCoordinatePlane.h"

namespace KDChart {
  class DefaultsModel;
  class AttributesModel;
  class DatasetProxyModel;

class KDChart::AbstractDiagram::Private
{
    friend class AbstractDiagram;
public:
    explicit Private();
    virtual ~Private();

protected:
    void init();
    void init( AbstractCoordinatePlane* plane );
    AbstractCoordinatePlane* plane;
    DefaultsModel* defaultsModel;
    AttributesModel* attributesModel;
    DatasetProxyModel* datasetProxy;
    bool allowOverlappingDataValueTexts;
    bool usePrivateAttributesModel;
    bool percent;
};

inline AbstractDiagram::AbstractDiagram( Private * p ) : _d( p ) { init(); }
inline AbstractDiagram::AbstractDiagram( Private * p, AbstractCoordinatePlane* plane )
  : QAbstractItemView( plane ), _d( p ) { _d->init( plane ); }

class DataValueTextInfo {
public:
  DataValueTextInfo(){}
  DataValueTextInfo( const QModelIndex& _index, const QPointF& _pos, double _value )
    :index( _index ), pos( _pos ), value( _value )
    {}
  DataValueTextInfo( const DataValueTextInfo& other )
    :index( other.index ), pos( other.pos ), value( other.value ) {}
  QModelIndex index;
  QPointF pos;
  double value;
};

typedef QVector<DataValueTextInfo> DataValueTextInfoList;
typedef QVectorIterator<DataValueTextInfo> DataValueTextInfoListIterator;

class LineAttributesInfo {
public :
  LineAttributesInfo() {}
  LineAttributesInfo( const QModelIndex _index, double _value, double _nextValue )
    :index( _index ), value ( _value ), nextValue ( _nextValue )  {}

  QModelIndex index;
  double value;
  double nextValue;
};

typedef QVector<LineAttributesInfo> LineAttributesInfoList;
typedef QVectorIterator<LineAttributesInfo> LineAttributesInfoListIterator;

class DataValueMarkerInfo {
public:
  DataValueMarkerInfo(){}
  DataValueMarkerInfo( const QModelIndex& _index, const QPointF& _pos, double _value )
    :index( _index ), pos( _pos ), value( _value )
    {}
  DataValueMarkerInfo( const DataValueMarkerInfo& other )
    :index( other.index ), pos( other.pos ), value( other.value ) {}
  QModelIndex index;
  QPointF pos;
  double value;
};

typedef QVector<DataValueMarkerInfo> DataValueMarkerInfoList;
typedef QVectorIterator<DataValueMarkerInfo> DataValueMarkerInfoListIterator;

}
#endif /* KDCHARTDIAGRAM_P_H */

