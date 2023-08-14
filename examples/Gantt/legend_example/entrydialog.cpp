/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "entrydialog.h"

#include "ui_entrydialog.h"

#include <KDGanttConstraintModel>
#include <KDGanttGlobal>
#include <QAbstractItemModel>
#include <QDateTime>
#include <QPersistentModelIndex>

Q_DECLARE_METATYPE(QPersistentModelIndex)

EntryDialog::EntryDialog(const QAbstractItemModel *model, QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f)
    , indexList(QList<QPersistentModelIndex>())
    , ui(new Ui::EntryDialog)
{
    ui->setupUi(this);
    this->model = model;
    init();
}

void EntryDialog::init()
{
    ui->type->addItem(tr("Event"), KDGantt::TypeEvent);
    ui->type->addItem(tr("Task"), KDGantt::TypeTask);
    ui->type->addItem(tr("Summary"), KDGantt::TypeSummary);
    ui->type->addItem(tr("Multi"), KDGantt::TypeMulti);

    for (int row = 0; row < model->rowCount(); ++row)
        addDependItem(model, model->index(row, 0));

    connect(ui->startDate, &QDateTimeEdit::dateTimeChanged,
            this, &EntryDialog::updateEndDate);
    connect(ui->readOnly, &QCheckBox::toggled, this, &EntryDialog::disableEditing);
    connect(ui->type, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &EntryDialog::typeChanged);

    ui->startDate->setDateTime(QDateTime::currentDateTime());
    typeChanged(0);
}

void EntryDialog::initFrom(const QModelIndex &index, const KDGantt::ConstraintModel *constraintModel)
{
    int row = index.row();
    const QModelIndex parent = index.parent();

    ui->name->setText(model->data(model->index(row, 0, parent)).toString());
    ui->legend->setText(model->data(model->index(row, 5, parent)).toString());
    int idx = ui->type->findData(model->data(model->index(row, 1, parent)).toInt());
    ui->type->setCurrentIndex(idx);
    ui->startDate->setDateTime(model->data(model->index(row, 2, parent), KDGantt::StartTimeRole).toDateTime());
    ui->endDate->setDateTime(model->data(model->index(row, 3, parent), KDGantt::EndTimeRole).toDateTime());
    ui->completion->setValue(model->data(model->index(row, 4, parent)).toInt());
    ui->readOnly->setChecked(!(model->flags(model->index(row, 0, parent)) & Qt::ItemIsEditable));

    QList<KDGantt::Constraint> constraints = constraintModel->constraintsForIndex(model->index(row, 0, parent));
    if (constraints.isEmpty())
        return;

    QModelIndex constraintIndex;
    for (int i = 0; i < constraints.size(); ++i) {
        KDGantt::Constraint constraint = constraints[i];
        if (constraint.endIndex() == index) {
            constraintIndex = constraint.startIndex();
            break;
        }
    }

    if (!constraintIndex.isValid())
        return;

    ui->depends->setCurrentIndex(indexList.indexOf(constraintIndex) + 1);
}

void EntryDialog::addDependItem(const QAbstractItemModel *model, const QModelIndex &index, int indent)
{
    indexList << QPersistentModelIndex(index);
    QString str = QString("%1%2").arg(QString().fill(' ', indent)).arg(model->data(index).toString());
    ui->depends->addItem(str);

    for (int row = 0; row < model->rowCount(index); ++row)
        addDependItem(model, model->index(row, 0, index), indent + 2);
}

QString EntryDialog::name() const
{
    return ui->name->text();
}

int EntryDialog::type() const
{
    return ui->type->itemData(ui->type->currentIndex()).toInt();
}

QDateTime EntryDialog::startDate() const
{
    return ui->startDate->dateTime();
}

QDateTime EntryDialog::endDate() const
{
    return ui->endDate->dateTime();
}

int EntryDialog::completion() const
{
    return ui->completion->value();
}

void EntryDialog::updateEndDate(const QDateTime &startDate)
{
    ui->endDate->setMinimumDate(startDate.date());
    ui->endDate->setMinimumTime(startDate.time());
}

bool EntryDialog::readOnly() const
{
    return ui->readOnly->isChecked();
}

QModelIndex EntryDialog::depends() const
{
    if (ui->depends->currentIndex() == 0)
        return QModelIndex();

    QPersistentModelIndex index = indexList[ui->depends->currentIndex() - 1];
    if (index.isValid())
        return index;

    return QModelIndex();
}

QString EntryDialog::legend() const
{
    return ui->legend->text();
}

void EntryDialog::disableEditing(bool disable)
{
    ui->name->setEnabled(!disable);
    ui->type->setEnabled(!disable);
    ui->completion->setEnabled(!disable);
    ui->startDate->setEnabled(!disable);
    ui->endDate->setEnabled(!disable);
    ui->depends->setEnabled(!disable);
}

void EntryDialog::typeChanged(int index)
{
    if (!index) {
        ui->label_EndDate->hide();
        ui->endDate->hide();
    } else {
        ui->label_EndDate->show();
        ui->endDate->show();
    }
}
