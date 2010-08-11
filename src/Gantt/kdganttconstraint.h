#ifndef KDGANTTCONSTRAINT_H
#define KDGANTTCONSTRAINT_H

#include <QMap>
#include <QModelIndex>
#include <QObject>
#include <QSharedDataPointer>
#include <QVariant>

#include "../kdchart_export.h"
#ifndef QT_NO_DEBUG_STREAM
#include <QDebug>
#endif

namespace KDGantt {
    class KDCHART_EXPORT Constraint {
        class Private;
    public:
        enum Type
        {
            TypeSoft = 0,
            TypeHard = 1
        };
        enum RelationType
        {
            FinishStart = 0,
            FinishFinish = 1,
            StartStart = 2,
            StartFinish = 3
        };

        enum ConstraintDataRole
        {
            ValidConstraintPen = Qt::UserRole,
            InvalidConstraintPen
        };

        Constraint();
        Constraint( const QModelIndex& idx1,
                    const QModelIndex& idx2,
                    Type type=TypeSoft,
                    RelationType=FinishStart,
                    const QMap<int, QVariant>& dataMap = QMap<int, QVariant>() );
        Constraint( const Constraint& other);
        ~Constraint();

        Type type() const;
        RelationType relationType() const;
        QModelIndex startIndex() const;
        QModelIndex endIndex() const;

        void setData( int role, const QVariant& value );
        QVariant data( int role ) const;

        void setDataMap( const QMap< int, QVariant >& datamap );
        QMap< int, QVariant > dataMap() const;

        bool compareIndexes(const Constraint& other) const;

        Constraint& operator=( const Constraint& other );
        bool operator==( const Constraint& other ) const;

        inline bool operator!=( const Constraint& other ) const {
            return !operator==( other );
        }

        uint hash() const;
#ifndef QT_NO_DEBUG_STREAM
        QDebug debug( QDebug dbg) const;
#endif

    private:
        QSharedDataPointer<Private> d;
    };

    inline uint qHash( const Constraint& c ) {return c.hash();}
}

#ifndef QT_NO_DEBUG_STREAM
QDebug KDCHART_EXPORT operator<<( QDebug dbg, const KDGantt::Constraint& c );
#endif /* QT_NO_DEBUG_STREAM */

#endif /* KDGANTTCONSTRAINT_H */

