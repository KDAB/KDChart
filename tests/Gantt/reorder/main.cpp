/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include <QApplication>
#include <QModelIndex>
#include <QStandardItemModel>
#include <QTimer>

#include <cassert>

#include <KDGanttConstraintModel>
#include <KDGanttView>

/* Test model that uses beginMoveRows() */
class MyTaskModel : public QAbstractTableModel
{
    struct Task
    {
        QString title;
        QDateTime start, end;
    };
    QList<Task> m_tasks;

public:
    MyTaskModel(QObject *parent = 0)
        : QAbstractTableModel(parent)
    {
        Task task;
        task.title = tr("Item 1");
        task.start = QDateTime::currentDateTime();
        task.end = task.start.addDays(1);
        m_tasks.push_back(task);
        task.title = tr("Item 2");
        task.start = QDateTime::currentDateTime().addDays(2);
        task.end = task.start.addDays(1);
        m_tasks.push_back(task);
        task.title = tr("Item 3");
        task.start = QDateTime::currentDateTime().addDays(4);
        task.end = task.start.addDays(1);
        m_tasks.push_back(task);
        task.title = tr("Item 4");
        task.start = QDateTime::currentDateTime().addDays(6);
        task.end = task.start.addDays(1);
        m_tasks.push_back(task);
    }

    /*reimp*/ int rowCount(const QModelIndex &index = QModelIndex()) const override
    {
        return index.isValid() ? 0 : m_tasks.size();
    }
    /*reimp*/ int columnCount(const QModelIndex &index = QModelIndex()) const override
    {
        return index.isValid() ? 0 : 4;
    }
    /*reimp*/ QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
    {
        if (index.isValid() && index.row() < rowCount() && index.column() < columnCount()) {
            switch (index.column()) {
            case 0:
                return (role == Qt::DisplayRole)
                    ? QVariant::fromValue(m_tasks[index.row()].title)
                    : QVariant();
            case 1:
                return (role == Qt::DisplayRole)
                    ? QVariant::fromValue(( int )KDGantt::TypeTask)
                    : QVariant();
            case 2:
                return (role == KDGantt::StartTimeRole || role == Qt::DisplayRole)
                    ? QVariant::fromValue(m_tasks[index.row()].start)
                    : QVariant();
            case 3:
                return (role == KDGantt::EndTimeRole || role == Qt::DisplayRole)
                    ? QVariant::fromValue(m_tasks[index.row()].end)
                    : QVariant();
            }
        }
        return QVariant();
    }

    void moveRow(int from, int to)
    {
        if (from == to)
            return;
        if (from >= m_tasks.size() || to >= m_tasks.size() + 1)
            return;

        if (beginMoveRows(QModelIndex(), from, from, QModelIndex(), to)) {
            m_tasks.move(from, to);
            endMoveRows();
        } else {
            assert(0);
        }
    }
};

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    auto *view = new KDGantt::View;

    MyTaskModel model;

    KDGantt::ConstraintModel constraints;
    constraints.addConstraint(KDGantt::Constraint(model.index(0, 0), model.index(1, 0)));
    constraints.addConstraint(KDGantt::Constraint(model.index(0, 0), model.index(2, 0)));
    constraints.addConstraint(KDGantt::Constraint(model.index(1, 0), model.index(3, 0)));

    view->setModel(&model);
    view->setConstraintModel(&constraints);
    view->show();

    QTimer::singleShot(5000, &app, [&model] {
        /* After 5 seconds, move row 1 to pos 0: */
        qDebug() << "Moving row" << 1 << "to" << 0;
        model.moveRow(1, 0);
    });

    return app.exec();
}
