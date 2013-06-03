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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include	<QWidget>


class ModelParamVsParam;

class QGridLayout;
class QLineEdit;

namespace KDChart
{
	class	LineDiagram;
};


class	MainWindow:	public QWidget
{
	 Q_OBJECT;

public:
	 MainWindow(
		 QWidget					*p_parent =	0);

	 virtual	~MainWindow();

private slots:
	void timeoutEditingFinished();
	void editingFinished();

private:
	void addPlots();
	void removePlots();
	void setMarkerAttributes(
		KDChart::LineDiagram		*p_lineDiagram);

private:
	ModelParamVsParam		*m_sourceModel;
	QLineEdit				*m_timeoutLineEdit;
	QLineEdit				*m_nrOfParametersLineEdit;
	QLineEdit				*m_nrOfSamplesLineEdit;
	QGridLayout				*m_paramVsParamGridLayout;
};	//	class	MainWindow


#endif // MAINWINDOW_H
