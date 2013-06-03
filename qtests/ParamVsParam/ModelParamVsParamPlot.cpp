/****************************************************************************
** Copyright (C) 2001-2013 Klaralvdalens Datakonsult AB.  All rights reserved.
**
** This file is part of the KD Chart library.
**
** Licensees holding valid commercial KD Chart licenses may use this file in
** accordance with the KD Chart Commercial License Agreement provided with
** the Software.
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 and version 3 as published by the
** Free Software Foundation and appearing in the file LICENSE.GPL.txt included.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** Contact info@kdab.com if any conditions of this licensing are not
** clear to you.
**
**********************************************************************/

#include "ModelParamVsParamPlot.h"

#include <QtGui>

#include <KDChartGlobal>


using namespace KDChart;


static int	nrOfValid(0);
static int	nrOfInvalid(0);
static int	nrOfQt(0);
static int	nrOfKDChart(0);


ModelParamVsParamPlot::ModelParamVsParamPlot(
	QAbstractItemModel	*p_sourceModel,
	int						p_column1,
	int						p_column2,
	QObject					*p_parent)
: QSortFilterProxyModel(p_parent),
	m_column1(p_column1),
	m_column2(p_column2)
{
	setSourceModel(p_sourceModel);
} // ModelParamVsParamPlot::ModelParamVsParamPlot()


int ModelParamVsParamPlot::columnCount(
	const QModelIndex	&/*p_parent*/) const
{
	return 2;
} // ModelParamVsParamPlot::columnCount()


QVariant ModelParamVsParamPlot::data(
	const QModelIndex		&p_index,
	int						p_role) const
{
	QVariant			ret;

	if (p_index.isValid())
	{
		nrOfValid++;

		switch (p_role)
		{
			case Qt::DisplayRole:
			case Qt::EditRole:
				nrOfQt++;

				switch (p_index.column())
				{
					case 0: // Parameter1
						ret = sourceModel()->data(sourceModel()->index(p_index.row(), m_column1));
					break;

					case 1: // Parameter2
						ret = sourceModel()->data(sourceModel()->index(p_index.row(), m_column2));
					break;

					default:
						ret = "Invalid column index";
						// Invalid column index cannot occur.
					break;
				} // switch column
			break;

			default:
				nrOfKDChart++;
		} // switch role
	} // if index valid
	else
	{
		nrOfInvalid++;
	}

	return ret;
} // ModelParamVsParamPlot::data()


bool ModelParamVsParamPlot::filterAcceptsColumn(
	int						p_source_column,
	const QModelIndex		&/*p_source_parent*/) const
{
	return p_source_column == m_column1 || p_source_column == m_column2;
} // ModelParamVsParamPlot::filterAcceptsColumn()
