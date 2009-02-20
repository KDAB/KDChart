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
			double	offset(c * 10);
			double	value(offset + (10.0 * rand()) / RAND_MAX);
			setData(index(r, c), value);
		}

	blockSignals(false);

	QModelIndex	topLeft(index(0, 0));
	QModelIndex	bottomRight(index(rowCount() - 1, columnCount() - 1));
	emit dataChanged(topLeft, bottomRight);
} // ModelParamVsParam::timeout()
