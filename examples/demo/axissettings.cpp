/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "axissettings.h"

#include "ui_axissettings.h"

#include <QtCore/QHash>
#include <QtCore/QVector>

#include <QComboBox>

#include <KDChart/KDChartAbstractCartesianDiagram>
#include <KDChart/KDChartCartesianAxis>
#include <KDChart/KDChartCartesianCoordinatePlane>
#include <KDChart/KDChartChart>
#include <KDChart/KDChartRulerAttributes>

#include <QDebug>

using namespace KDChart;

class AxisSettings::Private : public QObject
{
    Q_OBJECT
public:
    Private(Chart *chart, AxisSettings *q);
    void init();
    CartesianAxis *m_currentAxis;
    QHash<CartesianCoordinatePlane *, QVector<CartesianAxis *>> m_axisCache;
    Chart *m_chart;
    Ui::AxisSettings ui;
    AxisSettings *qq;
public Q_SLOTS:
    void currentIndexChanged(int index);
    void setVisible(bool value);
    void setShowMajorTickMarks(bool value);
    void setShowMinorTickMarks(bool value);

private:
    void updateUiForCurrentAxis();
};

AxisSettings::Private::Private(Chart *chart, AxisSettings *q)
    : m_chart(chart)
    , qq(q)
{
}

void AxisSettings::Private::init()
{
    ui.axisSelection->addItem(QIcon(), tr("Left Y-Axis"), CartesianAxis::Left);
    ui.axisSelection->addItem(QIcon(), tr("Right Y-Axis"), CartesianAxis::Right);
    ui.axisSelection->addItem(QIcon(), tr("Bottom X-Axis"), CartesianAxis::Bottom);
    ui.axisSelection->addItem(QIcon(), tr("Top X-Axis"), CartesianAxis::Top);
    connect(ui.axisSelection, SIGNAL(currentIndexChanged(int)), this, SLOT(currentIndexChanged(int)));
    connect(ui.axisVisibility, SIGNAL(toggled(bool)), this, SLOT(setVisible(bool)));
    connect(ui.majorTicks, SIGNAL(toggled(bool)), this, SLOT(setShowMajorTickMarks(bool)));
    connect(ui.minorTicks, SIGNAL(toggled(bool)), this, SLOT(setShowMinorTickMarks(bool)));
    ui.axisSelection->setCurrentIndex(0);
}

void AxisSettings::Private::currentIndexChanged(int index)
{
    m_currentAxis = nullptr;
    const CartesianAxis::Position pos = ( CartesianAxis::Position )ui.axisSelection->itemData(index).toInt();
    auto *plane = qobject_cast<CartesianCoordinatePlane *>(m_chart->coordinatePlane());
    auto *diag = qobject_cast<AbstractCartesianDiagram *>(m_chart->coordinatePlane()->diagram());
    if (plane && diag) {
        QVector<CartesianAxis *> axes = diag->axes().toVector();
        Q_FOREACH (CartesianAxis *axis, axes) {
            if (axis->position() == pos) {
                m_currentAxis = axis;
                break;
            }
        }
    }
    updateUiForCurrentAxis();
}

void AxisSettings::Private::updateUiForCurrentAxis()
{
    const bool prevSignalsBlocked = ui.axisVisibility->blockSignals(true);
    ui.axisVisibility->setChecked(m_currentAxis);
    ui.axisVisibility->blockSignals(prevSignalsBlocked);

    ui.majorTicks->setEnabled(m_currentAxis);
    ui.minorTicks->setEnabled(m_currentAxis);
    if (m_currentAxis) {
        const RulerAttributes rulerAttr = m_currentAxis->rulerAttributes();
        ui.majorTicks->setChecked(rulerAttr.showMajorTickMarks());
        ui.minorTicks->setChecked(rulerAttr.showMinorTickMarks());
    } else {
        ui.majorTicks->setChecked(false);
        ui.minorTicks->setChecked(false);
    }
}

void AxisSettings::Private::setVisible(bool value)
{
    const CartesianAxis::Position pos = ( CartesianAxis::Position )ui.axisSelection->itemData(ui.axisSelection->currentIndex()).toInt();
    auto *plane = qobject_cast<CartesianCoordinatePlane *>(m_chart->coordinatePlane());
    auto *diag = qobject_cast<AbstractCartesianDiagram *>(m_chart->coordinatePlane()->diagram());
    if (plane && diag) {
        QVector<CartesianAxis *> axes = m_axisCache[plane];
        bool foundAxis = false;
        Q_FOREACH (CartesianAxis *axis, axes) {
            if (axis->position() == pos) {
                foundAxis = true;
                if (!value) {
                    diag->takeAxis(axis);
                    m_currentAxis = nullptr;
                } else {
                    diag->addAxis(axis);
                    m_currentAxis = axis;
                }
                break;
            }
        }
        if (!foundAxis) {
            Q_ASSERT(value);
            auto *axis = new CartesianAxis(diag);
            axis->setPosition(pos);
            diag->addAxis(axis);
            m_axisCache[plane].append(axis);
            m_currentAxis = axis;
        }
        updateUiForCurrentAxis();
        m_chart->update();
    }
}

void AxisSettings::Private::setShowMajorTickMarks(bool value)
{
    if (m_currentAxis) {
        RulerAttributes attr(m_currentAxis->rulerAttributes());
        attr.setShowMajorTickMarks(value);
        m_currentAxis->setRulerAttributes(attr);
        m_chart->update();
    }
}

void AxisSettings::Private::setShowMinorTickMarks(bool value)
{
    if (m_currentAxis) {
        RulerAttributes attr(m_currentAxis->rulerAttributes());
        attr.setShowMinorTickMarks(value);
        m_currentAxis->setRulerAttributes(attr);
        m_chart->update();
    }
}

AxisSettings::AxisSettings(Chart *chart, QWidget *parent)
    : QWidget(parent)
    , d(new Private(chart, this))
{
    d->ui.setupUi(this);
    d->init();
}

AxisSettings::~AxisSettings()
{
    delete d;
}

void AxisSettings::diagramTypeChanged()
{
    auto *plane = qobject_cast<CartesianCoordinatePlane *>(d->m_chart->coordinatePlane());
    setEnabled(plane);
    if (plane)
        d->currentIndexChanged(d->ui.axisSelection->currentIndex());
}

#include "axissettings.moc"
