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

#include "mainwindow.h"

#include "ModelParamVsParam.h"
#include "ModelParamVsParamPlot.h"

#include <QtGui>

#include <KDChartChart>
#include <KDChartDataValueAttributes>
#include <KDChartGridAttributes>
#include <KDChartLineDiagram>


using namespace KDChart;


MainWindow::MainWindow(
    QWidget					*p_parent)
:
    QWidget(p_parent),
	m_sourceModel(new ModelParamVsParam),
	m_timeoutLineEdit(new QLineEdit("1")),
	m_nrOfParametersLineEdit(new QLineEdit("4")),
	m_nrOfSamplesLineEdit(new QLineEdit("4")),
	m_paramVsParamGridLayout(new QGridLayout)
{
	connect(m_timeoutLineEdit, SIGNAL(editingFinished()), SLOT(timeoutEditingFinished()));
	connect(m_nrOfParametersLineEdit, SIGNAL(editingFinished()), SLOT(editingFinished()));
	connect(m_nrOfSamplesLineEdit, SIGNAL(editingFinished()), SLOT(editingFinished()));

	QVBoxLayout	*vBoxLayout = new QVBoxLayout;
	setLayout(vBoxLayout);

	QGridLayout	*gridLayout = new QGridLayout;
	vBoxLayout->addLayout(gridLayout);
	gridLayout->setColumnStretch(2, 1);

	int	row(0);

	gridLayout->addWidget(new QLabel("Timeout [sec]:"), row, 0);
	gridLayout->addWidget(m_timeoutLineEdit, row++, 1);

	gridLayout->addWidget(new QLabel("Number of Parameters:"), row, 0);
	gridLayout->addWidget(m_nrOfParametersLineEdit, row++, 1);

	gridLayout->addWidget(new QLabel("Number of Samples:"), row, 0);
	gridLayout->addWidget(m_nrOfSamplesLineEdit, row++, 1);

	vBoxLayout->addLayout(m_paramVsParamGridLayout);

	m_sourceModel->setTimeout(m_timeoutLineEdit->text().toInt());
	m_sourceModel->populate(m_nrOfParametersLineEdit->text().toInt(), m_nrOfSamplesLineEdit->text().toInt());
	addPlots();
} // MainWindow::MainWindow()


MainWindow::~MainWindow()
{
	delete m_sourceModel;
	m_sourceModel = NULL;
} // MainWindow::~MainWindow()


void MainWindow::timeoutEditingFinished()
{
	if (m_timeoutLineEdit->isModified())
	{
		m_timeoutLineEdit->setModified(false);
		m_sourceModel->setTimeout(m_timeoutLineEdit->text().toInt());
	}
} // MainWindow::timeoutEditingFinished()


void MainWindow::editingFinished()
{
	if (m_nrOfParametersLineEdit->isModified() || m_nrOfSamplesLineEdit->isModified())
	{
		m_nrOfParametersLineEdit->setModified(false);
		m_nrOfSamplesLineEdit->setModified(false);

		m_sourceModel->populate(m_nrOfParametersLineEdit->text().toInt(), m_nrOfSamplesLineEdit->text().toInt());
		m_sourceModel->stopSampling();
		removePlots();
		addPlots();
		m_sourceModel->startSampling();
	}
} // MainWindow::editingFinished()


void MainWindow::addPlots()
{
	for (int r = 0; r < m_sourceModel->columnCount(); r++)
	{
		for (int c = 0; c < m_sourceModel->columnCount(); c++)
		{

			Chart * chart = new Chart;
			m_paramVsParamGridLayout->addWidget(chart, r, c);

			CartesianCoordinatePlane * plane = static_cast<CartesianCoordinatePlane *>(chart->coordinatePlane());

			// Hide grid.
			GridAttributes ga = plane->globalGridAttributes();
			ga.setGridVisible(false);
			plane->setGlobalGridAttributes(ga);

			// Set axes fixed scale.
			qreal						xoffset(c * 10);
			qreal						yoffset(r * 10);
			QPair<qreal, qreal>	horizontalRange(xoffset, xoffset + 10);
			QPair<qreal, qreal>	verticalRange(yoffset, yoffset + 10);
			plane->setHorizontalRange(horizontalRange);
			plane->setVerticalRange(verticalRange);

			if (r == c)
			{
			}
			else
			{
				ModelParamVsParamPlot	*modelParamVsParamPlot =
					new ModelParamVsParamPlot(m_sourceModel, c, r);

				LineDiagram	*lineDiagram = new LineDiagram;
				lineDiagram->setDatasetDimension(2);
				lineDiagram->setModel(modelParamVsParamPlot);
				lineDiagram->setPen(Qt::NoPen);
				setMarkerAttributes(lineDiagram);

				CartesianAxis	*xAxis = new CartesianAxis(lineDiagram);
				CartesianAxis	*yAxis = new CartesianAxis(lineDiagram);
				xAxis->setPosition(CartesianAxis::Bottom);
				yAxis->setPosition(CartesianAxis::Left);
				xAxis->setTitleText('P' + QString::number(c));
				yAxis->setTitleText('P' + QString::number(r));
				lineDiagram->addAxis(xAxis);
				lineDiagram->addAxis(yAxis);

				chart->coordinatePlane()->replaceDiagram(lineDiagram);
			}
		} // for all rows
	} // for all columns
} // MainWindow::addPlots()


void MainWindow::removePlots()
{
	while (m_paramVsParamGridLayout->count())
	{
		Chart *chart = static_cast<Chart *>(m_paramVsParamGridLayout->itemAt(0)->widget());
		m_paramVsParamGridLayout->removeWidget(chart);
		delete chart;
	}
} // MainWindow::removePlots()


void MainWindow::setMarkerAttributes(
	KDChart::LineDiagram		*p_lineDiagram)
{
	QColor								markerColor			= Qt::green;
	MarkerAttributes::MarkerStyle	markerStyle			= MarkerAttributes::Marker4Pixels;
	QColor								firstMarkerColor	= Qt::red;
	MarkerAttributes::MarkerStyle	firstMarkerStyle	= MarkerAttributes::MarkerDiamond;
	DataValueAttributes				dva					= p_lineDiagram->dataValueAttributes();
	MarkerAttributes					ma						= dva.markerAttributes();
	TextAttributes						ta						= dva.textAttributes();

	ma.setVisible(true);
	ma.setMarkerColor(markerColor);
	ma.setMarkerStyle(markerStyle);
	dva.setMarkerAttributes(ma);

	ta.setVisible(false);
	dva.setTextAttributes(ta);

	dva.setVisible(true);
	p_lineDiagram->setDataValueAttributes(0, dva);
	p_lineDiagram->setDataValueAttributes(1, dva);

	// Override for first row.
	ma.setMarkerColor(firstMarkerColor);
	ma.setMarkerStyle(firstMarkerStyle);
	dva.setMarkerAttributes(ma);
	QAbstractItemModel	*model = p_lineDiagram->model();
	p_lineDiagram->setDataValueAttributes(model->index(0, 0), dva);
	p_lineDiagram->setDataValueAttributes(model->index(0, 1), dva);
} // MainWindow::setMarkerAttributes()
