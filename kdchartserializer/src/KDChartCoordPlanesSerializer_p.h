#ifndef __KDCHARTCOORDPLANESSERIALIZER_P_H__
#define __KDCHARTCOORDPLANESSERIALIZER_P_H__

//
//  W A R N I N G
//  -------------
//
// This file is not part of the KD Chart API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <KDChartCoordPlanesSerializer.h>

/**
 * \internal
 */
class KDChart::CoordPlanesSerializer::Private
{
    friend class ::KDChart::CoordPlanesSerializer;
    CoordPlanesSerializer * const q;
public:
    explicit Private( CoordPlanesSerializer * qq );
    ~Private(); // non-virtual, since nothing inherits this

protected:
    DiagramsSerializer* m_diagS;
    QAbstractItemModel* m_model;

    void savePlane( QDomDocument& doc, QDomElement& e, const AbstractCoordinatePlane* p ) const;

    bool doParsePlane( const QDomElement& container, AbstractCoordinatePlane* plane ) const;
    
    bool parseAbstractPlane( const QDomElement& container, AbstractCoordinatePlane& plane ) const;
    void saveAbstractPlane( QDomDocument& doc, QDomElement& e, const AbstractCoordinatePlane& plane, const QString& title ) const;

    bool parseCartPlane( const QDomElement& container, CartesianCoordinatePlane& plane ) const;
    void saveCartPlane( QDomDocument& doc, QDomElement& planeElement, const CartesianCoordinatePlane& plane ) const;

    bool parsePolPlane( const QDomElement& container, PolarCoordinatePlane& plane ) const;
    void savePolPlane( QDomDocument& doc, QDomElement& planeElement, const PolarCoordinatePlane& plane ) const;

    bool parseAxesCalcMode( const QDomElement& container, AbstractCoordinatePlane::AxesCalcMode& mode ) const;
    void saveAxesCalcMode( QDomDocument& doc, QDomElement& e, const CartesianCoordinatePlane::AxesCalcMode& mode, const QString& title ) const;
};


#endif // KDChartCoordPlanesSerializer_p_H
