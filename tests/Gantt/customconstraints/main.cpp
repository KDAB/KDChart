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
#include <QDateTime>
#include <QDebug>
#include <QLabel>
#include <QMenu>
#include <QPainter>
#include <QPixmap>
#include <QPixmapCache>
#include <QSlider>
#include <QStandardItemModel>
#include <QVBoxLayout>

#include <KDGanttConstraintModel>
#include <KDGanttDateTimeGrid>
#include <KDGanttGraphicsView>
#include <KDGanttView>

class MyGraphicsView : public KDGantt::GraphicsView
{
    Q_OBJECT
public:
    MyGraphicsView(QWidget *parent = 0)
        : KDGantt::GraphicsView(parent)
    {
    }

    /*reimp*/ void addConstraint(const QModelIndex &from,
                                 const QModelIndex &to,
                                 Qt::KeyboardModifiers modifiers) override
    {
        qDebug() << "MyGraphicsView::addConstraint()";
        if (isReadOnly())
            return;
        KDGantt::ConstraintModel *cmodel = constraintModel();
        KDGantt::Constraint c(from, to, (modifiers & Qt::ShiftModifier) ? KDGantt::Constraint::TypeHard : KDGantt::Constraint::TypeSoft);
        c.setData(KDGantt::Constraint::ValidConstraintPen, QPen(QColor(Qt::green), 3, Qt::DashLine));
        c.setData(KDGantt::Constraint::InvalidConstraintPen, QPen(QColor(Qt::blue), 3, Qt::DashLine));
        if (cmodel->hasConstraint(c))
            cmodel->removeConstraint(c);
        else
            cmodel->addConstraint(c);
    }
};

class MyStandardItem : public QStandardItem
{
public:
    MyStandardItem(const QVariant &v)
        : QStandardItem()
    {
        setData(v, Qt::DisplayRole);
    }
    MyStandardItem(const QString &v)
        : QStandardItem()
    {
        setData(v, Qt::DisplayRole);
    }
    MyStandardItem(const QDateTime &dt, int role)
        : QStandardItem()
    {
        setData(QVariant::fromValue(dt), role);
    }
};

class MyWidget : public QWidget
{
    Q_OBJECT
public:
    MyWidget()
        : QWidget(0)
    {
        view.setGraphicsView(new MyGraphicsView);

        qDebug() << "Building data";
        // proxyModel.setSourceModel( &model );
        for (int h = 0; h < 2; ++h) {
            QList<QStandardItem *> items;
            items
                << new MyStandardItem(QString::fromLatin1("Item %1").arg(h))
                << new MyStandardItem(KDGantt::TypeTask)
                << new MyStandardItem(QDateTime::currentDateTime().addDays(h), KDGantt::StartTimeRole)
                << new MyStandardItem(QDateTime::currentDateTime().addDays(h + 1), KDGantt::EndTimeRole)
                << new MyStandardItem(50);
            model.appendRow(items);
        }

        qDebug() << "Creating view";
        slider.setOrientation(Qt::Horizontal);
        slider.setRange(1, 10000);
        slider.setValue(100);
        auto *l = new QVBoxLayout(this);
        l->addWidget(&view);
        l->addWidget(&slider);
        grid.setStartDateTime(QDateTime::currentDateTime().addDays(-3));
        grid.setDayWidth(100);
        // grid.setNoInformationBrush( Qt::NoBrush );
        view.setGrid(&grid);
        view.setModel(&model);

        connect(&slider, SIGNAL(valueChanged(int)),
                this, SLOT(slotZoom(int)));
    }

public slots:
    void slotZoom(int z)
    {
        grid.setDayWidth(z);
    }

private:
    KDGantt::View view;
    KDGantt::DateTimeGrid grid;
    QSlider slider;
    QStandardItemModel model;
};

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    // QPixmapCache::setCacheLimit( 30*1024 );

    MyWidget w;
    w.show();

    return app.exec();
}

#include "main.moc"
