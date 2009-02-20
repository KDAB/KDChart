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
