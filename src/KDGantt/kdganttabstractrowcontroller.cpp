/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "kdganttabstractrowcontroller.h"

using namespace KDGantt;

/*!\class KDGantt::AbstractRowController kdganttabstractrowcontroller.h KDGanttAbstractRowController
 * \ingroup KDGantt
 * \brief Abstract baseclass for row controllers. A row controller is used
 * by the GraphicsView to nagivate the model and to determine the
 * row geometries
 */

/*! Constructor. Does nothing */
AbstractRowController::AbstractRowController()
{
}

/*! Destructor. Does nothing */
AbstractRowController::~AbstractRowController()
{
}

/*!\fn virtual int AbstractRowController::headerHeight() const = 0
 * \returns The height of the header part of the view.
 *
 * Implement this to control how much space is reserved at
 * the top of the view for a header
 */

/*!\fn virtual int AbstractRowController::totalHeight() const = 0
 * \returns the total height of the rows. For uniformly sized rows that would be
 * number_of_rows*row_height.
 */

/*!\fn virtual bool AbstractRowController::isRowVisible( const QModelIndex& idx ) const
 * \returns true if the row containing index \a idx is visible
 * in the view.
 *
 * Implement this to allow KDGantt to optimize how items on
 * screen are created. It is not harmful to always return true here,
 * but the View will not perform optimally.
 */

/*!\fn virtual Span AbstractRowController::rowGeometry( const QModelIndex& idx ) const
 * \returns A Span consisting of the row offset and height for the row
 * containing \a idx. A simple implementation might look like
 *
 * \code
 * Span MyRowCtrlr::rowGeometry(const QModelIndex& idx)
 * {
 *      return Span(idx.row()*10,10);
 * }
 * \endcode
 */

/*!\fn virtual QModelIndex AbstractRowController::indexBelow( const QModelIndex& idx ) const
 *\returns The modelindex for the next row after \a idx.
 *
 *\see QTreeView::indexBelow
 */

/*!\fn virtual QModelIndex AbstractRowController::indexAbove( const QModelIndex& idx ) const
 *\returns The modelindex for the previous row before \a idx.
 *
 *\see QTreeView::indexAbove
 */
