#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QScrollBar>
#include <QtGui/QSplitter>
#include <QtGui/QWidget>

class Ui_MainWindow
{
public:
    QGridLayout *gridLayout;
    QScrollBar *vSBar;
    QScrollBar *hSBar;
    QSplitter *splitter;
    QFrame *chartFrame;

    void setupUi(QWidget *MainWindow)
    {
    MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
    MainWindow->resize(QSize(735, 691).expandedTo(MainWindow->minimumSizeHint()));
    MainWindow->setWindowIcon(QIcon());
    gridLayout = new QGridLayout(MainWindow);
    gridLayout->setSpacing(6);
    gridLayout->setMargin(9);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    vSBar = new QScrollBar(MainWindow);
    vSBar->setObjectName(QString::fromUtf8("vSBar"));
    vSBar->setMaximum(999);
    vSBar->setSingleStep(10);
    vSBar->setPageStep(100);
    vSBar->setValue(500);
    vSBar->setOrientation(Qt::Vertical);

    gridLayout->addWidget(vSBar, 0, 2, 1, 1);

    hSBar = new QScrollBar(MainWindow);
    hSBar->setObjectName(QString::fromUtf8("hSBar"));
    hSBar->setMaximum(999);
    hSBar->setSingleStep(10);
    hSBar->setPageStep(100);
    hSBar->setValue(500);
    hSBar->setOrientation(Qt::Horizontal);

    gridLayout->addWidget(hSBar, 1, 1, 1, 1);

    splitter = new QSplitter(MainWindow);
    splitter->setObjectName(QString::fromUtf8("splitter"));
    splitter->setOrientation(Qt::Horizontal);
    chartFrame = new QFrame(splitter);
    chartFrame->setObjectName(QString::fromUtf8("chartFrame"));
    chartFrame->setMinimumSize(QSize(300, 400));
    chartFrame->setFrameShape(QFrame::StyledPanel);
    chartFrame->setFrameShadow(QFrame::Raised);
    splitter->addWidget(chartFrame);

    gridLayout->addWidget(splitter, 0, 1, 1, 1);

    retranslateUi(MainWindow);

    QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QWidget *MainWindow)
    {
    MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Axes ", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(MainWindow);
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

#endif // UI_MAINWINDOW_H
