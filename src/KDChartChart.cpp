/****************************************************************************
 ** Copyright (C) 2006 Klar�vdalens Datakonsult AB.  All rights reserved.
 **
 ** This file is part of the KD Chart library.
 **
 ** This file may be distributed and/or modified under the terms of the
 ** GNU General Public License version 2 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.
 **
 ** Licensees holding valid commercial KD Chart licenses may use this file in
 ** accordance with the KD Chart Commercial License Agreement provided with
 ** the Software.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** See http://www.kdab.net/kdchart for
 **   information about KDChart Commercial License Agreements.
 **
 ** Contact info@kdab.net if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/

#include <QList>
#include <QtDebug>
#include <QGridLayout>
#include <QLabel>
#include <QHash>

#include "KDChartChart.h"
#include "KDChartChart_p.h"
#include "KDChartCartesianCoordinatePlane.h"
#include "KDChartAbstractCartesianDiagram.h"
#include "KDChartHeaderFooter.h"
#include "KDChartLegend.h"

#if defined KDAB_EVAL
#include "../evaldialog/evaldialog.h"
#endif

using namespace KDChart;

void Chart::Private::slotUnregisterDestroyedLegend( Legend *l )
{
    legends.removeAll( l );
}

void Chart::Private::slotUnregisterDestroyedHeaderFooter( HeaderFooter* hf )
{
    headerFooters.removeAll( hf );
}

void Chart::Private::slotUnregisterDestroyedPlane( AbstractCoordinatePlane* plane )
{
    coordinatePlanes.removeAll( plane );
}

Chart::Private::Private( Chart* chart_ )
    : chart( chart_ )
    , layout( 0 )
    , vLayout( 0 )
    , planesLayout( 0 )
    , headerLayout( 0 )
    , footerLayout( 0 )
    , dataAndLegendLayout( 0 )
    , globalLeadingLeft( 0 )
    , globalLeadingRight( 0 )
    , globalLeadingTop( 0 )
    , globalLeadingBottom( 0 )
{
}

void Chart::Private::layoutHeadersAndFooters()
{
    foreach ( HeaderFooter *hf, headerFooters ) {

        // for now, there are only two types of Header/Footer,
        // we use a pointer to the right layout, depending on the type():
        QGridLayout * headerFooterLayout;
        switch( hf->type() ){
            case HeaderFooter::Header:
                headerFooterLayout = headerLayout;
                break;
            case HeaderFooter::Footer:
                headerFooterLayout = footerLayout;
                break;
            default:
                Q_ASSERT( false ); // all types need to be handled
                break;
        };

        if( hf->position() != Position::Unknown ) {
            int row, column;
            Qt::Alignment hAlign, vAlign;
            if( hf->position().isNorthSide() ){
                row = 0;
                vAlign = Qt::AlignTop;
            }
            else if( hf->position().isSouthSide() ){
                row = 2;
                vAlign = Qt::AlignBottom;
            }
            else{
                row = 1;
                vAlign = Qt::AlignVCenter;
            }
            if( hf->position().isWestSide() ){
                column = 0;
                hAlign = Qt::AlignLeft;
            }
            else if( hf->position().isEastSide() ){
                column = 2;
                hAlign = Qt::AlignRight;
            }
            else{
                column = 1;
                hAlign = Qt::AlignHCenter;
            }
            headerFooterLayout->addWidget( hf, row, column, 1, 1, hAlign|vAlign );
        }
        else{
            qDebug( "Unknown header/footer position" );
        }
    }
}


struct LegendPosInfo{
    LegendPosInfo( Legend* legend_, Qt::Alignment hAlign_, Qt::Alignment vAlign_ )
        : legend(legend_)
        , hAlign(hAlign_)
        , vAlign(vAlign_){}
    Legend* legend;
    Qt::Alignment hAlign;
    Qt::Alignment vAlign;
};

void Chart::Private::layoutLegends()
{
    // To support more than one Legend, we first collect them all
    // in little lists: one list per grid position.
    // Since the dataAndLegendLayout is a 3x3 grid, we need 9 little lists.
    // (the middle field, will be used by floating legends, once they are impl'ed)
    QList<LegendPosInfo> infos[3][3];

    foreach ( Legend *legend, legends ) {

        if( legend->position() != Position::Unknown ) {
            int row, column;
            if( legend->position().isNorthSide() )
                row = 0;
            else if( legend->position().isSouthSide() )
                row = 2;
            else
                row = 1;
            if( legend->position().isWestSide() )
                column = 0;
            else if( legend->position().isEastSide() )
                column = 2;
            else
                column = 1;

            if( legend->position() == Position::Center ){
                qDebug( "Sorry: Legend position Center not supported." );
            }else{
                Qt::Alignment hAlign, vAlign;
                const Position& pos = legend->position();
                if( pos.isCorner() ){
                    if( legend->useHorizontalSpace() && legend->useVerticalSpace() ){
                        hAlign = Qt::AlignHCenter;
                        vAlign = Qt::AlignVCenter;
                    }else{
                        if( legend->useHorizontalSpace() ){
                            hAlign = Qt::AlignHCenter;
                            row    = 1;
                            if( pos.isNorthSide() )
                                vAlign = Qt::AlignTop;
                            else
                                vAlign = Qt::AlignBottom;
                        }else if( legend->useVerticalSpace() ){
                            vAlign = Qt::AlignVCenter;
                            column = 1;
                            if( pos.isWestSide() )
                                hAlign = Qt::AlignLeft;
                            else
                                hAlign = Qt::AlignRight;

                        }else{
                            // Floating legend is not supported yet.
                            hAlign = Qt::AlignHCenter;
                            vAlign = Qt::AlignVCenter;
                        }
                    }
                }else{
                    // Legend is not positioned in a corner,
                    // so it is in the middle of a side.
                    hAlign = Qt::AlignHCenter;
                    vAlign = Qt::AlignVCenter;
                }
                infos[row][column] << LegendPosInfo( legend, hAlign, vAlign );
            }
        }
        else{
            qDebug( "Unknown legend position" );
        }
    }
    // We have collected all legend information,
    // so we can design their layout now.
    for (int iR = 0; iR < 3; ++iR) {
        for (int iC = 0; iC < 3; ++iC) {
            QList<LegendPosInfo>& list = infos[iR][iC];
            const int count = list.size();
            switch( count ){
            case 0:
                break;
            case 1: {
                    const LegendPosInfo& info = list.first();
                    dataAndLegendLayout->addWidget(
                        info.legend, iR, iC, 1, 1, info.hAlign|info.vAlign );
                }
                break;
            default: {
                    // We have more than one legend in the same cell
                    // of the big dataAndLegendLayout grid.
                    //
                    // So we need to find out, if they are aligned the
                    // same way:
                    // Those legends, that are aligned the same way, will be drawn
                    // leftbound, on top of each other, in a little VBoxLayout.
                    //
                    // If not al of the legends are aligned the same way,
                    // there will be a grid with 3 cells: for left/mid/right side
                    // (or top/mid/bottom side, resp.) legends
                    LegendPosInfo& info = list.first();
                    Qt::Alignment hAlign = info.hAlign;
                    Qt::Alignment vAlign = info.vAlign;
                    bool haveSameAlign = true;
                    for (int i = 1; i < count; ++i) {
                        info = list.at(i);
                        if( hAlign != info.hAlign ||
                            vAlign != info.vAlign ){
                            haveSameAlign = false;
                            break;
                        }
                    }
                    if( haveSameAlign ){
                        QVBoxLayout* vLayout = new QVBoxLayout();
                        for (int i = 0; i < count; ++i) {
                            vLayout->addWidget(
                                list.at(i).legend, 0, Qt::AlignLeft );
                        }
                        dataAndLegendLayout->addLayout( vLayout, iR, iC, 1, 1, hAlign|vAlign );
                    }else{
                        QGridLayout* gridLayout = new QGridLayout();
                        // middle top cell, or middle bottom cell:
                        if( iC == 1 ){
                            // first we add all left-side legends, then the middle legends,
                            // then the right-side ones:
                            QVBoxLayout* leftLayout = new QVBoxLayout();
                            for (int i = 0; i < count; ++i) {
                                info = list.at(i);
                                if( info.hAlign == Qt::AlignLeft )
                                    leftLayout->addWidget( info.legend, 0, Qt::AlignLeft );
                            }
                            gridLayout->addLayout( leftLayout, 0, 0, Qt::AlignLeft|Qt::AlignTop );

                            QVBoxLayout* midLayout = new QVBoxLayout();
                            for (int i = 0; i < count; ++i) {
                                info = list.at(i);
                                if( info.hAlign == Qt::AlignHCenter )
                                    midLayout->addWidget( info.legend, 0, Qt::AlignHCenter );
                            }
                            gridLayout->addLayout( leftLayout, 0, 1, Qt::AlignLeft|Qt::AlignTop );

                            QVBoxLayout* rightLayout = new QVBoxLayout();
                            for (int i = 0; i < count; ++i) {
                                info = list.at(i);
                                if( info.hAlign == Qt::AlignRight )
                                    rightLayout->addWidget( info.legend, 0, Qt::AlignHCenter );
                            }
                            gridLayout->addLayout( leftLayout, 0, 2, Qt::AlignRight|Qt::AlignTop );

                        // middle of left side, or middle of right side:
                        }else{
                            // first we add all top legends, then the middle legends,
                            // then the bottom ones:
                            QVBoxLayout* topLayout = new QVBoxLayout();
                            for (int i = 0; i < count; ++i) {
                                info = list.at(i);
                                if( info.hAlign == Qt::AlignTop )
                                    topLayout->addWidget( info.legend, 0, Qt::AlignLeft );
                            }
                            gridLayout->addLayout( topLayout, 0, 0, Qt::AlignTop|Qt::AlignLeft );

                            QVBoxLayout* midLayout = new QVBoxLayout();
                            for (int i = 0; i < count; ++i) {
                                info = list.at(i);
                                if( info.hAlign == Qt::AlignVCenter )
                                    midLayout->addWidget( info.legend, 0, Qt::AlignLeft );
                            }
                            gridLayout->addLayout( midLayout, 1, 0, Qt::AlignHCenter|Qt::AlignLeft );

                            QVBoxLayout* bottomLayout = new QVBoxLayout();
                            for (int i = 0; i < count; ++i) {
                                info = list.at(i);
                                if( info.hAlign == Qt::AlignBottom )
                                    bottomLayout->addWidget( info.legend, 0, Qt::AlignLeft );
                            }
                            gridLayout->addLayout( bottomLayout, 2, 0, Qt::AlignBottom|Qt::AlignLeft );

                        }
                        dataAndLegendLayout->addLayout( gridLayout, iR, iC, 1, 1 );
                    }
                }
            }
        }
    }
}



QHash<AbstractCoordinatePlane*, Chart::Private::PlaneInfo> Chart::Private::buildPlaneLayoutInfos()
{
    /* There are two ways in which planes can be caused to interact in
     * where they are put layouting wise: The first is the reference plane. If
     * such a reference plane is set, on a plane, it will use the same cell in the
     * layout as that one. In addition to this, planes can share an axis. In that case
     * they will be layed out in relation to each other as suggested by the position
     * of the axis. If, for example Plane1 and Plane2 share an axis at position Left,
     * that will result in the layout: Axis Plane1 Plane 2, vertically. If Plane1
     * also happens to be Plane2's referece plane, both planes are drawn over each
     * other. The reference plane concept allows two planes to share the same space
     * even if neither has any axis, and in case there are shared axis, it is used
     * to decided, whether the planes should be painted on top of each other or
     * layed out vertically or horizontally next to each other. */
    QHash<CartesianAxis*, AxisInfo> axisInfos;
    QHash<AbstractCoordinatePlane*, PlaneInfo> planeInfos;
    foreach (AbstractCoordinatePlane* plane, coordinatePlanes )
    {
        PlaneInfo p;
        // first check if we share space with another plane
       p.referencePlane = plane->referenceCoordinatePlane();
       planeInfos.insert( plane, p );

        foreach ( AbstractDiagram* abstractDiagram, plane->diagrams() ) {
            AbstractCartesianDiagram* diagram =
                    dynamic_cast<AbstractCartesianDiagram*> ( abstractDiagram );
            if( !diagram ) continue;

            foreach ( CartesianAxis* axis, diagram->axes() ) {
                if ( !axisInfos.contains( axis ) ) {
                    /* If this is the first time we see this axis, add it, with the
                     * current plane. The first plane added to the chart that has
                     * the axis associated with it thus "owns" it, and decides about
                     * layout. */
                    AxisInfo i;
                    i.plane = plane;
                    axisInfos.insert( axis, i );
                } else {
                    AxisInfo i = axisInfos[axis];
                    if ( i.plane == plane ) continue; // we don't want duplicates, only shared

                    /* The user expects diagrams to be added on top, and to the right
                     * so that horizontally we need to move the new diagram, vertically
                     * the reference one. */
                    PlaneInfo pi = planeInfos[plane];
                    // plane-to-plane linking overrides linking via axes
                    if ( !pi.referencePlane ) {
                        // we're not the first plane to see this axis, mark us as a slave
                        pi.referencePlane = i.plane;
                        if ( axis->position() == CartesianAxis::Left
                            ||  axis->position() == CartesianAxis::Right )
                            pi.horizontalOffset += 1;
                        planeInfos[plane] = pi;

                        pi = planeInfos[i.plane];
                        if ( axis->position() == CartesianAxis::Top
                            || axis->position() == CartesianAxis::Bottom  )
                            pi.verticalOffset += 1;

                        planeInfos[i.plane] = pi;
                    }
                }
            }
        }
        // Create a new grid layout for each plane that has no reference.
        p = planeInfos[plane];
        if ( p.referencePlane == 0 ) {
            p.grid = new QGridLayout();
            planeInfos[plane] = p;
        }
    }
    return planeInfos;
}

void Chart::Private::slotLayoutPlanes()
{
    if ( dataAndLegendLayout ) {
        dataAndLegendLayout->removeItem( planesLayout );
    }
    delete planesLayout;
    planesLayout = new QVBoxLayout(); // FIXME is this the right default, I wonder?

    /* First go through all planes and all axes and figure out whether the planes
     * need to coordinate. If they do, they share a grid layout, if not, each
     * get their own. See buildPlaneLayoutInfos() for more details. */
    QHash<AbstractCoordinatePlane*, PlaneInfo> planeInfos = buildPlaneLayoutInfos();
    QHash<AbstractAxis*, AxisInfo> axisInfos;
    foreach ( AbstractCoordinatePlane* plane, coordinatePlanes ) {
        Q_ASSERT( planeInfos.contains(plane) );
        const PlaneInfo pi = planeInfos[plane];
        int column = pi.horizontalOffset;
        int row = pi.verticalOffset;
        QGridLayout *planeLayout = pi.grid;
        if ( !planeLayout ) {
            // this plane is sharing an axis with another one, so use
            // the grid of that one as well
            planeLayout = planeInfos[pi.referencePlane].grid;
        } else {
            planesLayout->addLayout( planeLayout );
        }
        Q_ASSERT( planeLayout );
        /* Put the plane in the center of the layout. If this is our own, that's
         * the middle of the layout, if we are sharing, it's a cell in the center
         * column of the shared grid. */
        planeLayout->addWidget( plane, row, column, Qt::AlignCenter );

        foreach ( AbstractDiagram* abstractDiagram, plane->diagrams() )
        {
            AbstractCartesianDiagram* diagram =
                    dynamic_cast<AbstractCartesianDiagram*> ( abstractDiagram );
            if( !diagram ) continue;  // FIXME polar ?
            // collect all axes of a kind into sublayouts
            QVBoxLayout *topAxes = new QVBoxLayout();
            QVBoxLayout *bottomAxes = new QVBoxLayout();
            QHBoxLayout *leftAxes = new QHBoxLayout();
            QHBoxLayout *rightAxes = new QHBoxLayout();

            foreach ( CartesianAxis* axis, diagram->axes() ) {
                if ( axisInfos.contains( axis ) ) continue; // already layed this one out
                Q_ASSERT ( axis );
                switch ( axis->position() )
                {
                    case CartesianAxis::Top:
                        topAxes->addWidget( axis );
                        break;
                    case CartesianAxis::Bottom:
                        bottomAxes->addWidget( axis );
                        break;
                    case CartesianAxis::Left:
                        leftAxes->addWidget( axis );
                        break;
                    case CartesianAxis::Right:
                        rightAxes->addWidget( axis );
                        break;
                    default:
                        Q_ASSERT_X( false, "Chart::paintEvent",
                                    "unknown axis position" );
                        break;
                };
                axisInfos.insert( axis, AxisInfo() );
            }
            /* Put each stack of axes-layouts in the cells surrounding the
             * associated plane. We are laying out in the oder the planes
             * were added, and the first one gets to lay out shared axes.
             * Private axes go here as well, of course. */
            planeLayout->addLayout( topAxes, 0, 1 );
            planeLayout->addLayout( bottomAxes, row + 1, 1 );
            planeLayout->addLayout( leftAxes, row, 0 );
            planeLayout->addLayout( rightAxes, row, column + 1);
        }

    }
    // re-add our grid(s) to the chart's layout
    if ( dataAndLegendLayout )
        dataAndLegendLayout->addLayout( planesLayout, 1, 1 );
    slotRelayout();
}

void Chart::Private::createLayouts( QWidget* w )
{
    // layout for the planes is handled separately, so we don't want to delete it here
    if ( dataAndLegendLayout) {
        dataAndLegendLayout->removeItem( planesLayout );
        planesLayout->setParent( 0 );
    }
    // nuke the old bunch
    delete layout;

    // The HBox p->layout provides the left and right global leadings
    layout = new QHBoxLayout( w );
    layout->addSpacing( globalLeadingLeft );

    // The vLayout provides top and bottom global leadings and lays
    // out headers/footers and the data area.
    vLayout = new QVBoxLayout();
    layout->addLayout( vLayout, 2 );
    layout->addSpacing( globalLeadingRight );
    vLayout->addSpacing( globalLeadingTop );
    headerLayout = new QGridLayout();
    vLayout->addLayout( headerLayout );
    dataAndLegendLayout = new QGridLayout();
    vLayout->addLayout( dataAndLegendLayout, 2 );
    footerLayout = new QGridLayout();
    vLayout->addLayout( footerLayout );
    vLayout->addSpacing( globalLeadingBottom );

    // the data+axes area
    dataAndLegendLayout->addLayout( planesLayout, 1, 1 );
    dataAndLegendLayout->setRowStretch( 1, 2 );
    dataAndLegendLayout->setColumnStretch( 1, 2 );
}

void Chart::Private::slotRelayout()
{
    createLayouts( chart );
    layoutHeadersAndFooters();
    layoutLegends();
    layout->activate();
}

// ******** Chart interface implementation ***********

Chart::Chart ( QWidget* parent )
    : QWidget ( parent )
    , p ( new Private( this ) )
{
#if defined KDAB_EVAL
    EvalDialog::checkEvalLicense( "KD Chart" );
#endif

    addCoordinatePlane( new CartesianCoordinatePlane ( this ) );
}

Chart::~Chart()
{
}

AbstractCoordinatePlane* Chart::coordinatePlane()
{
    if ( p->coordinatePlanes.isEmpty() )
    {
        qWarning() << "Chart::coordinatePlane: warning: no coordinate plane defined.";
        return 0;
    } else {
        return p->coordinatePlanes.first();
    }
}

CoordinatePlaneList Chart::coordinatePlanes()
{
    return p->coordinatePlanes;
}

void Chart::addCoordinatePlane( AbstractCoordinatePlane* plane )
{
    connect( plane, SIGNAL( destroyedCoordinatePlane( AbstractCoordinatePlane* ) ),
             p, SLOT( slotUnregisterDestroyedPlane( AbstractCoordinatePlane* ) ) );
    connect( plane, SIGNAL( diagramsChanged() ),
             p, SLOT( slotLayoutPlanes() ) );
    p->coordinatePlanes.append( plane );
    plane->setParent( this );
    p->slotLayoutPlanes();
}

void Chart::replaceCoordinatePlane( AbstractCoordinatePlane* plane, int position )
{
    if ( position >=0 && position < p->coordinatePlanes.size() ) {
        connect( plane, SIGNAL( destroyedCoordinatePlane( AbstractCoordinatePlane* ) ),
                 p, SLOT( slotUnregisterDestroyedPlane( AbstractCoordinatePlane* ) ) );
        connect( plane, SIGNAL( diagramsChanged() ),
                 p, SLOT( slotLayoutPlanes() ) );
        AbstractCoordinatePlane* oldPlane = p->coordinatePlanes.at( position );
        p->coordinatePlanes.replace ( position, plane );
        disconnect( oldPlane, SIGNAL( destroyedCoordinatePlane( AbstractCoordinatePlane* ) ),
                    p, SLOT( slotUnregisterDestroyedPlane( AbstractCoordinatePlane* ) ) );
        delete oldPlane;
    }
    p->slotLayoutPlanes();
}

void Chart::removeCoordinatePlane( int position )
{
    if ( position >=0 && position < p->coordinatePlanes.size() ) {
        AbstractCoordinatePlane* oldPlane = p->coordinatePlanes.at( position );
        p->coordinatePlanes.removeAt( position );
        disconnect( oldPlane, SIGNAL( destroyedCoordinatePlane( AbstractCoordinatePlane* ) ),
                    p, SLOT( slotUnregisterDestroyedPlane( AbstractCoordinatePlane* ) ) );
        delete oldPlane;
    }
    p->slotLayoutPlanes();
}

void Chart::setGlobalLeading( int left, int top, int right, int bottom )
{
    setGlobalLeadingLeft( left );
    setGlobalLeadingTop( top );
    setGlobalLeadingRight( right );
    setGlobalLeadingBottom( bottom );
    p->slotRelayout();
}

void Chart::setGlobalLeadingLeft( int leading )
{
    p->globalLeadingLeft = leading;
    p->slotRelayout();
}

int Chart::globalLeadingLeft() const
{
    return p->globalLeadingLeft;
}

void Chart::setGlobalLeadingTop( int leading )
{
    p->globalLeadingTop = leading;
    p->slotRelayout();
}

int Chart::globalLeadingTop() const
{
    return p->globalLeadingTop;
}

void Chart::setGlobalLeadingRight( int leading )
{
    p->globalLeadingRight = leading;
    p->slotRelayout();
}

int Chart::globalLeadingRight() const
{
    return p->globalLeadingRight;
}

void Chart::setGlobalLeadingBottom( int leading )
{
    p->globalLeadingBottom = leading;
    p->slotRelayout();
}

int Chart::globalLeadingBottom() const
{
    return p->globalLeadingBottom;
}


void Chart::paintEvent( QPaintEvent* )
{
}


void Chart::addHeaderFooter( HeaderFooter* headerFooter )
{
    p->headerFooters.append( headerFooter );
    headerFooter->setParent( this );
    connect( headerFooter, SIGNAL( destroyedHeaderFooter( HeaderFooter* ) ),
             p, SLOT( slotUnregisterDestroyedHeaderFooter( HeaderFooter* ) ) );
    connect( headerFooter, SIGNAL( positionChanged( AbstractArea* ) ),
             p, SLOT( slotRelayout() ) );
    p->slotRelayout();
}

void Chart::replaceHeaderFooter( HeaderFooter* headerFooter, int position )
{
    if( p->headerFooters.size() <= position ) {
        addHeaderFooter( headerFooter );
    } else {
        HeaderFooter *old = p->headerFooters.value( position );
        p->headerFooters.replace( position, headerFooter );
        disconnect( old, SIGNAL( destroyedHeaderFooter( HeaderFooter* ) ),
                    p, SLOT( slotUnregisterDestroyedHeaderFooter( HeaderFooter* ) ) );
        delete old;
        connect( headerFooter, SIGNAL( destroyedHeaderFooter( HeaderFooter* ) ),
                 p, SLOT( slotUnregisterDestroyedHeaderFooter( HeaderFooter* ) ) );
    }
    p->slotRelayout();
}

void Chart::removeHeaderFooter( int position )
{
    if( position >= 0 && p->headerFooters.size() > position ) {
        HeaderFooter * old = p->headerFooters.at( position );
        disconnect( old, SIGNAL( destroyedHeaderFooter( HeaderFooter* ) ),
                    p, SLOT( slotUnregisterDestroyedHeaderFooter( HeaderFooter* ) ) );
        delete p->headerFooters.takeAt( position );
    }
    p->slotRelayout();
}

HeaderFooter* Chart::headerFooter()
{
    if( p->headerFooters.isEmpty() ) {
        return 0;
    } else {
        return p->headerFooters.first();
    }
}

HeaderFooterList Chart::headerFooters()
{
    return p->headerFooters;
}

void Chart::addLegend( Legend* legend )
{
    p->legends.append( legend );
    legend->setParent( this );
    connect( legend, SIGNAL( destroyedLegend( Legend* ) ),
             p, SLOT( slotUnregisterDestroyedLegend( Legend* ) ) );
    connect( legend, SIGNAL( positionChanged( AbstractArea* ) ),
             p, SLOT( slotRelayout() ) );
    p->slotRelayout();
}

void Chart::replaceLegend( Legend* legend, int position )
{
    if( p->legends.size() <= position ) {
        addLegend( legend );
    } else {
        Legend *old = p->legends.value( position );
        p->legends.replace( position, legend );
        disconnect( old, SIGNAL( destroyedLegend( Legend* ) ),
                    p, SLOT( slotUnregisterDestroyedLegend( Legend* ) ) );
        delete old;
        connect( legend, SIGNAL( destroyedLegend( Legend* ) ),
                 p, SLOT( slotUnregisterDestroyedLegend( Legend* ) ) );
    }
    p->slotRelayout();
}

void Chart::removeLegend( int position )
{
    if( position >= 0 && p->legends.size() > position ) {
        Legend *old = p->legends.value( position );
        disconnect( old, SIGNAL( destroyedLegend( Legend* ) ),
                    p, SLOT( slotUnregisterDestroyedLegend( Legend* ) ) );
        delete old;
    }
    p->slotRelayout();
}

Legend* Chart::legend()
{
    if ( p->legends.isEmpty() )
    {
        return 0;
    } else {
        return p->legends.first();
    }
}

LegendList Chart::legends()
{
    return p->legends;
}


