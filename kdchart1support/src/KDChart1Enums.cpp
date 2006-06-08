#include "KDChart1Enums.h"

QString KDChart1Enums::positionFlagToString( PositionFlag type )
{
    switch( type ) {
    case PosTopLeft:
        return "TopLeft";
    case PosTopCenter:
        return "TopCenter";
    case PosTopRight:
        return "TopRight";
    case PosCenterLeft:
        return "CenterLeft";
    case PosCenter:
        return "Center";
    case PosCenterRight:
        return "CenterRight";
    case PosBottomLeft:
        return "BottomLeft";
    case PosBottomCenter:
        return "BottomCenter";
    case PosBottomRight:
        return "BottomRight";
    default: // should not happen
        qDebug( "Unknown content position" );
        return "TopLeft";
    }
}

KDChart1Enums::PositionFlag KDChart1Enums::stringToPositionFlag( const QString& string )
{
    if( string ==      "TopLeft" )
        return PosTopLeft;
    else if( string == "TopCenter" )
        return PosTopCenter;
    else if( string == "TopRight" )
        return PosTopRight;
    else if( string == "CenterLeft" )
        return PosCenterLeft;
    else if( string == "Center" )
        return PosCenter;
    else if( string == "CenterRight" )
        return PosCenterRight;
    else if( string == "BottomLeft" )
        return PosBottomLeft;
    else if( string == "BottomCenter" )
        return PosBottomCenter;
    else if( string == "BottomRight" )
        return PosBottomRight;
    else // default, should not happen
        return PosTopLeft;
}

QPoint KDChart1Enums::positionFlagToPoint( const QRect& rect,
                                          PositionFlag pos )
{
    QPoint pt;
    if( rect.isValid() ) {
        switch( pos ) {
        case KDChart1Enums::PosTopLeft:
            pt = rect.topLeft();
            break;
        case KDChart1Enums::PosTopCenter:
            pt.setY( rect.top() );
            pt.setX( rect.center().x() );
            break;
        case KDChart1Enums::PosTopRight:
            pt = rect.topRight();
            break;
        case KDChart1Enums::PosCenterLeft:
            pt.setY( rect.center().y() );
            pt.setX( rect.left() );
            break;
        case KDChart1Enums::PosCenter:
            pt = rect.center();
            break;
        case KDChart1Enums::PosCenterRight:
            pt.setY( rect.center().y() );
            pt.setX( rect.right() );
            break;
        case KDChart1Enums::PosBottomLeft:
            pt = rect.bottomLeft();
            break;
        case KDChart1Enums::PosBottomCenter:
            pt.setY( rect.bottom() );
            pt.setX( rect.center().x() );
            break;
        case KDChart1Enums::PosBottomRight:
            pt = rect.bottomRight();
            break;
        }
    }
    return pt;
}
