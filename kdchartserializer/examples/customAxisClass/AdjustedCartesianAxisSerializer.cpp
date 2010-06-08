#include "AdjustedCartesianAxisSerializer.h"
#include "AdjustedCartesianAxis.h"

#include <KDXMLTools>

AdjustedCartesianAxisSerializer::AdjustedCartesianAxisSerializer()
{
    // this space left empty intentionally
}

AdjustedCartesianAxisSerializer::~AdjustedCartesianAxisSerializer()
{
    // this space left empty intentionally
}

void AdjustedCartesianAxisSerializer::saveElement( QDomDocument& doc, QDomElement& e, const QObject* obj ) const
{
    const AdjustedCartesianAxis* axis = qobject_cast< const AdjustedCartesianAxis* >( obj );
    if( axis )
    {
        // first we save the base class into the first child element
        QDomElement baseClassElement = doc.createElement( "kdchart:cartesian-axis" );
        e.appendChild( baseClassElement );
        AxesSerializer::saveElement( doc, baseClassElement, obj );
        KDXML::createRealNode( doc, e, "LowerBound", axis->lowerBound() );
        KDXML::createRealNode( doc, e, "UpperBound", axis->upperBound() );
    }
}

bool AdjustedCartesianAxisSerializer::parseElement( const QDomElement& container, QObject* ptr ) const
{
    bool bOK = true;
    AdjustedCartesianAxis* axis = qobject_cast< AdjustedCartesianAxis* >( ptr );
    if( axis )
    {
        const QString containerName = container.tagName();
        qreal upperBound = 0.0;
        qreal lowerBound = 0.0;
        QDomNode node = container.firstChild();
        while( !node.isNull() ) {
            QDomElement element = node.toElement();
            if( !element.isNull() ) { // was really an element
                const QString tagName = element.tagName();
                //qDebug()<<tagName;
                if( tagName == "kdchart:cartesian-axis" ) {
                    if( ! AxesSerializer::parseElement( element, ptr ) )
                        qDebug() << "Error parsing base-class of"<<containerName<<":" << tagName;
                } else if( tagName == "LowerBound" ) {
                    qreal r;
                    if( KDXML::readRealNode( element, r ) )
                        lowerBound = r;
                    else
                        qDebug() << "Error parsing"<<containerName<<"tag:" << tagName;
                } else if( tagName == "UpperBound" ) {
                    qreal r;
                    if( KDXML::readRealNode( element, r ) )
                        upperBound = r;
                    else
                        qDebug() << "Error parsing"<<containerName<<"tag:" << tagName;
                } else {
                    qDebug() << "Unknown subelement of"<<containerName<<"found:" << tagName;
                    bOK = false;
                }
            }
            node = node.nextSibling();
        }
        if( bOK )
            axis->setBounds( lowerBound, upperBound );
    }
    return bOK;
}
