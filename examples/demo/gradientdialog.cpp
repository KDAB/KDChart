/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "gradientdialog.h"
#include "ui_gradientdialog.h"

#include "colorslider.h"

#include <QHBoxLayout>
#include <QSpinBox>
#include <QVBoxLayout>

#include <QLabel>

class GradientDialog::Private : public QObject
{
    Q_OBJECT
public:
    Private(GradientDialog *qq);
    ~Private() override;
    void init();
    ColorSlider *redSlider;
    ColorSlider *greenSlider;
    ColorSlider *blueSlider;
    QVector<QGradientStop> m_gradient;
    int m_currStopNr;
    Ui::GradientDialog *ui;
    GradientDialog *q;
public Q_SLOTS:
    void changedIndex(int index);
    void changeStopPosition(double value);
    void insertItem();
    void deleteItem();
    void resetColors();
    void updateGradientDisplay();
};

GradientDialog::Private::Private(GradientDialog *qq)
    : m_currStopNr(2)
    , ui(new Ui::GradientDialog)
    , q(qq)
{
    m_gradient << qMakePair(( qreal )0.0, QColor(Qt::red));
    m_gradient << qMakePair(( qreal )1.0, QColor(Qt::blue));
}

GradientDialog::Private::~Private()
{
    delete ui;
}

void GradientDialog::Private::changeStopPosition(double value)
{
    m_gradient[ui->stopSelector->currentIndex()].first = value;
}

void GradientDialog::Private::changedIndex(int index)
{
    if (index >= 0) {
        ui->stopPosition->setValue(m_gradient[index].first);
        QColor stopColor = m_gradient[index].second;
        redSlider->setValue(stopColor.red());
        greenSlider->setValue(stopColor.green());
        blueSlider->setValue(stopColor.blue());
    }
}

void GradientDialog::Private::insertItem()
{
    QGradientStop newStop;
    newStop.first = 0.5;
    newStop.second = Qt::white;
    const int index = ui->stopSelector->currentIndex() + 1;
    ui->stopSelector->setCurrentIndex(-1);
    m_gradient.insert(index, newStop);

    QStringList newItems;
    newItems << tr("stop %1").arg(m_currStopNr);
    ui->stopSelector->insertItems(index, newItems);

    ui->stopSelector->setCurrentIndex(index);

    updateGradientDisplay();
}

void GradientDialog::Private::deleteItem()
{
}

void GradientDialog::Private::updateGradientDisplay()
{
    QLinearGradient gradient;
    gradient.setStart(0, 0);
    gradient.setStart(1, 0);
    gradient.setCoordinateMode(QGradient::ObjectBoundingMode);
    Q_FOREACH (const QGradientStop &stop, m_gradient)
        gradient.setColorAt(stop.first, stop.second);
    QPalette palette = ui->gradientDisplay->palette();
    palette.setBrush(QPalette::Window, gradient);
    ui->gradientDisplay->setPalette(palette);
}

void GradientDialog::Private::init()
{
    ui->setupUi(q);
    QStringList list;
    list << tr("stop1") << tr("stop2");
    ui->stopSelector->addItems(list);
    auto *redLayout = new QHBoxLayout;
    dynamic_cast<QVBoxLayout *>(ui->gradientStopBox->layout())->addLayout(redLayout);
    auto *redLabel = new QLabel("R");
    QFont redFont(redLabel->font());
    redFont.setUnderline(true);
    redLabel->setFont(redFont);
    redLayout->addWidget(redLabel);
    redSlider = new ColorSlider(q);
    redSlider->setStartColor(Qt::black);
    redSlider->setEndColor(Qt::red);
    auto *redSpin = new QSpinBox(q);
    redSpin->setMinimum(0);
    redSpin->setMaximum(255);
    redSpin->setAccelerated(true);
    redSpin->setValue(redSlider->value());
    connect(redSpin, QOverload<int>::of(&QSpinBox::valueChanged), redSlider, &ColorSlider::setValue);
    connect(redSlider, &ColorSlider::valueChanged, redSpin, &QSpinBox::setValue);
    redLayout->addWidget(redSlider);
    redLayout->addWidget(redSpin);

    auto *greenLayout = new QHBoxLayout;
    dynamic_cast<QVBoxLayout *>(ui->gradientStopBox->layout())->addLayout(greenLayout);
    auto *greenLabel = new QLabel("G");
    QFont greenFont(greenLabel->font());
    greenFont.setUnderline(true);
    greenLabel->setFont(greenFont);
    greenLayout->addWidget(greenLabel);
    greenSlider = new ColorSlider(q);
    greenSlider->setStartColor(Qt::black);
    greenSlider->setEndColor(Qt::green);
    auto *greenSpin = new QSpinBox(q);
    greenSpin->setMinimum(0);
    greenSpin->setMaximum(255);
    greenSpin->setAccelerated(true);
    greenSpin->setValue(greenSlider->value());
    connect(greenSpin, QOverload<int>::of(&QSpinBox::valueChanged), greenSlider, &ColorSlider::setValue);
    connect(greenSlider, &ColorSlider::valueChanged, greenSpin, &QSpinBox::setValue);
    greenLayout->addWidget(greenSlider);
    greenLayout->addWidget(greenSpin);

    auto *blueLayout = new QHBoxLayout;
    dynamic_cast<QVBoxLayout *>(ui->gradientStopBox->layout())->addLayout(blueLayout);
    auto *blueLabel = new QLabel("B");
    QFont blueFont(blueLabel->font());
    blueFont.setUnderline(true);
    blueLabel->setFont(blueFont);
    blueLayout->addWidget(blueLabel);
    blueSlider = new ColorSlider(q);
    blueSlider->setStartColor(Qt::black);
    blueSlider->setEndColor(Qt::blue);
    auto *blueSpin = new QSpinBox(q);
    blueSpin->setMinimum(0);
    blueSpin->setMaximum(255);
    blueSpin->setAccelerated(true);
    blueSpin->setValue(blueSlider->value());
    connect(blueSpin, QOverload<int>::of(&QSpinBox::valueChanged), blueSlider, &ColorSlider::setValue);
    connect(blueSlider, &ColorSlider::valueChanged, blueSpin, &QSpinBox::setValue);
    blueLayout->addWidget(blueSlider);
    blueLayout->addWidget(blueSpin);

    updateGradientDisplay();

    connect(redSlider, &ColorSlider::valueChanged, this, &Private::resetColors);
    connect(greenSlider, &ColorSlider::valueChanged, this, &Private::resetColors);
    connect(blueSlider, &ColorSlider::valueChanged, this, &Private::resetColors);

    connect(ui->newStop, &QPushButton::clicked, this, &Private::insertItem);
    connect(ui->deleteStop, &QPushButton::clicked, this, &Private::deleteItem);
    connect(ui->stopSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Private::changedIndex);

    connect(ui->stopPosition, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &Private::changeStopPosition);
}

GradientDialog::GradientDialog(QWidget *parent)
    : QDialog(parent)
    , d(new Private(this))
{
    d->init();
}

GradientDialog::~GradientDialog()
{
    delete d;
}

void GradientDialog::Private::resetColors()
{
    const int red = redSlider->value();
    const int green = greenSlider->value();
    const int blue = blueSlider->value();
    QColor redSliderStartColor(0, green, blue);
    QColor redSliderStopColor(255, green, blue);
    redSlider->setStartColor(redSliderStartColor);
    redSlider->setEndColor(redSliderStopColor);
    m_gradient[ui->stopSelector->currentIndex()].second.setRed(red);

    QColor greenSliderStartColor(red, 0, blue);
    QColor greenSliderStopColor(red, 255, blue);
    greenSlider->setStartColor(greenSliderStartColor);
    greenSlider->setEndColor(greenSliderStopColor);
    m_gradient[ui->stopSelector->currentIndex()].second.setGreen(green);

    QColor blueSliderStartColor(red, green, 0);
    QColor blueSliderStopColor(red, green, 255);
    blueSlider->setStartColor(blueSliderStartColor);
    blueSlider->setEndColor(blueSliderStopColor);
    m_gradient[ui->stopSelector->currentIndex()].second.setBlue(blue);

    updateGradientDisplay();
}

QGradient GradientDialog::gradient() const
{
    QLinearGradient gradient;
    gradient.setStart(0, 0);
    gradient.setStart(1, 0);
    gradient.setCoordinateMode(QGradient::ObjectBoundingMode);
    Q_FOREACH (const QGradientStop &stop, d->m_gradient)
        gradient.setColorAt(stop.first, stop.second);
    return gradient;
}
void GradientDialog::setGradient(const QGradient &gradient)
{
    d->m_gradient.clear();
    d->ui->stopSelector->clear();
    const QGradientStops stops = gradient.stops();
    Q_FOREACH (const QGradientStop &stop, stops) {
        d->m_gradient.append(stop);
    }
    QStringList newEntries;
    for (int i = 0; i < stops.count(); ++i)
        newEntries << tr("stop %1").arg(i);
    d->ui->stopSelector->addItems(newEntries);
    d->m_currStopNr = newEntries.count();
    d->updateGradientDisplay();
}

QGradient GradientDialog::getGradient(const QGradient &gradient, QWidget *parent, const QString &title)
{
    GradientDialog dialog(parent);
    dialog.setGradient(gradient);
    dialog.setWindowTitle(title);
    dialog.exec();
    return dialog.gradient();
}

#include "gradientdialog.moc"
