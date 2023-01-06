/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QCheckBox;
QT_END_NAMESPACE

namespace KDGantt {
class View;
}

class ProjectModel;

class SavePdfDialog : public QDialog
{
    Q_OBJECT
public:
    QLineEdit *m_fileEdit;
    QCheckBox *m_rowLabels;
    QCheckBox *m_columnLabels;
    SavePdfDialog(QWidget *parent = nullptr);
private slots:
    void fileButtonClicked();
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void slotFileSavePdf();
    void slotFilePrint();
    void slotFileQuit();
    void slotToolsNewItem();
    void slotToolsAppendItem();
    void slotCollapseAll();
    void slotExpandAll();

    void slotAlignLeft();
    void slotAlignCenter();
    void slotAlignRight();
    void slotAlignHidden();

private:
    ProjectModel *m_model;
    KDGantt::View *m_view;
};
#endif /* MAINWINDOW_H */
