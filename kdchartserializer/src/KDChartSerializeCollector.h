#ifndef __KDCHART_SERIALIZE_COLLECTOR_H__
#define __KDCHART_SERIALIZE_COLLECTOR_H__

#include <QDomDocument>
#include <QDomElement>
#include <QMap>

namespace KDChart {

    class SerializeCollector
    {
        //friend class Serializer; // is allowed to delete the mapper instance

    public:
        static SerializeCollector* instance();

    protected:
        SerializeCollector();
        virtual ~SerializeCollector();

    public:
        /** Returns the QDomElement that was stored for this name.
     *
     * If none was stored before, a new element is created by the document
     * and the name is returned.
     *
     * This is used for saving global elements, like e.g. the
     * list of diagrams, or the list of attribute models, ...
     *
     * Note that the element is not added to any QDomNode,
     * since it is assumed that the calling process will do that
     * afterwards, e.g. KDChart::Serializer::saveChartElement() does that.
     *
     * The QDomElement is owned by the SerializeCollector,
     * so it will be deleted when the collector is deleted.
        */
    QDomElement* findOrMakeElement( QDomDocument& doc, const QString& name );

    /** Returns the QDomElement that was stored for this name.
      *
      * If none was stored before,zero is returned.
      */
    QDomElement* findElement( const QString& name )const;

     private:
         QMap<QString, QDomElement*> mMap;
    };

} // end of namespace

#endif
