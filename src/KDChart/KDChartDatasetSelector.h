/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2021 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDAB-KDChart OR LicenseRef-KDAB-KDChart-US
**
** Licensees holding valid commercial KD Chart licenses may use this file in
** accordance with the KD Chart Commercial License Agreement provided with
** the Software.
**
** Contact info@kdab.com if any conditions of this licensing are not
** clear to you.
**
****************************************************************************/

#ifndef KDCHARTDATASETSELECTOR_H
#define KDCHARTDATASETSELECTOR_H

#include <QFrame>

#include "KDChartDatasetProxyModel.h"

/**
 * \cond PRIVATE_API_DOCU
 *
 * ( This class is used internally by DatasetSelectorWidget. )
 */
QT_BEGIN_NAMESPACE
namespace Ui {
class DatasetSelector;
}
QT_END_NAMESPACE
/**
 * \endcond
 */

namespace KDChart {

class KDCHART_EXPORT DatasetSelectorWidget : public QFrame
{
    Q_OBJECT

public:
    explicit DatasetSelectorWidget(QWidget *parent = nullptr);
    ~DatasetSelectorWidget() override;

public Q_SLOTS:
    void setSourceRowCount(const int &rowCount);
    void setSourceColumnCount(const int &columnCount);

Q_SIGNALS:
    void configureDatasetProxyModel(
        const DatasetDescriptionVector &rowConfig,
        const DatasetDescriptionVector &columnConfig);

    void mappingDisabled();

private Q_SLOTS:
    void on_sbStartColumn_valueChanged(int);
    void on_sbStartRow_valueChanged(int);
    void on_sbColumnCount_valueChanged(int);
    void on_sbRowCount_valueChanged(int);
    void on_cbReverseRows_stateChanged(int);
    void on_cbReverseColumns_stateChanged(int);
    void on_groupBox_toggled(bool);

private:
    void resetDisplayValues();
    void calculateMapping();

    Ui::DatasetSelector *mUi;
    int mSourceRowCount;
    int mSourceColumnCount;
};

}

#endif
