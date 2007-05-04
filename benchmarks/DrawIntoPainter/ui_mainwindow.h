/********************************************************************************
** Form generated from reading ui file 'mainwindow.ui'
**
** Created: Wed May 2 17:35:36 2007
**      by: Qt User Interface Compiler version 4.2.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QScrollBar>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QSplitter>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

class Ui_MainWindow
{
public:
    QGridLayout *gridLayout;
    QScrollBar *vSBar;
    QScrollBar *hSBar;
    QSplitter *splitter;
    QFrame *chartFrame;
    QVBoxLayout *vboxLayout;
    QCheckBox *paintValuesCB;
    QFrame *line_3;
    QVBoxLayout *vboxLayout1;
    QLabel *lineTypeLB;
    QComboBox *lineTypeCB;
    QFrame *line_2;
    QVBoxLayout *vboxLayout2;
    QLabel *markersLB;
    QCheckBox *paintMarkersCB;
    QLabel *markersStyleLB;
    QComboBox *markersStyleCB;
    QFrame *line_4;
    QVBoxLayout *vboxLayout3;
    QLabel *markersSizeLB;
    QHBoxLayout *hboxLayout;
    QVBoxLayout *vboxLayout4;
    QLabel *markersWidthLB;
    QLabel *markersHeightLB;
    QVBoxLayout *vboxLayout5;
    QSpinBox *markersWidthSB;
    QSpinBox *markersHeightSB;
    QFrame *line_5;
    QVBoxLayout *vboxLayout6;
    QLabel *AreaChartLB;
    QCheckBox *displayAreasCB;
    QHBoxLayout *hboxLayout1;
    QLabel *transparencyLB;
    QSpinBox *transparencySB;
    QFrame *line;
    QVBoxLayout *vboxLayout7;
    QLabel *zoomingLB;
    QHBoxLayout *hboxLayout2;
    QLabel *zoomFactorLB;
    QDoubleSpinBox *zoomFactorSB;
    QFrame *line_6;
    QCheckBox *paintLegendCB;
    QSpacerItem *spacerItem;
    QPushButton *savePB;

    void setupUi(QWidget *MainWindow)
    {
    MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
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
    line_3->setFrameShadow(QFrame::Sunken);

    vboxLayout->addWidget(line_3);

    vboxLayout1 = new QVBoxLayout();
    vboxLayout1->setSpacing(6);
    vboxLayout1->setMargin(0);
    vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
    lineTypeLB = new QLabel(MainWindow);
    lineTypeLB->setObjectName(QString::fromUtf8("lineTypeLB"));

    vboxLayout1->addWidget(lineTypeLB);

    lineTypeCB = new QComboBox(MainWindow);
    lineTypeCB->setObjectName(QString::fromUtf8("lineTypeCB"));

    vboxLayout1->addWidget(lineTypeCB);


    vboxLayout->addLayout(vboxLayout1);

    line_2 = new QFrame(MainWindow);
    line_2->setObjectName(QString::fromUtf8("line_2"));
    line_2->setFrameShape(QFrame::HLine);
    line_2->setFrameShadow(QFrame::Sunken);

    vboxLayout->addWidget(line_2);

    vboxLayout2 = new QVBoxLayout();
    vboxLayout2->setSpacing(6);
    vboxLayout2->setMargin(0);
    vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
    markersLB = new QLabel(MainWindow);
    markersLB->setObjectName(QString::fromUtf8("markersLB"));

    vboxLayout2->addWidget(markersLB);

    paintMarkersCB = new QCheckBox(MainWindow);
    paintMarkersCB->setObjectName(QString::fromUtf8("paintMarkersCB"));

    vboxLayout2->addWidget(paintMarkersCB);

    markersStyleLB = new QLabel(MainWindow);
    markersStyleLB->setObjectName(QString::fromUtf8("markersStyleLB"));

    vboxLayout2->addWidget(markersStyleLB);

    markersStyleCB = new QComboBox(MainWindow);
    markersStyleCB->setObjectName(QString::fromUtf8("markersStyleCB"));

    vboxLayout2->addWidget(markersStyleCB);


    vboxLayout->addLayout(vboxLayout2);

    line_4 = new QFrame(MainWindow);
    line_4->setObjectName(QString::fromUtf8("line_4"));
    line_4->setFrameShape(QFrame::HLine);
    line_4->setFrameShadow(QFrame::Sunken);

    vboxLayout->addWidget(line_4);

    vboxLayout3 = new QVBoxLayout();
    vboxLayout3->setSpacing(6);
    vboxLayout3->setMargin(0);
    vboxLayout3->setObjectName(QString::fromUtf8("vboxLayout3"));
    markersSizeLB = new QLabel(MainWindow);
    markersSizeLB->setObjectName(QString::fromUtf8("markersSizeLB"));

    vboxLayout3->addWidget(markersSizeLB);

    hboxLayout = new QHBoxLayout();
    hboxLayout->setSpacing(6);
    hboxLayout->setMargin(0);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    vboxLayout4 = new QVBoxLayout();
    vboxLayout4->setSpacing(6);
    vboxLayout4->setMargin(0);
    vboxLayout4->setObjectName(QString::fromUtf8("vboxLayout4"));
    markersWidthLB = new QLabel(MainWindow);
    markersWidthLB->setObjectName(QString::fromUtf8("markersWidthLB"));

    vboxLayout4->addWidget(markersWidthLB);

    markersHeightLB = new QLabel(MainWindow);
    markersHeightLB->setObjectName(QString::fromUtf8("markersHeightLB"));

    vboxLayout4->addWidget(markersHeightLB);


    hboxLayout->addLayout(vboxLayout4);

    vboxLayout5 = new QVBoxLayout();
    vboxLayout5->setSpacing(6);
    vboxLayout5->setMargin(0);
    vboxLayout5->setObjectName(QString::fromUtf8("vboxLayout5"));
    markersWidthSB = new QSpinBox(MainWindow);
    markersWidthSB->setObjectName(QString::fromUtf8("markersWidthSB"));
    markersWidthSB->setValue(10);

    vboxLayout5->addWidget(markersWidthSB);

    markersHeightSB = new QSpinBox(MainWindow);
    markersHeightSB->setObjectName(QString::fromUtf8("markersHeightSB"));
    markersHeightSB->setValue(10);

    vboxLayout5->addWidget(markersHeightSB);


    hboxLayout->addLayout(vboxLayout5);


    vboxLayout3->addLayout(hboxLayout);


    vboxLayout->addLayout(vboxLayout3);

    line_5 = new QFrame(MainWindow);
    line_5->setObjectName(QString::fromUtf8("line_5"));
    line_5->setFrameShape(QFrame::HLine);
    line_5->setFrameShadow(QFrame::Sunken);

    vboxLayout->addWidget(line_5);

    vboxLayout6 = new QVBoxLayout();
    vboxLayout6->setSpacing(6);
    vboxLayout6->setMargin(0);
    vboxLayout6->setObjectName(QString::fromUtf8("vboxLayout6"));
    AreaChartLB = new QLabel(MainWindow);
    AreaChartLB->setObjectName(QString::fromUtf8("AreaChartLB"));

    vboxLayout6->addWidget(AreaChartLB);

    displayAreasCB = new QCheckBox(MainWindow);
    displayAreasCB->setObjectName(QString::fromUtf8("displayAreasCB"));

    vboxLayout6->addWidget(displayAreasCB);

    hboxLayout1 = new QHBoxLayout();
    hboxLayout1->setSpacing(6);
    hboxLayout1->setMargin(0);
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    transparencyLB = new QLabel(MainWindow);
    transparencyLB->setObjectName(QString::fromUtf8("transparencyLB"));

    hboxLayout1->addWidget(transparencyLB);

    transparencySB = new QSpinBox(MainWindow);
    transparencySB->setObjectName(QString::fromUtf8("transparencySB"));
    transparencySB->setMaximum(255);
    transparencySB->setValue(20);

    hboxLayout1->addWidget(transparencySB);


    vboxLayout6->addLayout(hboxLayout1);


    vboxLayout->addLayout(vboxLayout6);

    line = new QFrame(MainWindow);
    line->setObjectName(QString::fromUtf8("line"));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    vboxLayout->addWidget(line);

    vboxLayout7 = new QVBoxLayout();
    vboxLayout7->setSpacing(6);
    vboxLayout7->setMargin(0);
    vboxLayout7->setObjectName(QString::fromUtf8("vboxLayout7"));
    zoomingLB = new QLabel(MainWindow);
    zoomingLB->setObjectName(QString::fromUtf8("zoomingLB"));

    vboxLayout7->addWidget(zoomingLB);

    hboxLayout2 = new QHBoxLayout();
    hboxLayout2->setSpacing(6);
    hboxLayout2->setMargin(0);
    hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
    zoomFactorLB = new QLabel(MainWindow);
    zoomFactorLB->setObjectName(QString::fromUtf8("zoomFactorLB"));

    hboxLayout2->addWidget(zoomFactorLB);

    zoomFactorSB = new QDoubleSpinBox(MainWindow);
    zoomFactorSB->setObjectName(QString::fromUtf8("zoomFactorSB"));
    zoomFactorSB->setMaximum(9.9);
    zoomFactorSB->setMinimum(0.01);
    zoomFactorSB->setSingleStep(0.1);
    zoomFactorSB->setValue(1);

    hboxLayout2->addWidget(zoomFactorSB);


    vboxLayout7->addLayout(hboxLayout2);


    vboxLayout->addLayout(vboxLayout7);

    line_6 = new QFrame(MainWindow);
    line_6->setObjectName(QString::fromUtf8("line_6"));
    line_6->setFrameShape(QFrame::HLine);
    line_6->setFrameShadow(QFrame::Sunken);

    vboxLayout->addWidget(line_6);

    paintLegendCB = new QCheckBox(MainWindow);
    paintLegendCB->setObjectName(QString::fromUtf8("paintLegendCB"));
    paintLegendCB->setChecked(true);

    vboxLayout->addWidget(paintLegendCB);

    spacerItem = new QSpacerItem(170, 16, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout->addItem(spacerItem);

    savePB = new QPushButton(MainWindow);
    savePB->setObjectName(QString::fromUtf8("savePB"));

    vboxLayout->addWidget(savePB);


    gridLayout->addLayout(vboxLayout, 0, 0, 1, 1);


    retranslateUi(MainWindow);

    QSize size(735, 702);
    size = size.expandedTo(MainWindow->minimumSizeHint());
    MainWindow->resize(size);


    QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QWidget *MainWindow)
    {
    MainWindow->setWindowTitle(QApplication::translate("MainWindow", "KD Chart drawing into a foreign QPainter ", 0, QApplication::UnicodeUTF8));
    paintValuesCB->setText(QApplication::translate("MainWindow", "Display Data Values", 0, QApplication::UnicodeUTF8));
    lineTypeLB->setText(QApplication::translate("MainWindow", "Line Chart Type:", 0, QApplication::UnicodeUTF8));
    lineTypeCB->clear();
    lineTypeCB->addItem(QApplication::translate("MainWindow", "Normal", 0, QApplication::UnicodeUTF8));
    lineTypeCB->addItem(QApplication::translate("MainWindow", "Stacked", 0, QApplication::UnicodeUTF8));
    lineTypeCB->addItem(QApplication::translate("MainWindow", "Percent", 0, QApplication::UnicodeUTF8));
    markersLB->setText(QApplication::translate("MainWindow", "Markers:", 0, QApplication::UnicodeUTF8));
    paintMarkersCB->setText(QApplication::translate("MainWindow", "Paint Markers", 0, QApplication::UnicodeUTF8));
    markersStyleLB->setText(QApplication::translate("MainWindow", "Markers Style", 0, QApplication::UnicodeUTF8));
    markersStyleCB->clear();
    markersStyleCB->addItem(QApplication::translate("MainWindow", "Map", 0, QApplication::UnicodeUTF8));
    markersStyleCB->addItem(QApplication::translate("MainWindow", "Circle", 0, QApplication::UnicodeUTF8));
    markersStyleCB->addItem(QApplication::translate("MainWindow", "Square", 0, QApplication::UnicodeUTF8));
    markersStyleCB->addItem(QApplication::translate("MainWindow", "Diamond", 0, QApplication::UnicodeUTF8));
    markersStyleCB->addItem(QApplication::translate("MainWindow", "1 Pixel", 0, QApplication::UnicodeUTF8));
    markersStyleCB->addItem(QApplication::translate("MainWindow", "4 Pixels", 0, QApplication::UnicodeUTF8));
    markersStyleCB->addItem(QApplication::translate("MainWindow", "Ring", 0, QApplication::UnicodeUTF8));
    markersStyleCB->addItem(QApplication::translate("MainWindow", "Cross", 0, QApplication::UnicodeUTF8));
    markersStyleCB->addItem(QApplication::translate("MainWindow", "Fast Cross", 0, QApplication::UnicodeUTF8));
    markersSizeLB->setText(QApplication::translate("MainWindow", "Markers Size:", 0, QApplication::UnicodeUTF8));
    markersWidthLB->setText(QApplication::translate("MainWindow", "Width:", 0, QApplication::UnicodeUTF8));
    markersHeightLB->setText(QApplication::translate("MainWindow", "Height", 0, QApplication::UnicodeUTF8));
    AreaChartLB->setText(QApplication::translate("MainWindow", "Area Charts:", 0, QApplication::UnicodeUTF8));
    displayAreasCB->setText(QApplication::translate("MainWindow", "Display Areas", 0, QApplication::UnicodeUTF8));
    transparencyLB->setText(QApplication::translate("MainWindow", "Transparency:", 0, QApplication::UnicodeUTF8));
    zoomingLB->setText(QApplication::translate("MainWindow", "Zooming:", 0, QApplication::UnicodeUTF8));
    zoomFactorLB->setText(QApplication::translate("MainWindow", "Zoom Factor:", 0, QApplication::UnicodeUTF8));
    paintLegendCB->setText(QApplication::translate("MainWindow", "Display Legend", 0, QApplication::UnicodeUTF8));
    savePB->setText(QApplication::translate("MainWindow", "Save as PNG", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(MainWindow);
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

#endif // UI_MAINWINDOW_H
