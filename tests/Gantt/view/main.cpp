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
#include <QPrintDialog>
#include <QPrinter>
#include <QPushButton>
#include <QSlider>
#include <QStandardItemModel>
#include <QVBoxLayout>

#include <KDGanttConstraintModel>
#include <KDGanttDateTimeGrid>
#include <KDGanttGraphicsView>
#include <KDGanttView>

class MyStandardItem : public QStandardItem
{
public:
    MyStandardItem(const QVariant &v)
        : QStandardItem()
    {
        setData(v, Qt::DisplayRole);
        // setFlags( flags() & ~Qt::ItemIsEditable );
    }
    MyStandardItem(const QString &v)
        : QStandardItem()
    {
        setData(v, Qt::DisplayRole);
        // setFlags( flags() & ~Qt::ItemIsEditable );
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
        qDebug() << "Building data";
        // proxyModel.setSourceModel( &model );
        for (int h = 0; h < 20; ++h) {
            QStandardItem *topitem = new MyStandardItem(QString::fromLatin1("Top Item %1").arg(h));
            for (int i = 0; i < 20; ++i) {
                QStandardItem *item = new MyStandardItem(QString::fromLatin1("Multi Item %1").arg(i));
                for (int j = 0; j < 30; j += 3) {
                    item->appendRow(QList<QStandardItem *>()
                                    << new MyStandardItem(QString::fromLatin1("Item %1").arg(j))
                                    << new MyStandardItem(KDGantt::TypeTask)
                                    << new MyStandardItem(QDateTime::currentDateTime().addDays(j), KDGantt::StartTimeRole)
                                    << new MyStandardItem(QDateTime::currentDateTime().addDays(j + 1 + i / 7), KDGantt::EndTimeRole)
                                    << new MyStandardItem(50));
                }
                item->appendRow(QList<QStandardItem *>()
                                << new MyStandardItem(QString::fromLatin1("Event"))
                                << new MyStandardItem(KDGantt::TypeEvent)
                                << new MyStandardItem(QDateTime::currentDateTime(), KDGantt::StartTimeRole)
                                << new MyStandardItem(QDateTime(), KDGantt::EndTimeRole)
                                << new MyStandardItem(QString()));

                topitem->appendRow(QList<QStandardItem *>()
                                   << item
                                   << new MyStandardItem(KDGantt::TypeMulti)
                                   << new MyStandardItem(QString())
                                   << new MyStandardItem(QString())
                                   << new MyStandardItem(QString()));
            }
            model.appendRow(QList<QStandardItem *>()
                            << topitem
                            << new MyStandardItem(KDGantt::TypeMulti /*Summary*/)
                            << new MyStandardItem(QString())
                            << new MyStandardItem(QString())
                            << new MyStandardItem(QString()));
        }
        model.appendRow(QList<QStandardItem *>()
                        << new MyStandardItem(QString::fromLatin1("No data")));

        // cmodel.addConstraint( KDGantt::Constraint( proxyModel.index( 0, 3 ), proxyModel.index( 10, 3 ) ) );
        // cmodel.addConstraint( KDGantt::Constraint( proxyModel.index( 10, 3 ), proxyModel.index( 5, 3 ) ) );
        QModelIndex pidx = model.index(0, 0);
        pidx = model.index(0, 0, pidx);
        cmodel.addConstraint(KDGantt::Constraint(model.index(0, 0, pidx), model.index(1, 0, pidx)));
        cmodel.addConstraint(KDGantt::Constraint(model.index(1, 0, pidx), model.index(0, 0, pidx)));
        cmodel.addConstraint(KDGantt::Constraint(model.index(1, 0, pidx), model.index(10, 0, pidx)));
        cmodel.addConstraint(KDGantt::Constraint(model.index(3, 0, pidx), model.index(5, 0, pidx)));
        cmodel.addConstraint(KDGantt::Constraint(model.index(7, 0, pidx), model.index(4, 0, pidx)));

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
        // view.setConstraintModel( &cmodel );
        connect(&slider, &QSlider::valueChanged,
                this, &MyWidget::slotZoom);

        auto *pb1 = new QPushButton(tr("Print Preview..."));
        auto *pb2 = new QPushButton(tr("Print..."));
        l->addWidget(pb1);
        l->addWidget(pb2);
        connect(pb1, &QPushButton::clicked, this, &MyWidget::slotPrintPreview);
        connect(pb2, &QPushButton::clicked, this, &MyWidget::slotPrint);

        view.graphicsView()->setHeaderContextMenuPolicy(Qt::CustomContextMenu);
        connect(view.graphicsView(), &KDGantt::GraphicsView::headerContextMenuRequested,
                this, &MyWidget::slotHeaderMenu);
    }

public slots:
    void slotZoom(int z)
    {
        grid.setDayWidth(z);
    }

    void slotPrintPreview()
    {
        QPixmap pix(1000, 200);
        pix.fill(Qt::white);
        QPainter painter(&pix);
        view.print(&painter, pix.rect());
        painter.end();
        auto *label = new QLabel;
        label->setPixmap(pix);
        label->show();
    }
    void slotPrint()
    {
        QPrinter printer(QPrinter::HighResolution);
        QPrintDialog pd(&printer, this);
        if (pd.exec() == QDialog::Accepted) {
            /*
            printer.setResolution( 600 );
            QPainter p( &printer );
            QFont f;
            f.setPixelSize( 9 );
            p.setFont( f );
            view.print( &p );
            */
            QRectF r = view.graphicsView()->mapToScene(view.graphicsView()->viewport()->rect()).boundingRect();
            view.print(&printer, r.left(), r.right());
        }
    }
    void slotHeaderMenu(const QPoint &pt)
    {
        QMenu menu;
        menu.addAction(tr("This"));
        menu.addAction(tr("is"));
        menu.addAction(tr("just"));
        menu.addAction(tr("a"));
        menu.addAction(tr("test"));
        menu.exec(pt);
    }

private:
    KDGantt::View view;
    KDGantt::DateTimeGrid grid;
    QSlider slider;
    QStandardItemModel model;
    KDGantt::ConstraintModel cmodel;
};

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QPixmapCache::setCacheLimit(30 * 1024);

    MyWidget w;
    w.show();

    return app.exec();
}

#include "main.moc"
