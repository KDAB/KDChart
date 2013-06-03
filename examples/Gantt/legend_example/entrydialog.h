/****************************************************************************
** Copyright (C) 2001-2013 Klaralvdalens Datakonsult AB.  All rights reserved.
**
** This file is part of the KD Chart library.
**
** Licensees holding valid commercial KD Chart licenses may use this file in
** accordance with the KD Chart Commercial License Agreement provided with
** the Software.
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 and version 3 as published by the
** Free Software Foundation and appearing in the file LICENSE.GPL.txt included.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** Contact info@kdab.com if any conditions of this licensing are not
** clear to you.
**
**********************************************************************/

#ifndef ENTRYDIALOG_H
#define ENTRYDIALOG_H

#include <QDateTime>
#include <QDialog>
#include <QModelIndex>

QT_BEGIN_NAMESPACE
class QAbstractItemModel;
namespace Ui {
    class EntryDialog;
}
QT_END_NAMESPACE

namespace KDGantt {
    class ConstraintModel;
}

class EntryDialog : public QDialog {
    Q_OBJECT

public:
    explicit EntryDialog( const QAbstractItemModel* model, QWidget* parent = 0, Qt::WindowFlags f = 0 );
    void initFrom( const QModelIndex& index, const KDGantt::ConstraintModel* constraintModel );
    
    QString name() const;
    int type() const;
    QDateTime startDate() const;
    QDateTime endDate() const;
    int completion() const;
    bool readOnly() const;
    QModelIndex depends() const;
    QString legend() const;

private slots:
    void updateEndDate( const QDateTime& startDate );
    void disableEditing( bool disable );
    void typeChanged( int index );
    
private:
    void init();
    void addDependItem( const QAbstractItemModel* model, const QModelIndex& index, int indent = 0 );
    
    QList<QPersistentModelIndex> indexList;
    const QAbstractItemModel* model;
    Ui::EntryDialog* ui;
};

#endif /* ENTRYDIALOG_H */
