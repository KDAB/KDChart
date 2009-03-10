#include <QtGui>

#include <KDChartChart>
#include <KDChartGridAttributes>
#include <KDChartFrameAttributes>
#include <KDChartTextAttributes>
#include <KDChartDataValueAttributes>
#include <KDChartMarkerAttributes>
#include <KDChartPlotter>
#include <KDChartLegend>
#include <KDChartBackgroundAttributes>

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	KDChart::Chart chart;

	QStandardItemModel model;

	model.setRowCount(100);
	model.setColumnCount(6); // 3 data sets
	for (int i=0; i<100; ++i) {
		// dateset 1
		double t = i;
		double v = double(i)*i;
		QModelIndex index = model.index( i, 0 );
		model.setData( index, QVariant( t ) );
		index = model.index( i, 1 );
		model.setData( index, QVariant( v ) );

		// dateset 2
		t = i*2;
		v = double(i)*i-i;
		index = model.index( i, 2 );
		model.setData( index, QVariant( t ) );
		index = model.index( i, 3 );
		model.setData( index, QVariant( v ) );

		// dateset 3
		t = double(i)/2;
		v = double(i)*i+i;
		index = model.index( i, 4 );
		model.setData( index, QVariant( t ) );
		index = model.index( i, 5 );
		model.setData( index, QVariant( v ) );
	}

	// general chart layout
	KDChart::FrameAttributes fm = chart.frameAttributes();
	fm.setVisible(true);
	fm.setPen( QPen(Qt::black) );
	chart.setFrameAttributes(fm);
	chart.setGlobalLeading( 10, 0, 10, 10 );

	KDChart::BackgroundAttributes chart_bg;
	chart_bg.setBrush(Qt::white);
	chart_bg.setVisible(true);
	chart.setBackgroundAttributes(chart_bg);

	// coordinate plane setup
	KDChart::AbstractCoordinatePlane * plane1 = chart.coordinatePlane();
	plane1->setRubberBandZoomingEnabled(true);

	// create cartesian diagrams
	KDChart::Plotter * plotter = new KDChart::Plotter;
	plotter->setAntiAliasing(false);
	plotter->setModel( &model );
	plane1->replaceDiagram( plotter );

	// customize grids
	KDChart::CartesianCoordinatePlane * cp1 = static_cast<KDChart::CartesianCoordinatePlane *>(plane1);
	KDChart::GridAttributes gv = cp1->gridAttributes( Qt::Vertical );
	QPen gridPen(QColor(200,100,100));
	gridPen.setStyle(Qt::DashLine);
	gv.setGridPen(gridPen);
	gridPen.setStyle(Qt::DotLine);
	gridPen.setColor( QColor(255,155,155) );
	gv.setSubGridPen(gridPen);
	cp1->setGridAttributes( Qt::Vertical,  gv );

	// axis
	KDChart::CartesianAxis * xAxis = new KDChart::CartesianAxis( plotter );
	xAxis->setPosition( KDChart::CartesianAxis::Bottom );
	xAxis->setTitleText("X-Title");

	KDChart::TextAttributes att = xAxis->titleTextAttributes();
	QFont f = att.font();
	f.setBold(true);
	att.setFont(f);
	att.setAutoShrink(true);
	att.setFontSize( KDChart::Measure(16) );
	xAxis->setTitleTextAttributes(att);

	KDChart::CartesianAxis * y1Axis = new KDChart::CartesianAxis( plotter );
	y1Axis->setPosition( KDChart::CartesianAxis::Left );
	y1Axis->setTitleText("Y-Title");

	att = y1Axis->titleTextAttributes();
	f = att.font();
	f.setBold(true);
	att.setFont(f);
	att.setAutoShrink(true);
	att.setFontSize( KDChart::Measure(16) );
	y1Axis->setTitleTextAttributes(att);

	// add the axis to the plotter
	plotter->addAxis( xAxis );
	plotter->addAxis( y1Axis );

	// create legend
	KDChart::Legend * legend = new KDChart::Legend( plotter, &chart );
	chart.addLegend( legend );
	att = legend->textAttributes();
	f = att.font();
	f.setBold(false);
	att.setFont(f);
	att.setAutoShrink(true);
	att.setFontSize( KDChart::Measure(16) );
	legend->setTextAttributes(att);

	legend->setPosition( KDChart::Position::East );
	legend->setAlignment( Qt::AlignCenter );
	legend->setTitleText( "Curves" );
	att = legend->titleTextAttributes();
	f = att.font();
	f.setBold(true);
	att.setFont(f);
	att.setAutoShrink(true);
	att.setFontSize( KDChart::Measure(16) );
	legend->setTitleTextAttributes(att);

	KDChart::BackgroundAttributes legend_bg;
	legend_bg.setBrush(Qt::white);
	legend_bg.setVisible(true);
	legend->setBackgroundAttributes(legend_bg);

	// customize marker properties

	// Dataset 1 : green, MarkerRing, no line
	QColor SERIES_1_OUTLINE = QColor(0,128,0);
	KDChart::DataValueAttributes attr = plotter->dataValueAttributes(0);
	KDChart::MarkerAttributes mattr = attr.markerAttributes();
	mattr.setMarkerColor(SERIES_1_OUTLINE);
	mattr.setMarkerStyle(KDChart::MarkerAttributes::MarkerRing);
	mattr.setMarkerSize(QSizeF(6.0, 6.0));
	mattr.setVisible(true);
	attr.setMarkerAttributes(mattr);
	attr.setVisible(true);
	plotter->setDataValueAttributes(0, attr);
	plotter->setPen(0, Qt::NoPen);
	legend->setBrush(0, SERIES_1_OUTLINE );
	legend->setText(0, "Dataset 1");
	// this works so far

	// Dataset 2 : MarkerDiamond, (black outline, red inside), no line
	QColor SERIES_2_OUTLINE = Qt::black;
	QColor SERIES_2_INSIDE = QColor(255,100,100);
	attr = plotter->dataValueAttributes(1); // BUG? Which index to use here?
	mattr = attr.markerAttributes();
	mattr.setMarkerColor(SERIES_2_OUTLINE);
	mattr.setMarkerStyle(KDChart::MarkerAttributes::MarkerDiamond);
	mattr.setMarkerSize(QSizeF(8.0, 8.0));
	mattr.setVisible(true);
	attr.setMarkerAttributes(mattr);
	attr.setVisible(true);
	plotter->setDataValueAttributes(1, attr); // BUG? Which index to use here?
	plotter->setPen(1, Qt::NoPen);
	legend->setBrush(1, SERIES_2_INSIDE );
	legend->setPen(1, QPen(SERIES_2_OUTLINE));
	legend->setText(1, "Dataset 2");

	// Dataset 3 : MarkerCircle, (green outline, yellow inside), blue line
	QColor SERIES_3_OUTLINE = QColor(100,255,150);
	QColor SERIES_3_INSIDE = QColor("yellow");
	QColor SERIES_3_LINE = QColor("navy");
	attr = plotter->dataValueAttributes(2); // BUG? Which index to use here?
	mattr = attr.markerAttributes();
	mattr.setMarkerColor(SERIES_3_OUTLINE);
	mattr.setMarkerStyle(KDChart::MarkerAttributes::MarkerCircle);
	mattr.setMarkerSize(QSizeF(8.0, 8.0));
	mattr.setVisible(true);
	attr.setMarkerAttributes(mattr);
	attr.setVisible(true);
	plotter->setDataValueAttributes(2, attr); // BUG? Which index to use here?
	plotter->setPen(2, QPen(SERIES_3_LINE));
	legend->setBrush(2, SERIES_3_INSIDE );
	legend->setPen(2, QPen(SERIES_3_OUTLINE));
	legend->setText(2, "Dataset 3");

	chart.show();
	return a.exec();
}
