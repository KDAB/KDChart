#ifndef KDCHARTABSTRACTTERNARYDIAGRAM_H
#define KDCHARTABSTRACTTERNARYDIAGRAM_H

namespace KDChart {

    class TernaryCoordinatePlane;

    class AbstractTernaryDiagram : public AbstractDiagram
    {
        Q_OBJECT
        Q_DISABLE_COPY( AbstractTernaryDiagram )
        KDCHART_DECLARE_DERIVED_DIAGRAM( AbstractTernaryDiagram,
                                         TernaryCoordinatePlane )

    public:
        explicit AbstractTernaryDiagram ( QWidget* parent = 0,
                                          TernaryCoordinatePlane* plane = 0 );
        virtual ~AbstractTernaryDiagram();

        void resize (const QSizeF &area);
        void paint (PaintContext *paintContext);

    protected:
        virtual const QPair< QPointF, QPointF >  calculateDataBoundaries () const;

    };

}

#endif
