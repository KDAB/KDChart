#ifndef KDCHARTDATASETSELECTOR_H
#define KDCHARTDATASETSELECTOR_H

#include <QFrame>

#include "KDChartDatasetProxyModel.h"

#ifdef __GNUC__
#warning For this to become useful, it has to be connected to the proxy model it configures!
#endif

namespace Ui {
    class DatasetSelector;
}

namespace KDChart {

    class KDCHART_EXPORT DatasetSelectorWidget : public QFrame
    {
        Q_OBJECT

    public:
        explicit DatasetSelectorWidget ( QWidget* parent = 0 );

    public slots:
        void setSourceRowCount ( const int& rowCount );
        void setSourceColumnCount ( const int& columnCount );

    signals:
        void configureDatasetProxyModel (
            const DatasetDescriptionVector& rowConfig,
            const DatasetDescriptionVector& columnConfig );

        void mappingDisabled ();

    private slots:
        void on_sbStartColumn_valueChanged ( int );
        void on_sbStartRow_valueChanged ( int );
        void on_sbColumnCount_valueChanged( int );
        void on_sbRowCount_valueChanged( int );
        void on_cbReverseRows_stateChanged ( int );
        void on_cbReverseColumns_stateChanged ( int );
        void on_groupBox_toggled ( bool );


    private:
        void resetDisplayValues ();
        void calculateMapping();

        Ui::DatasetSelector* mUi;
        int mSourceRowCount;
        int mSourceColumnCount;
    };

}

#endif

