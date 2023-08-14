/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "datavaluesettings.h"
#include "ui_datavaluesettings.h"

#include <KDChart/KDChartAbstractCoordinatePlane>
#include <KDChart/KDChartAbstractDiagram>
#include <KDChart/KDChartBarDiagram>
#include <KDChart/KDChartChart>
#include <KDChart/KDChartPosition>
#include <KDChart/KDChartRelativePosition>
#include <KDChart/KDChartTextAttributes>
#include <KDChartDataValueAttributes>

#include <QDebug>
#include <QPainter>

using namespace KDChart;

class DataValueSettings::Private : public QObject
{
    Q_OBJECT
public:
    Private(KDChart::Chart *chart, DataValueSettings *q);
    DataValueSettings *qq;
    void init();
public Q_SLOTS:
    void on_scopeOneBarRB_toggled(bool checked);
    void on_scopeBarDatasetSB_valueChanged(int i);
    void on_scopeBarItemSB_valueChanged(int i);
    void on_scopeDatasetRB_toggled(bool checked);
    void on_scopeDatasetSB_valueChanged(int i);
    void on_scopeCommonRB_toggled(bool checked);

    void on_paintValuesCB_toggled(bool checked);

    void on_fontCombo_currentIndexChanged(int index);
    void on_relativeSizeSB_valueChanged(int i);
    void on_minimumSizeSB_valueChanged(int i);
    void on_rotationSB_valueChanged(int i);

    void on_posPosCombo_currentIndexChanged(int index);
    void on_posAlignCombo_currentIndexChanged(int index);
    void on_posPadHoriSB_valueChanged(int i);
    void on_posPadVertSB_valueChanged(int i);
    void on_negPosCombo_currentIndexChanged(int index);
    void on_negAlignCombo_currentIndexChanged(int index);
    void on_negPadHoriSB_valueChanged(int i);
    void on_negPadVertSB_valueChanged(int i);

    void on_labelLE_textEdited(const QString &text);
    void on_prefixLE_textEdited(const QString &text);
    void on_suffixLE_textEdited(const QString &text);

public:
    const QModelIndex currentIndex() const;
    const KDChart::DataValueAttributes attributes() const;
    const char *positionToScreenName(const KDChart::Position &pos) const;
    const Qt::Alignment alignmentFromScreeName(const QString &name) const;
    const QString alignmentToScreenName(const Qt::Alignment &align) const;
    void populateWidgets();
    void setAttributes(const KDChart::DataValueAttributes &da);
    void connectWidgets();

    KDChart::Chart *m_chart;
    Ui::DataValueSettings *ui;
};

DataValueSettings::Private::Private(KDChart::Chart *chart, DataValueSettings *q)
    : qq(q)
    , m_chart(chart)
    , ui(new Ui::DataValueSettings)
{
}

void DataValueSettings::Private::init()
{
    ui->setupUi(qq);
    ui->scopeCommonRB->setFocus(Qt::OtherFocusReason);
    ui->scopeCommonRB->setChecked(true);
    ui->paintValuesCB->setChecked(true);
    populateWidgets();
    connectWidgets();
}

void DataValueSettings::Private::connectWidgets()
{
    connect(ui->scopeBarDatasetSB, &QSpinBox::valueChanged, this, &Private::on_scopeBarDatasetSB_valueChanged);
    connect(ui->scopeBarItemSB, &QSpinBox::valueChanged, this, &Private::on_scopeBarItemSB_valueChanged);
    connect(ui->scopeDatasetSB, &QSpinBox::valueChanged, this, &Private::on_scopeDatasetSB_valueChanged);
    connect(ui->scopeOneBarRB, &QRadioButton::toggled, this, &Private::on_scopeOneBarRB_toggled);
    connect(ui->scopeDatasetRB, &QRadioButton::toggled, this, &Private::on_scopeDatasetRB_toggled);
    connect(ui->scopeCommonRB, &QRadioButton::toggled, this, &Private::on_scopeCommonRB_toggled);
    connect(ui->paintValuesCB, &QCheckBox::toggled, this, &Private::on_paintValuesCB_toggled);
    connect(ui->fontCombo, &QFontComboBox::currentIndexChanged, this, &Private::on_fontCombo_currentIndexChanged);
    connect(ui->relativeSizeSB, &QSpinBox::valueChanged, this, &Private::on_relativeSizeSB_valueChanged);
    connect(ui->minimumSizeSB, &QSpinBox::valueChanged, this, &Private::on_minimumSizeSB_valueChanged);
    connect(ui->rotationSB, &QSpinBox::valueChanged, this, &Private::on_rotationSB_valueChanged);
    connect(ui->posPosCombo, &QComboBox::currentIndexChanged, this, &Private::on_posPosCombo_currentIndexChanged);
    connect(ui->posAlignCombo, &QComboBox::currentIndexChanged, this, &Private::on_posAlignCombo_currentIndexChanged);
    connect(ui->posPadHoriSB, &QSpinBox::valueChanged, this, &Private::on_posPadHoriSB_valueChanged);
    connect(ui->posPadVertSB, &QSpinBox::valueChanged, this, &Private::on_posPadVertSB_valueChanged);
    connect(ui->negPosCombo, &QComboBox::currentIndexChanged, this, &Private::on_negPosCombo_currentIndexChanged);
    connect(ui->negAlignCombo, &QComboBox::currentIndexChanged, this, &Private::on_negAlignCombo_currentIndexChanged);
    connect(ui->negPadHoriSB, &QSpinBox::valueChanged, this, &Private::on_negPadHoriSB_valueChanged);
    connect(ui->negPadVertSB, &QSpinBox::valueChanged, this, &Private::on_negPadVertSB_valueChanged);
    connect(ui->labelLE, &QLineEdit::textEdited, this, &Private::on_labelLE_textEdited);
    connect(ui->prefixLE, &QLineEdit::textEdited, this, &Private::on_prefixLE_textEdited);
    connect(ui->suffixLE, &QLineEdit::textEdited, this, &Private::on_suffixLE_textEdited);
}

DataValueSettings::DataValueSettings(KDChart::Chart *chart, QWidget *parent)
    : QWidget(parent)
    , d(new Private(chart, this))
{
    d->init();
}

DataValueSettings::~DataValueSettings()
{
    delete d;
}

void DataValueSettings::refresh()
{
    d->populateWidgets();
}

void DataValueSettings::Private::on_scopeOneBarRB_toggled(bool checked)
{
    if (checked) {
        ui->scopeBarDatasetSB->setDisabled(false);
        ui->scopeBarItemSB->setDisabled(false);
        ui->scopeDatasetSB->setDisabled(true);
        populateWidgets();
    }
}

void DataValueSettings::Private::on_scopeBarDatasetSB_valueChanged(int i)
{
    Q_UNUSED(i)
    populateWidgets();
}

void DataValueSettings::Private::on_scopeBarItemSB_valueChanged(int i)
{
    Q_UNUSED(i)
    populateWidgets();
}

void DataValueSettings::Private::on_scopeDatasetRB_toggled(bool checked)
{
    if (checked) {
        ui->scopeBarDatasetSB->setDisabled(true);
        ui->scopeBarItemSB->setDisabled(true);
        ui->scopeDatasetSB->setDisabled(false);
        populateWidgets();
    }
}

void DataValueSettings::Private::on_scopeDatasetSB_valueChanged(int i)
{
    Q_UNUSED(i)
    populateWidgets();
}

void DataValueSettings::Private::on_scopeCommonRB_toggled(bool checked)
{
    if (checked) {
        ui->scopeBarDatasetSB->setDisabled(true);
        ui->scopeBarItemSB->setDisabled(true);
        ui->scopeDatasetSB->setDisabled(true);
        populateWidgets();
    }
}

void DataValueSettings::Private::on_paintValuesCB_toggled(bool checked)
{
    DataValueAttributes da(attributes());
    da.setVisible(checked);
    setAttributes(da);

    m_chart->update();
}

void DataValueSettings::Private::on_fontCombo_currentIndexChanged(int index)
{
    DataValueAttributes da(attributes());
    TextAttributes ta(da.textAttributes());
    const QString text = ui->fontCombo->itemText(index);
    QFont font(text);
    ta.setFont(font);
    da.setTextAttributes(ta);
    setAttributes(da);

    m_chart->update();
}

void DataValueSettings::Private::on_relativeSizeSB_valueChanged(int i)
{
    DataValueAttributes da(attributes());
    TextAttributes ta(da.textAttributes());
    Measure fs(ta.fontSize());
    fs.setValue(i);
    ta.setFontSize(i);
    da.setTextAttributes(ta);
    setAttributes(da);

    m_chart->update();
}

void DataValueSettings::Private::on_minimumSizeSB_valueChanged(int i)
{
    DataValueAttributes da(attributes());
    TextAttributes ta(da.textAttributes());
    Measure fs(ta.fontSize());
    fs.setValue(i);
    ta.setMinimalFontSize(i);
    da.setTextAttributes(ta);
    setAttributes(da);

    m_chart->update();
}

void DataValueSettings::Private::on_rotationSB_valueChanged(int i)
{
    DataValueAttributes da(attributes());
    TextAttributes ta(da.textAttributes());
    ta.setRotation(i);
    da.setTextAttributes(ta);
    setAttributes(da);

    m_chart->update();
}

void DataValueSettings::Private::on_posPosCombo_currentIndexChanged(int index)
{
    DataValueAttributes da(attributes());
    RelativePosition relPos(da.positivePosition());
    const QString text = ui->posPosCombo->itemText(index);
    relPos.setReferencePosition(Position::fromName(qPrintable(text)));
    da.setPositivePosition(relPos);
    setAttributes(da);

    m_chart->update();
}

void DataValueSettings::Private::on_posAlignCombo_currentIndexChanged(int index)
{
    DataValueAttributes da(attributes());
    RelativePosition relPos(da.positivePosition());
    const QString text = ui->posAlignCombo->itemText(index);
    relPos.setAlignment(alignmentFromScreeName(text));
    da.setPositivePosition(relPos);
    setAttributes(da);

    m_chart->update();
}

void DataValueSettings::Private::on_posPadHoriSB_valueChanged(int i)
{
    DataValueAttributes da(attributes());
    RelativePosition relPos(da.positivePosition());
    Measure pad(relPos.horizontalPadding());
    pad.setValue(i);
    relPos.setHorizontalPadding(pad);
    da.setPositivePosition(relPos);
    setAttributes(da);

    m_chart->update();
}

void DataValueSettings::Private::on_posPadVertSB_valueChanged(int i)
{
    DataValueAttributes da(attributes());
    RelativePosition relPos(da.positivePosition());
    Measure pad(relPos.verticalPadding());
    pad.setValue(i);
    relPos.setVerticalPadding(pad);
    da.setPositivePosition(relPos);
    setAttributes(da);

    m_chart->update();
}

void DataValueSettings::Private::on_negPosCombo_currentIndexChanged(int index)
{
    DataValueAttributes da(attributes());
    RelativePosition relPos(da.negativePosition());
    const QString text = ui->negPosCombo->itemText(index);
    relPos.setReferencePosition(Position::fromName(qPrintable(text)));
    da.setNegativePosition(relPos);
    setAttributes(da);

    m_chart->update();
}

void DataValueSettings::Private::on_negAlignCombo_currentIndexChanged(int index)
{
    DataValueAttributes da(attributes());
    RelativePosition relPos(da.negativePosition());
    const QString text = ui->negAlignCombo->itemText(index);
    relPos.setAlignment(alignmentFromScreeName(text));
    da.setNegativePosition(relPos);
    setAttributes(da);

    m_chart->update();
}

void DataValueSettings::Private::on_negPadHoriSB_valueChanged(int i)
{
    DataValueAttributes da(attributes());
    RelativePosition relPos(da.negativePosition());
    Measure pad(relPos.horizontalPadding());
    pad.setValue(i);
    relPos.setHorizontalPadding(pad);
    da.setNegativePosition(relPos);
    setAttributes(da);

    m_chart->update();
}

void DataValueSettings::Private::on_negPadVertSB_valueChanged(int i)
{
    DataValueAttributes da(attributes());
    RelativePosition relPos(da.negativePosition());
    Measure pad(relPos.verticalPadding());
    pad.setValue(i);
    relPos.setVerticalPadding(pad);
    da.setNegativePosition(relPos);
    setAttributes(da);

    m_chart->update();
}

void DataValueSettings::Private::on_labelLE_textEdited(const QString &text)
{
    DataValueAttributes da(attributes());
    da.setDataLabel(text.isEmpty() ? QString() : text);
    setAttributes(da);

    m_chart->update();
}

void DataValueSettings::Private::on_prefixLE_textEdited(const QString &text)
{
    DataValueAttributes da(attributes());
    da.setPrefix(text.isEmpty() ? QString() : text);
    setAttributes(da);

    m_chart->update();
}

void DataValueSettings::Private::on_suffixLE_textEdited(const QString &text)
{
    DataValueAttributes da(attributes());
    da.setSuffix(text.isEmpty() ? QString() : text);
    setAttributes(da);

    m_chart->update();
}

const QModelIndex DataValueSettings::Private::currentIndex() const
{
    const int dataset = ui->scopeBarDatasetSB->value();
    const int item = ui->scopeBarItemSB->value();
    Q_ASSERT(m_chart);
    Q_ASSERT(m_chart->coordinatePlane());
    AbstractDiagram *diag = m_chart->coordinatePlane()->diagram();
    return diag->model()->index(item, dataset, QModelIndex());
}

const KDChart::DataValueAttributes DataValueSettings::Private::attributes() const
{
    Q_ASSERT(m_chart);
    Q_ASSERT(m_chart->coordinatePlane());
    AbstractDiagram *diag = m_chart->coordinatePlane()->diagram();
    DataValueAttributes attr = diag->dataValueAttributes();
    if (ui->scopeOneBarRB->isChecked()) {
        attr = diag->dataValueAttributes(currentIndex());
    } else if (ui->scopeDatasetRB->isChecked()) {
        attr = diag->dataValueAttributes(ui->scopeDatasetSB->value());
    }
    attr.setShowRepetitiveDataLabels(true);
    return attr;
}

void DataValueSettings::Private::setAttributes(const KDChart::DataValueAttributes &da)
{
    Q_ASSERT(m_chart);
    Q_ASSERT(m_chart->coordinatePlane());
    AbstractDiagram *diag = m_chart->coordinatePlane()->diagram();
    if (ui->scopeOneBarRB->isChecked())
        diag->setDataValueAttributes(currentIndex(), da);
    else if (ui->scopeDatasetRB->isChecked())
        diag->setDataValueAttributes(ui->scopeDatasetSB->value(), da);
    else
        diag->setDataValueAttributes(da);
}

// just a convenience method:
// In the combo box we have the text "( Default Value )" instead of "Unknown Position"
// because by setting a position to unknown we get KD Chart to use the
// diagram-specific default positions.
const char *DataValueSettings::Private::positionToScreenName(const Position &pos) const
{
    static const char *defaultPositionName = "( Default Value )";
    if (pos.isUnknown())
        return defaultPositionName;
    return pos.name();
}

const Qt::Alignment DataValueSettings::Private::alignmentFromScreeName(const QString &name) const
{
    if (name == "Center")
        return Qt::AlignCenter;
    if (name == "BottomLeft")
        return Qt::AlignLeft | Qt::AlignBottom;
    if (name == "Bottom")
        return Qt::AlignHCenter | Qt::AlignBottom;
    if (name == "BottomRight")
        return Qt::AlignRight | Qt::AlignBottom;
    if (name == "Right")
        return Qt::AlignRight | Qt::AlignVCenter;
    if (name == "TopRight")
        return Qt::AlignRight | Qt::AlignTop;
    if (name == "Top")
        return Qt::AlignHCenter | Qt::AlignTop;
    if (name == "TopLeft")
        return Qt::AlignLeft | Qt::AlignTop;
    if (name == "Left")
        return Qt::AlignLeft | Qt::AlignVCenter;
    return Qt::AlignCenter;
}

const QString DataValueSettings::Private::alignmentToScreenName(const Qt::Alignment &align) const
{
    if (align == Qt::AlignCenter)
        return "Center";
    if (align == (Qt::AlignLeft | Qt::AlignBottom))
        return "BottomLeft";
    if (align == (Qt::AlignHCenter | Qt::AlignBottom))
        return "Bottom";
    if (align == (Qt::AlignRight | Qt::AlignBottom))
        return "BottomRight";
    if (align == (Qt::AlignRight | Qt::AlignVCenter))
        return "Right";
    if (align == (Qt::AlignRight | Qt::AlignTop))
        return "TopRight";
    if (align == (Qt::AlignHCenter | Qt::AlignTop))
        return "Top";
    if (align == (Qt::AlignLeft | Qt::AlignTop))
        return "TopLeft";
    if (align == (Qt::AlignLeft | Qt::AlignVCenter))
        return "Left";
    return "Center";
}

void DataValueSettings::Private::populateWidgets()
{
    const DataValueAttributes da(attributes());
    const TextAttributes ta(da.textAttributes());
    const RelativePosition posPos(da.positivePosition());
    const RelativePosition negPos(da.negativePosition());

    ui->paintValuesCB->setChecked(da.isVisible() && ta.isVisible());
    ui->fontCombo->setCurrentFont(ta.font());
    ui->relativeSizeSB->setValue(static_cast<int>(ta.fontSize().value()));
    ui->minimumSizeSB->setValue(static_cast<int>(ta.minimalFontSize().value()));
    ui->rotationSB->setValue(static_cast<int>(ta.rotation()));

    ui->posPosCombo->setCurrentIndex(ui->posPosCombo->findText(
        positionToScreenName(posPos.referencePosition())));
    ui->posAlignCombo->setCurrentIndex(ui->posAlignCombo->findText(
        alignmentToScreenName(posPos.alignment())));
    ui->posPadHoriSB->setValue(static_cast<int>(posPos.horizontalPadding().value()));
    ui->posPadVertSB->setValue(static_cast<int>(posPos.verticalPadding().value()));

    ui->negPosCombo->setCurrentIndex(ui->negPosCombo->findText(positionToScreenName(
        negPos.referencePosition())));
    ui->negAlignCombo->setCurrentIndex(ui->negAlignCombo->findText(
        alignmentToScreenName(negPos.alignment())));
    ui->negPadHoriSB->setValue(static_cast<int>(negPos.horizontalPadding().value()));
    ui->negPadVertSB->setValue(static_cast<int>(negPos.verticalPadding().value()));

    ui->labelLE->setText(da.dataLabel());
    ui->prefixLE->setText(da.prefix());
    ui->suffixLE->setText(da.suffix());
}

#include "datavaluesettings.moc"
