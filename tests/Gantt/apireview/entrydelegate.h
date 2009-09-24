#ifndef ENTRYDELEGATE_H
#define ENTRYDELEGATE_H

#include <QItemDelegate>

namespace KDGantt {
    class ConstraintModel;
}

class EntryDelegate : public QItemDelegate {
public:
    EntryDelegate( KDGantt::ConstraintModel* constraintModel, QObject* parent = 0 );
    
    bool editorEvent( QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index );
    
private:
    void addConstraint(const QModelIndex & index1, const QModelIndex & index2);
    void setReadOnly(const QModelIndex & index, bool readOnly);
    
    KDGantt::ConstraintModel* constraintModel;
};

#endif /* ENTRYDELEGATE_H */
