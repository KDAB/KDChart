#include "kdganttconstraintproxy.h"
#include "kdganttconstraintmodel.h"

#include <QAbstractProxyModel>

using namespace KDGantt;

/*!\class KDGantt::ConstraintProxy
 * \internal
 */

ConstraintProxy::ConstraintProxy( QObject* parent )
    : QObject( parent )
{
}

ConstraintProxy::~ConstraintProxy()
{
}

void ConstraintProxy::setSourceModel( ConstraintModel* src )
{
    if ( m_source ) disconnect( m_source );
    m_source = src;

    copyFromSource();

    connect( m_source, SIGNAL( constraintAdded( const KDGantt::Constraint& ) ),
             this, SLOT( slotSourceConstraintAdded( const KDGantt::Constraint& ) ) );
    connect( m_source, SIGNAL( constraintRemoved( const KDGantt::Constraint& ) ),
             this, SLOT( slotSourceConstraintRemoved( const KDGantt::Constraint& ) ) );
}

void ConstraintProxy::setDestinationModel( ConstraintModel* dest )
{
    if ( m_destination ) disconnect( m_destination );
    m_destination = dest;

    copyFromSource();

    connect( m_destination, SIGNAL( constraintAdded( const KDGantt::Constraint& ) ),
             this, SLOT( slotDestinationConstraintAdded( const KDGantt::Constraint& ) ) );
    connect( m_destination, SIGNAL( constraintRemoved( const KDGantt::Constraint& ) ),
             this, SLOT( slotDestinationConstraintRemoved( const KDGantt::Constraint& ) ) );
}

void ConstraintProxy::setProxyModel( QAbstractProxyModel* proxy )
{
    if ( m_proxy == proxy ) return;
    if ( m_proxy  ) disconnect( m_proxy );
    m_proxy = proxy;
    if ( m_proxy ) {
        connect( m_proxy, SIGNAL( layoutChanged() ), this, SLOT( slotLayoutChanged() ) );
        connect( m_proxy, SIGNAL( modelReset() ), this, SLOT( slotLayoutChanged() ) );
    }
}

ConstraintModel* ConstraintProxy::sourceModel() const { return m_source; }
ConstraintModel* ConstraintProxy::destinationModel() const { return m_destination; }
QAbstractProxyModel* ConstraintProxy::proxyModel() const { return m_proxy; }


void ConstraintProxy::copyFromSource()
{
    if ( m_destination ) {
        m_destination->clear();
        if ( !m_source ) return;
        QList<Constraint> lst = m_source->constraints();
        Q_FOREACH( const Constraint& c, lst )
        {
           Constraint temp( m_proxy->mapFromSource( c.startIndex() ), m_proxy->mapFromSource( c.endIndex() ) );
           temp.setData(Constraint::ValidConstraintPen, c.data(Constraint::ValidConstraintPen));
           temp.setData(Constraint::InvalidConstraintPen, c.data(Constraint::InvalidConstraintPen));
           m_destination->addConstraint( temp );
        }
    }
}

void ConstraintProxy::slotSourceConstraintAdded( const KDGantt::Constraint& c )
{
    if ( m_destination )
    {
        Constraint temp( m_proxy->mapFromSource( c.startIndex() ), m_proxy->mapFromSource( c.endIndex() ), c.type(), c.relationType() );
        temp.setData(Constraint::ValidConstraintPen, c.data(Constraint::ValidConstraintPen));
        temp.setData(Constraint::InvalidConstraintPen, c.data(Constraint::InvalidConstraintPen));
        m_destination->addConstraint( temp );
    }
}

void ConstraintProxy::slotSourceConstraintRemoved( const KDGantt::Constraint& c )
{
    if ( m_destination )
    {
        Constraint temp( m_proxy->mapFromSource( c.startIndex() ), m_proxy->mapFromSource( c.endIndex() ), c.type(), c.relationType() );
        temp.setData(Constraint::ValidConstraintPen, c.data(Constraint::ValidConstraintPen));
        temp.setData(Constraint::InvalidConstraintPen, c.data(Constraint::InvalidConstraintPen));
        m_destination->removeConstraint( temp );
    }
}

void ConstraintProxy::slotDestinationConstraintAdded( const KDGantt::Constraint& c )
{
    if ( m_source )
    {
        Constraint temp( m_proxy->mapToSource( c.startIndex() ), m_proxy->mapToSource( c.endIndex() ), c.type(), c.relationType() );
        temp.setData(Constraint::ValidConstraintPen, c.data(Constraint::ValidConstraintPen));
        temp.setData(Constraint::InvalidConstraintPen, c.data(Constraint::InvalidConstraintPen));
        m_source->addConstraint( temp );
    }
}

void ConstraintProxy::slotDestinationConstraintRemoved( const KDGantt::Constraint& c )
{
    if ( m_source )
    {
        Constraint temp( m_proxy->mapToSource( c.startIndex() ), m_proxy->mapToSource( c.endIndex() ), c.type(), c.relationType() );
        temp.setData(Constraint::ValidConstraintPen, c.data(Constraint::ValidConstraintPen));
        temp.setData(Constraint::InvalidConstraintPen, c.data(Constraint::InvalidConstraintPen));
        m_source->removeConstraint( temp );
    }
}

void ConstraintProxy::slotLayoutChanged()
{
    copyFromSource();
}

#include "moc_kdganttconstraintproxy.cpp"
