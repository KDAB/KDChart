#include <cmath>

#include "TernaryConstants.h"

extern const double Sqrt3 = sqrt( 3.0 );
extern const double TriangleWidth = 1.0;
extern const double TriangleHeight = 0.5 * Sqrt3;
extern const QPointF TriangleTop( 0.5, TriangleHeight );
extern const QPointF TriangleBottomLeft( 0.0, 0.0 );
extern const QPointF TriangleBottomRight( 1.0, 0.0 );
extern const QPointF AxisVector_C_A( TriangleTop - TriangleBottomRight );
extern const QPointF Norm_C_A( -AxisVector_C_A.y(), AxisVector_C_A.x() );
extern const QPointF AxisVector_B_A( TriangleTop );
extern const QPointF Norm_B_A( -AxisVector_B_A.y(), AxisVector_B_A.x() );
extern const QPointF AxisVector_B_C( TriangleBottomRight );
extern const QPointF Norm_B_C( -AxisVector_B_C.y(), AxisVector_B_C.x() );

extern const double RelMarkerLength = 0.03 * TriangleWidth;
extern const QPointF FullMarkerDistanceBC( RelMarkerLength * Norm_B_C );
extern const QPointF FullMarkerDistanceAC( -RelMarkerLength * Norm_C_A );
extern const QPointF FullMarkerDistanceBA( RelMarkerLength * Norm_B_A );

