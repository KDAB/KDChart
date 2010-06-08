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

#ifndef __KDCHART_ID_MAPPER_H__
#define __KDCHART_ID_MAPPER_H__

#include <QString>
#include <QMap>

#include <KDChartGlobal>

namespace KDChart {

    /**
     * Auxiliary class used by the KDChart::*Serializer classes.
     *
     * Normally there should be no need to call any of these methods yourself.
     *
     * It is all handled automatically, if you use the main KDChart::Serializer
     * class as entry point of your serialization operations.
     */
    class IdMapper
    {
        //friend class Serializer; // is allowed to delete the mapper instance

        Q_DISABLE_COPY( IdMapper )

        KDCHART_DECLARE_PRIVATE_BASE_POLYMORPHIC( IdMapper )

    public:
        static IdMapper* instance();

        IdMapper();
        virtual ~IdMapper();

    public:
        /** Returns the name that was stored for this pointer.
         *
         * If none was stored before, a new name is created
         * ( basename + ":" + a current number ),
         * the entry is stored and the name is returned.
         */
        QString findOrMakeName( const void* id,
                                const QString& baseName,
                                bool& wasFound,
                                bool useMapOfKnownElements=true );
        /** Returns the name that was stored for this pointer.
         *
         * If none was stored before, a QString() is returned.
         */
        QString findName( const void* id,
                          bool useMapOfKnownElements=true )const;
        /** Returns the pointer that was stored for the given name.
         *
         * If none was stored, it returns zero.
         */
        const void* findId( const QString& name,
                            bool useMapOfKnownElements=true )const;

        /** Delete all collected data.
         */
        void clear();

        /** Return the list of unresolved elements
         */
        const QMap<const void*, QString> unresolvedMap()const;

        /** Prints all stored names and their respective pointers via qDebug().
          */
        void debugOut()const;
    };

} // end of namespace

#endif
