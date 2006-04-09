#ifndef KDCHARTABSTRACTDIAGRAM_P_H
#define KDCHARTABSTRACTDIAGRAM_P_H

#include "KDChartCoordinatePlane.h"

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
    void init( CoordinatePlane* plane );
    CoordinatePlane* plane;
    DefaultsModel* defaultsModel;
    AttributesModel* attributesModel;
    DatasetProxyModel* datasetProxy;
    bool allowOverlappingDataValueTexts;
    bool usePrivateAttributesModel;
};

inline AbstractDiagram::AbstractDiagram( Private * p ) : _d( p ) { init(); }
inline AbstractDiagram::AbstractDiagram( Private * p, CoordinatePlane* plane )
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

