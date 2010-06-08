#include "kdganttconstraintgraphicsitem.h"
#include "kdganttconstraintmodel.h"
#include "kdganttgraphicsscene.h"
#include "kdganttitemdelegate.h"
#include "kdganttsummaryhandlingproxymodel.h"

#include <QPainter>
#include <QDebug>

using namespace KDGantt;

/*!\class KDGantt::ConstraintGraphicsItem
 * \internal
 */

ConstraintGraphicsItem::ConstraintGraphicsItem( const Constraint& c, QGraphicsItem* parent, GraphicsScene* scene )
    : QGraphicsItem( parent, scene ),  m_constraint( c )
{
    setPos( QPointF( 0., 0. ) );
    setAcceptsHoverEvents( false );
    setAcceptedMouseButtons( Qt::NoButton );
    setZValue( 10. );
}

ConstraintGraphicsItem::~ConstraintGraphicsItem()
{
}

int ConstraintGraphicsItem::type() const
{
    return Type;
}

GraphicsScene* ConstraintGraphicsItem::scene() const
{
    return qobject_cast<GraphicsScene*>( QGraphicsItem::scene() );
}

Constraint ConstraintGraphicsItem::proxyConstraint() const
{
    return Constraint( scene()->summaryHandlingModel()->mapFromSource( m_constraint.startIndex() ),
                       scene()->summaryHandlingModel()->mapFromSource( m_constraint.endIndex() ),
                       m_constraint.type() );
}

QRectF ConstraintGraphicsItem::boundingRect() const
{
    return scene()->itemDelegate()->constraintBoundingRect( m_start, m_end, m_constraint );
}

void ConstraintGraphicsItem::paint( QPainter* painter, const QStyleOptionGraphicsItem* option,
                                    QWidget* widget )
{
    Q_UNUSED( widget );

    scene()->itemDelegate()->paintConstraintItem( painter, *option, m_start, m_end, m_constraint );
}

QString ConstraintGraphicsItem::ganttToolTip() const
{
    return m_constraint.data( Qt::ToolTipRole ).toString();
}

void ConstraintGraphicsItem::setStart( const QPointF& start )
{
    prepareGeometryChange();
    m_start = start;
    update();
}

void ConstraintGraphicsItem::setEnd( const QPointF& end )
{
    prepareGeometryChange();
    m_end = end;
    update();
}

void ConstraintGraphicsItem::updateItem( const QPointF& start,const QPointF& end )
{
    setStart( start );
    setEnd( end );
}
