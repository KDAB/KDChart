#ifndef KDCHARTABSTRACTAREABASE_H
#define KDCHARTABSTRACTAREABASE_H

#include <QPointF>
#include <QSizeF>
#include <QRectF>

#include "KDChartGlobal.h"
#include "KDChartLayoutItems.h"
#include "KDChartRelativePosition.h"
#include "KDChartAbstractAreaBase.h"


class QPainter;
class QString;
namespace KDChart {
    class TextAttributes;
    class BackgroundAttributes;
    class FrameAttributes;
    class PaintContext;


/**
  * @class AbstractAreaBase KDChartAbstractAreaBase.h
  * @brief Base class for AbstractArea and AbstractAreaWidget: An area
  * in the chart with a background, a frame, etc.
  *
  * AbstractAreaBase is the base class for all chart elements that have
  * a set of background attributes and frame attributes, such as
  * legends or axes.
  *
  * @note Normally you should not use AbstractAreaBase directly, but
  * derive your classes from AbstractArea or AbstractAreaWidget.
  *
  * @note This classis not a QObject, so it is easier to inherit from
  * it, if your are inheriting from a QObject too like AbstractAreaWidget does it.
  *
  * @sa AbstractArea, AbstractAreaWidget
  */
class KDCHART_EXPORT AbstractAreaBase
{
    Q_DISABLE_COPY( AbstractAreaBase )
    KDCHART_DECLARE_PRIVATE_BASE_POLYMORPHIC( AbstractAreaBase )

protected:
    AbstractAreaBase();
    virtual ~AbstractAreaBase() ;

public:
//    virtual AbstractAreaBase * clone() const = 0;

    /**
     * Returns true if both areas have the same settings.
     */
    bool compare( const AbstractAreaBase* other )const;

    void alignToReferencePoint( const RelativePosition& position );

    void setFrameAttributes( const FrameAttributes &a );
    FrameAttributes frameAttributes() const;

    void setBackgroundAttributes( const BackgroundAttributes &a );
    BackgroundAttributes backgroundAttributes() const;

    virtual void paintBackground( QPainter& painter, const QRect& rectangle );
    virtual void paintFrame( QPainter& painter, const QRect& rectangle );

    static void paintBackgroundAttributes( QPainter& painter, const QRect& rectangle,
        const KDChart::BackgroundAttributes& attributes );
    static void paintFrameAttributes( QPainter& painter, const QRect& rectangle,
        const KDChart::FrameAttributes& attributes );

    /** \internal
      * \note Normally you should not call this method, but derive your classes
      * from AbstractArea or AbstractAreaWidget.
      * \sa AbstractArea, AbstractAreaWidget
      */
    void getFrameLeadings(int& left, int& top, int& right, int& bottom ) const;


protected:
    /** \internal
      * \note Normally you should not call this method, but derive your classes
      * from AbstractArea or AbstractAreaWidget.
      * \sa AbstractArea, AbstractAreaWidget
      */
    QRect innerRect() const;

    /** \internal
      * This internal method is used by AbstractArea and AbstractAreaWidget
      * to find out the real widget size.
      * \sa AbstractArea, AbstractAreaWidget
      */
    virtual QRect areaGeometry() const = 0;

    /** \internal
      * This internal method can be overwritten by derived classes,
      * if they want to emit a signal (or perform other actions, resp.)
      * when the Position of the area has been changed.
      * The default implementation does nothing.
      */
    virtual void positionHasChanged();

}; // End of class AbstractAreaBase

}
#endif // KDCHARTABSTRACTAREABASE_H
