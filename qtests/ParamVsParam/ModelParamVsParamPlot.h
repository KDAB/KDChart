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

#ifndef SUMGUI_MODELPARAMVSPARAMPLOT_H
#define SUMGUI_MODELPARAMVSPARAMPLOT_H


#include <QSortFilterProxyModel>


class ModelParamVsParamPlot: public QSortFilterProxyModel
{
public:
	ModelParamVsParamPlot(
		QAbstractItemModel	*p_sourceModel,
		int						p_column1,
		int						p_column2,
		QObject					*p_parent = 0);

	int columnCount(
		const QModelIndex	&p_parent = QModelIndex()) const;

protected:
	QVariant data(
		const QModelIndex		&p_index,
		int						p_role = Qt::DisplayRole) const;

	virtual bool filterAcceptsColumn(
		int						p_source_column,
		const QModelIndex		&p_source_parent) const;

private:
	int					m_column1;
	int					m_column2;
}; // class ModelParamVsParamPlot


#endif // SUMGUI_MODELPARAMVSPARAMPLOT_H
