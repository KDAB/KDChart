#ifndef KDCHARTCHART_H
#define KDCHARTCHART_H

#include <QWidget>

#include "kdchart_export.h"

namespace KDChart {

    class AbstractDiagram;
    class AbstractCoordinatePlane;
    class HeaderFooter;
    class Legend;

    class CoordinatePlaneList : public QList<AbstractCoordinatePlane*> {};
    class HeaderFooterList : public QList<HeaderFooter*> {};
    class LegendList : public QList<Legend*> {};

    /**
     * @brief A chart with one or more diagrams.
     *
     * The Chart class represents a drawing consisting of one or more diagrams
     * and various optional elements such as legends, axes, text boxes, headers
     * or footers. It takes ownership of all these elements when they are assigned
     * to it. Each diagram is associated with a coordinate plane, of which the chart
     * can have more than one. The coordinate planes (and thus the associated diagrams)
     * can be layed out in various ways.
     *
     * The Chart class makes heavy use of the Qt Interview framework for model/view
     * programming, and thus requires data to be presented to it in a QAbstractItemModel
     * compatible way. For many simple charts, especially if the visualized data is
     * static, KDChart::Widget provides an abstracted interface, that hides the complexity
     * of Interview to a large extent. *
     */
    class KDCHART_EXPORT Chart : public QWidget
    {
        Q_OBJECT

    public:
        class Private;

        explicit Chart ( QWidget* parent = 0 );
        virtual ~Chart();

        /**
         * Each chart must have at least one coordinate plane.
         * Initially a default CartesianCoordinatePlane is created.
         * Use replaceCoordinatePlane() to replace it with a different
         * one, such as a PolarCoordinatePlane.
         * @return The first coordinate plane of the chart.
         */
        AbstractCoordinatePlane* coordinatePlane();

        /**
         * The list of coordinate planes.
         * @return The list of coordinate planes.
         */
        CoordinatePlaneList coordinatePlanes();

        /**
         * Adds a coordinate plane to the chart. The chart takes ownership.
         * @param plane The coordinate plane to add.
         */
        void addCoordinatePlane( AbstractCoordinatePlane* plane );

        /**
         * Replaces the coordinate plane at the given position with the
         * given coordinate plane. If there is no plane at the
         * specified position, the plane is appended. The chart takes ownership.
         * @param plane The coordinate plane to add.
         * @param position The position at which to replace the plane.
         */
        void replaceCoordinatePlane( AbstractCoordinatePlane* plane, int position = 0 );

        /**
         * Remove the coordinate plane at the given position, if there is one.
         * @param position The position at which to remove the plane. 
         */
        void removeCoordinatePlane( int position );

        /**
         * Remove the given coordinate plane, if it is owned by this chart.
         * @param plane The plane to remove.
         */
        void removeCoordinatePlane( AbstractCoordinatePlane* plane );


        /**
         * The first header or footer of the chart. By default there is none.
         * @return The first header or footer of the chart or 0 if there was none
         * added to the chart.
         */
        HeaderFooter* headerFooter();

        /**
         * The list of headers and footers associated with the chart.
         * @return The list of headers and footers associated with the chart.
         */
        HeaderFooterList headerFooters();

        /**
         * Add a header or footer to the chart. The chart takes ownership.
         * @param headerFooter The header or footer to add.
         */
        virtual void addHeaderFooter( HeaderFooter* headerFooter );

        /**
         * Replace the header or footer to the chart at the given position.
         * The chart takes ownership. If there is no header or footer at the
         * given position, the header or footer is appended. The chart takes ownership.
         * @param headerFooter The header or footer to add.
         * @param position The position at which to add the header or footer.
         */
        virtual void replaceHeaderFooter ( HeaderFooter* headerFooter, int position = 0 );

        /**
         * Remove the header or footer at the given position, if there is one.
         * @param position The position at which to remove the header or footer.
         */
        virtual void removeHeaderFooter( int position = 0 );

        /**
         * The first legend of the chart or 0 if there was none added to the chart.
         * @return The first legend of the chart or 0 if none exists.
         */
        Legend* legend();

        /**
         * The list of all legends associated with the chart.
         * @return The list of all legends associated with the chart.
         */
        LegendList legends();

        /**
         * Add the given legend to the chart. The chart takes ownership.
         * @param legend The legend to add.
         */
        virtual void addLegend( Legend* legend );

        /**
         * Replace the legend at the given position. The chart takes ownership.
         * @param legend The legend to replace with.
         * @param position The position at which to replace the legend.
         */
        virtual void replaceLegend ( Legend* legend, int position = 0 );

        /**
         * Remove the legend at the given position, if there is one.
         * @param position The position at which to remove the legend.
         */
        virtual void removeLegend( int position = 0 );

        /**
         * Set the padding between the margin of the widget and the area that
         * the contents are drawn into.
         * @param left The padding on the left side.
         * @param top The padding at the top.
         * @param right The padding on the left hand side.
         * @param bottom The padding on the bottom.
         */
        void setGlobalLeading( int left, int top, int right, int bottom );

        /**
         * Set the padding between the start of the widget and the start
         * of the area that is used for drawing on the left.
         * @param leading The padding value.
         */
        void setGlobalLeadingLeft( int leading );

        /**
         * The padding between the start of the widget and the start
         * of the area that is used for drawing on the left.
         * @return The padding between the start of the widget and the start
         * of the area that is used for drawing on the left.
         */
        int globalLeadingLeft() const;

        /**
         * Set the padding between the start of the widget and the start
         * of the area that is used for drawing at the top.
         * @param leading The padding value.
         */
        void setGlobalLeadingTop( int leading );

        /**
         * The padding between the start of the widget and the start
         * of the area that is used for drawing at the top.
         * @return The padding between the start of the widget and the start
         * of the area that is used for drawing at the top.
         */
        int globalLeadingTop() const;

        /**
         * Set the padding between the start of the widget and the start
         * of the area that is used for drawing on the right.
         * @param leading The padding value.
         */
        void setGlobalLeadingRight( int leading );

        /**
         * The padding between the start of the widget and the start
         * of the area that is used for drawing on the right.
         * @return The padding between the start of the widget and the start
         * of the area that is used for drawing on the right.
         */
        int globalLeadingRight() const;

        /**
         * Set the padding between the start of the widget and the start
         * of the area that is used for drawing on the bottom.
         * @param leading The padding value.
         */
        void setGlobalLeadingBottom( int leading );

        /**
         * The padding between the start of the widget and the start
         * of the area that is used for drawing at the bottom.
         * @return The padding between the start of the widget and the start
         * of the area that is used for drawing at the bottom.
         */
        int globalLeadingBottom() const;

    protected:
        void paintEvent( QPaintEvent* );

    private:
        Private* p;
    };

}

#endif
