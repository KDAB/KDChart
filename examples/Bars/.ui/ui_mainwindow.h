#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QSplitter>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

class Ui_MainWindow
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *vboxLayout;
    QCheckBox *paintValuesCB;
    QFrame *line_3;
    QVBoxLayout *vboxLayout1;
    QLabel *barTypeLB;
    QComboBox *barTypeCB;
    QFrame *line;
    QVBoxLayout *vboxLayout2;
    QLabel *penLabel;
    QHBoxLayout *hboxLayout;
    QCheckBox *markColumnCB;
    QSpinBox *markColumnSB;
    QFrame *line_2;
    QVBoxLayout *vboxLayout3;
    QLabel *threeDLB;
    QCheckBox *paintThreeDBarsCB;
    QHBoxLayout *hboxLayout1;
    QSpacerItem *spacerItem;
    QSplitter *splitter;
    QFrame *chartFrame;

    void setupUi(QWidget *MainWindow)
    {
    MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
    MainWindow->resize(QSize(692, 408).expandedTo(MainWindow->minimumSizeHint()));
    MainWindow->setWindowIcon(QIcon());
    gridLayout = new QGridLayout(MainWindow);
    gridLayout->setSpacing(6);
    gridLayout->setMargin(9);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    vboxLayout = new QVBoxLayout();
    vboxLayout->setSpacing(6);
    vboxLayout->setMargin(0);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    paintValuesCB = new QCheckBox(MainWindow);
    paintValuesCB->setObjectName(QString::fromUtf8("paintValuesCB"));

    vboxLayout->addWidget(paintValuesCB);

    line_3 = new QFrame(MainWindow);
    line_3->setObjectName(QString::fromUtf8("line_3"));
    line_3->setFrameShape(QFrame::HLine);

    vboxLayout->addWidget(line_3);

    vboxLayout1 = new QVBoxLayout();
    vboxLayout1->setSpacing(6);
    vboxLayout1->setMargin(0);
    vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
    barTypeLB = new QLabel(MainWindow);
    barTypeLB->setObjectName(QString::fromUtf8("barTypeLB"));

    vboxLayout1->addWidget(barTypeLB);

    barTypeCB = new QComboBox(MainWindow);
    barTypeCB->setObjectName(QString::fromUtf8("barTypeCB"));

    vboxLayout1->addWidget(barTypeCB);


    vboxLayout->addLayout(vboxLayout1);

    line = new QFrame(MainWindow);
    line->setObjectName(QString::fromUtf8("line"));
    line->setFrameShape(QFrame::HLine);

    vboxLayout->addWidget(line);

    vboxLayout2 = new QVBoxLayout();
    vboxLayout2->setSpacing(6);
    vboxLayout2->setMargin(0);
    vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
    penLabel = new QLabel(MainWindow);
    penLabel->setObjectName(QString::fromUtf8("penLabel"));

    vboxLayout2->addWidget(penLabel);

    hboxLayout = new QHBoxLayout();
    hboxLayout->setSpacing(6);
    hboxLayout->setMargin(0);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    markColumnCB = new QCheckBox(MainWindow);
    markColumnCB->setObjectName(QString::fromUtf8("markColumnCB"));

    hboxLayout->addWidget(markColumnCB);

    markColumnSB = new QSpinBox(MainWindow);
    markColumnSB->setObjectName(QString::fromUtf8("markColumnSB"));
    markColumnSB->setMaximum(4);

    hboxLayout->addWidget(markColumnSB);


    vboxLayout2->addLayout(hboxLayout);


    vboxLayout->addLayout(vboxLayout2);

    line_2 = new QFrame(MainWindow);
    line_2->setObjectName(QString::fromUtf8("line_2"));
    line_2->setFrameShape(QFrame::HLine);

    vboxLayout->addWidget(line_2);

    vboxLayout3 = new QVBoxLayout();
    vboxLayout3->setSpacing(6);
    vboxLayout3->setMargin(0);
    vboxLayout3->setObjectName(QString::fromUtf8("vboxLayout3"));
    threeDLB = new QLabel(MainWindow);
    threeDLB->setObjectName(QString::fromUtf8("threeDLB"));

    vboxLayout3->addWidget(threeDLB);

    paintThreeDBarsCB = new QCheckBox(MainWindow);
    paintThreeDBarsCB->setObjectName(QString::fromUtf8("paintThreeDBarsCB"));

    vboxLayout3->addWidget(paintThreeDBarsCB);

    hboxLayout1 = new QHBoxLayout();
    hboxLayout1->setSpacing(6);
    hboxLayout1->setMargin(0);
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));

    vboxLayout3->addLayout(hboxLayout1);


    vboxLayout->addLayout(vboxLayout3);

    spacerItem = new QSpacerItem(159, 61, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout->addItem(spacerItem);


    gridLayout->addLayout(vboxLayout, 0, 0, 1, 1);

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
    MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Bar Chart ", 0, QApplication::UnicodeUTF8));
    paintValuesCB->setText(QApplication::translate("MainWindow", "Display Data Values", 0, QApplication::UnicodeUTF8));
    barTypeLB->setText(QApplication::translate("MainWindow", "Bar Chart Type:", 0, QApplication::UnicodeUTF8));
    barTypeCB->clear();
    barTypeCB->addItem(QApplication::translate("MainWindow", "Normal", 0, QApplication::UnicodeUTF8));
    barTypeCB->addItem(QApplication::translate("MainWindow", "Stacked", 0, QApplication::UnicodeUTF8));
    barTypeCB->addItem(QApplication::translate("MainWindow", "Percent", 0, QApplication::UnicodeUTF8));
    penLabel->setText(QApplication::translate("MainWindow", "Pen Settings", 0, QApplication::UnicodeUTF8));
    markColumnCB->setText(QApplication::translate("MainWindow", "Mark Column", 0, QApplication::UnicodeUTF8));
    threeDLB->setText(QApplication::translate("MainWindow", "Paint ThreeD Bars", 0, QApplication::UnicodeUTF8));
    paintThreeDBarsCB->setText(QApplication::translate("MainWindow", "ThreeD", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(MainWindow);
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

#endif // UI_MAINWINDOW_H
