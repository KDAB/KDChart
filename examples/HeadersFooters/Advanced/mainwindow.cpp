/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "mainwindow.h"

#include <KDChartCartesianCoordinatePlane>
#include <KDChartChart>
#include <KDChartDatasetProxyModel>
#include <KDChartHeaderFooter>
#include <KDChartLineDiagram>
#include <KDChartPosition>
#include <KDChartTextAttributes>
#include <QComboBox>
#include <QLineEdit>
#include <QPen>

class HeaderItem : public QTreeWidgetItem
{
public:
    HeaderItem(KDChart::HeaderFooter *header, QTreeWidget *parent)
        : QTreeWidgetItem(parent)
        , m_header(header)
    {
    }

    KDChart::HeaderFooter *header() const
    {
        return m_header;
    }

private:
    KDChart::HeaderFooter *m_header;
};

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);

    auto *chartLayout = new QHBoxLayout(chartFrame);
    m_chart = new KDChart::Chart();
    chartLayout->addWidget(m_chart);

    m_model.loadFromCSV(":/data");

    // Set up the diagram
    m_lines = new KDChart::LineDiagram();
    m_lines->setModel(&m_model);
    m_chart->coordinatePlane()->replaceDiagram(m_lines);

    m_chart->update();
}

void MainWindow::setupAddHeaderDialog(QDialog *dlg,
                                      Ui::AddHeaderDialog &conf) const
{
    conf.setupUi(dlg);
    conf.textED->setFocus();

    // Note: Header/Footer position can be Center but it can not be Floating
    const QStringList labels = KDChart::Position::printableNames(KDChart::Position::IncludeCenter);
    const QList<QByteArray> names = KDChart::Position::names(KDChart::Position::IncludeCenter);

    for (int i = 0, end = qMin(labels.size(), names.size()); i != end; ++i)
        conf.positionCO->addItem(labels[i], names[i]);
}

void MainWindow::on_addHeaderPB_clicked()
{
    QDialog dlg;
    Ui::AddHeaderDialog conf;
    setupAddHeaderDialog(&dlg, conf);
    conf.typeCO->setCurrentIndex(0); // let us start with "Header"
    conf.positionCO->setCurrentIndex(0);
    if (dlg.exec()) {
        auto *headerFooter = new KDChart::HeaderFooter(m_chart);
        m_chart->addHeaderFooter(headerFooter);
        headerFooter->setText(conf.textED->text());
        KDChart::TextAttributes attrs(headerFooter->textAttributes());
        attrs.setPen(QPen(Qt::red));
        headerFooter->setTextAttributes(attrs);
        headerFooter->setType(conf.typeCO->currentText() == "Header" ? KDChart::HeaderFooter::Header : KDChart::HeaderFooter::Footer);
        headerFooter->setPosition(
            KDChart::Position::fromName(conf.positionCO->itemData(
                                                           conf.positionCO->currentIndex())
                                            .toByteArray()));

        auto *newItem = new HeaderItem(headerFooter, headersTV);
        newItem->setText(0, conf.textED->text());
        newItem->setText(1, headerFooter->type() == KDChart::HeaderFooter::Header ? tr("Header") : tr("Footer"));
        newItem->setText(2, conf.positionCO->currentText());
        m_chart->update();
    }
}

void MainWindow::on_editHeaderPB_clicked()
{
    if (headersTV->selectedItems().size() == 0)
        return;
    HeaderItem *item =
        static_cast<HeaderItem *>(headersTV->selectedItems().first());
    KDChart::HeaderFooter *headerFooter = item->header();
    QDialog dlg;
    Ui::AddHeaderDialog conf;
    setupAddHeaderDialog(&dlg, conf);
    conf.textED->setText(headerFooter->text());
    conf.typeCO->setCurrentIndex(headerFooter->type() == KDChart::HeaderFooter::Header ? 0 : 1);
    conf.positionCO->setCurrentIndex(
        conf.positionCO->findText(headerFooter->position().printableName()));
    if (dlg.exec()) {
        headerFooter->setText(conf.textED->text());
        headerFooter->setType(conf.typeCO->currentText() == "Header" ? KDChart::HeaderFooter::Header : KDChart::HeaderFooter::Footer);
        headerFooter->setPosition(
            KDChart::Position::fromName(conf.positionCO->itemData(
                                                           conf.positionCO->currentIndex())
                                            .toByteArray()));
        item->setText(0, conf.textED->text());
        item->setText(1, headerFooter->type() == KDChart::HeaderFooter::Header ? tr("Header") : tr("Footer"));
        item->setText(2, conf.positionCO->currentText());
        m_chart->update();
    }
}

void MainWindow::on_removeHeaderPB_clicked()
{
    if (headersTV->selectedItems().size() == 0)
        return;
    QList<QTreeWidgetItem *> items = headersTV->selectedItems();
    for (QList<QTreeWidgetItem *>::const_iterator it = items.begin(); it != items.end(); ++it) {
        KDChart::HeaderFooter *headerFooter = static_cast<HeaderItem *>((*it))->header();
#if 0
        // Note: Despite it being owned by the Chart, you *can* just
        //       delete the header: KD Chart will notice that and
        //       it will adjust its layout ...
        delete headerFooter;
#else
        // ... but the correct way is to first take it, so the Chart
        // is no longer owning it:
        m_chart->takeHeaderFooter(headerFooter);
        // ... and then delete it:
        delete headerFooter;
#endif
        delete (*it);
    }
    m_chart->update();
}

void MainWindow::on_headersTV_itemSelectionChanged()
{
    removeHeaderPB->setEnabled(headersTV->selectedItems().count() > 0);
    editHeaderPB->setEnabled(headersTV->selectedItems().count() == 1);
}
