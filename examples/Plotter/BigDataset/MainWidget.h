#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "Model.h"

#include "ui_Controls.h"

#include <QWidget>



namespace KDChart {
class Plotter;
}

class Model;
class QPushButton;
class QRadioButton;

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    MainWidget();

private slots:
    void functionToggled( bool checked );
    void addPointsButtonClicked();

private:
    QWidget* m_controlsContainer;
    Ui::Controls m_controls;
    QVector< QRadioButton* > m_functionSelector;
    QVector< QPushButton* > m_addPointsButtons;

    KDChart::Plotter* m_plotter;
    Model m_model;
};

#endif 
