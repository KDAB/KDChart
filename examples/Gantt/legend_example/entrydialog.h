#ifndef ENTRYDIALOG_H
#define ENTRYDIALOG_H

#include <QDateTime>
#include <QDialog>
#include <QModelIndex>

class QAbstractItemModel;

namespace KDGantt {
    class ConstraintModel;
}

namespace Ui {
    class EntryDialog;
}

class EntryDialog : public QDialog {
    Q_OBJECT

public:
    EntryDialog( const QAbstractItemModel* model, QWidget* parent = 0, Qt::WindowFlags f = 0 );
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
