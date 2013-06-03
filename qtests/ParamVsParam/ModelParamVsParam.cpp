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

#include "ModelParamVsParam.h"


ModelParamVsParam::ModelParamVsParam(
	QObject					*p_parent)
:
	QStandardItemModel(p_parent)
{
	connect(&m_timer, SIGNAL(timeout()), SLOT(timeout()));
	//m_timer.setSingleShot(true);
	m_timer.setInterval(2000);
} // ModelParamVsParam::ModelParamVsParam()


void ModelParamVsParam::setTimeout(
	int					p_timeout)
{
	m_timer.setInterval(p_timeout * 1000);
} // ModelParamVsParam::setTimeout()


void ModelParamVsParam::startSampling()
{
	m_timer.start();
} // ModelParamVsParam::startSampling()


void ModelParamVsParam::stopSampling()
{
	m_timer.stop();
} // ModelParamVsParam::stopSampling()


void ModelParamVsParam::populate(
	int					p_nrOfParameters,
	int					p_nrOfSamples)
{
	m_timer.stop();

	setColumnCount(p_nrOfParameters);
	setRowCount(p_nrOfSamples);
	timeout();

	m_timer.start();
} // ModelParamVsParam::populate()


void ModelParamVsParam::timeout()
{
	blockSignals(true);

	for (int r = 0; r < rowCount(); r++)
		for (int c = 0; c < columnCount(); c++)
		{
			// First column values in range 0..10, second column in range 10..20 etc.
			qreal	offset(c * 10);
			qreal	value(offset + (10.0 * rand()) / RAND_MAX);
			setData(index(r, c), value);
		}

	blockSignals(false);

	QModelIndex	topLeft(index(0, 0));
	QModelIndex	bottomRight(index(rowCount() - 1, columnCount() - 1));
	emit dataChanged(topLeft, bottomRight);
} // ModelParamVsParam::timeout()
