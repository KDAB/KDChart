//------------------------------------------------------------------------
// COPYRIGHT (C) 2010
// THIS SOFTWARE IS THE PROPERTY OF ROCHE DIAGNOSTICS GRAZ GMBH, AUSTRIA:
// ALL RIGHTS RESERVED NO PART OF THIS SOFTWARE MAY BE USED OR COPIED IN
// ANY WAY WITHOUT THE PRIOR WRITTEN PERMISSION OF ROCHE DIAGNOSTICS GRAZ
// GMBH, AUSTRIA
//------------------------------------------------------------------------

/*!
 * \file %FILENAME%
 * \brief $MODULE$ Class
 * \author $AUTHOR$ <$EMAIL$>
 */

#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui
{
    class Dialog;
}

namespace KDChart
{
    class Chart;
    class Plotter;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

signals:
    void start();
    void stop();

private slots:
    void startSimulation();
    void stopSimulation();
    void onStopped();

protected:
    //! Widget initialization
    void setupWidget();
    //! Text initialization
    void retranslateUi();

private:
    Ui::Dialog *_ui;
    KDChart::Chart * _pChart;
    KDChart::Plotter* _pLineDiagram;
    void setupModel();
};

#endif // DIALOG_H
