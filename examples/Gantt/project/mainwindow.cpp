#include "mainwindow.h"
#include "projectmodel.h"

#include <QAction>
#include <QApplication>
#include <QComboBox>
#include <QMenuBar>
#include <QMenu>
#include <QModelIndex>
#include <QItemSelectionModel>
#include <QTreeView>
#include <QDebug>

#include <KDGanttView>
#include <KDGanttItemDelegate>

class ItemTypeComboBox : public QComboBox {
    Q_OBJECT
    Q_PROPERTY( KDGantt::ItemType itemType READ itemType WRITE setItemType )
public:
    explicit ItemTypeComboBox( QWidget* parent=0 );

    KDGantt::ItemType itemType() const;
public slots:
    void setItemType( KDGantt::ItemType typ );
};

ItemTypeComboBox::ItemTypeComboBox( QWidget* parent )
    : QComboBox( parent )
{
    addItem( tr( "Task" ), QVariant( KDGantt::TypeTask ) );
    addItem( tr( "Event" ), QVariant( KDGantt::TypeEvent ) );
    addItem( tr( "Summary" ), QVariant( KDGantt::TypeSummary ) );
}

KDGantt::ItemType ItemTypeComboBox::itemType() const
{
    return static_cast<KDGantt::ItemType>( itemData( currentIndex() ).toInt() );
}

void ItemTypeComboBox::setItemType( KDGantt::ItemType typ )
{
    setCurrentIndex( typ-1 );
}

class MyItemDelegate : public KDGantt::ItemDelegate {
public:
    explicit MyItemDelegate( QObject* parent=0 );

    /*reimp*/ QWidget* createEditor( QWidget* parent,
                                     const QStyleOptionViewItem& option,
                                     const QModelIndex& idx ) const;
    /*reimp*/ void setEditorData( QWidget* editor, const QModelIndex& index ) const;
    /*reimp*/ void setModelData( QWidget* editor, QAbstractItemModel* model,
				  const QModelIndex & index ) const;
protected:
    /*reimp*/void drawDisplay( QPainter* painter, const QStyleOptionViewItem & option,
			       const QRect& rect, const QString& text ) const;
private:
    bool m_isitemtype; // Nasty!
};

MyItemDelegate::MyItemDelegate( QObject* parent )
    : KDGantt::ItemDelegate( parent )
{
}

QWidget* MyItemDelegate::createEditor( QWidget* parent,
                                       const QStyleOptionViewItem& option,
                                       const QModelIndex& idx ) const
{
    qDebug() << "MyItemDelegate::createEditor("<<parent<<idx<<")";
    if ( idx.isValid() && idx.column() == 1 ) {
      return new ItemTypeComboBox(parent);
    } else return ItemDelegate::createEditor( parent, option, idx );
}

void MyItemDelegate::setEditorData ( QWidget* editor, const QModelIndex& index ) const
{
  ItemTypeComboBox* c;
  if( (c = qobject_cast<ItemTypeComboBox*>(editor)) && index.isValid() ) {
      c->setItemType(static_cast<KDGantt::ItemType>(index.data(Qt::EditRole).toInt()));
  } else {
      ItemDelegate::setEditorData(editor,index);
  }
}

void MyItemDelegate::setModelData ( QWidget* editor, QAbstractItemModel* model,
				  const QModelIndex & index ) const
{
    ItemTypeComboBox* c;
    if( (c = qobject_cast<ItemTypeComboBox*>(editor)) && index.isValid() ) {
        model->setData(index,c->itemType());
  } else {
        ItemDelegate::setModelData(editor,model,index);
  }
}

void MyItemDelegate::drawDisplay( QPainter* painter, const QStyleOptionViewItem& option,
				  const QRect& rect, const QString& text ) const
{
  qDebug() << "MyItemDelegate::drawDisplay(" <<painter<<rect<<text<<")";
  KDGantt::ItemType typ = static_cast<KDGantt::ItemType>(text.toInt());
  QString str;
  switch(typ){
  case KDGantt::TypeTask: str = tr("Task"); break;
  case KDGantt::TypeEvent: str = tr("Event"); break;
  case KDGantt::TypeSummary: str = tr("Summary"); break;
  default: str = tr("None"); break;
  }
  ItemDelegate::drawDisplay(painter,option,rect,str);
}


MainWindow::MainWindow( QWidget* parent )
    : QMainWindow( parent ),
      m_model( new ProjectModel( this ) ),
      m_view( new KDGantt::View )
{
    m_view->setModel( m_model );
    m_view->setSelectionModel( new QItemSelectionModel(m_model));

    slotToolsNewItem();
    m_view->leftView()->setItemDelegateForColumn( 1, new MyItemDelegate( this ) );
  //QItemEditorCreatorBase *creator = new QItemEditorCreator<ItemTypeComboBox>("itemType");
  //QItemEditorFactory* factory = new QItemEditorFactory;
  //factory->registerEditor( QVariant( KDGantt::TypeTask ).type(), creator );
    //m_view->itemDelegate()->setItemEditorFactory( factory );

    setCentralWidget( m_view );

    QMenuBar* mb = menuBar();

    QMenu* fileMenu = new QMenu( tr( "&File" ) );

    //  QAction* fileOpen = fileMenu->addAction( tr( "&Open..." ), this, SLOT( slotFileOpen() ) );

    QAction* fileQuit = fileMenu->addAction( tr( "&Quit" ), this, SLOT( slotFileQuit() ) );

    mb->addMenu( fileMenu );

    QMenu* toolsMenu = new QMenu( tr( "Tools" ) );

    toolsMenu->addAction( tr( "New Item" ), this, SLOT( slotToolsNewItem() ) );
    toolsMenu->addAction( tr( "Add Item" ), this, SLOT( slotToolsAppendItem() ) );
    toolsMenu->addSeparator();
    toolsMenu->addAction( tr( "Collapse All" ), this, SLOT( slotCollapseAll() ) );
    toolsMenu->addAction( tr( "Expand All" ), this, SLOT( slotExpandAll() ) );
    
    mb->addMenu( toolsMenu );
}

void MainWindow::slotFileOpen()
{
    // TODO
}

void MainWindow::slotFileQuit()
{
    // TODO
    QApplication::instance()->quit();
}

void MainWindow::slotToolsNewItem()
{
    QModelIndex idx = m_view->selectionModel()->currentIndex();
    if ( idx.isValid() ) {
        qDebug() << idx;
        m_model->insertRows( 0, 1, m_model->index( idx.row(),0,idx.parent() ) );
    } else {
        m_model->insertRows( 0, 1, m_view->rootIndex() );
    }
}

void MainWindow::slotToolsAppendItem()
{
    QModelIndex idx = m_view->selectionModel()->currentIndex();
    if ( idx.isValid() ) {
        qDebug() << idx;
        m_model->insertRows( m_model->rowCount( idx ), 1, m_model->index( idx.row(),0,idx.parent() ) );
    } else {
        m_model->insertRows( m_model->rowCount( m_view->rootIndex() ), 1, m_view->rootIndex() );
    }
}

void MainWindow::slotCollapseAll()
{
    QTreeView* view = qobject_cast<QTreeView*>( m_view->leftView() );
    Q_ASSERT(view);
    view->collapseAll();
}

void MainWindow::slotExpandAll()
{
    QTreeView* view = qobject_cast<QTreeView*>( m_view->leftView() );
    Q_ASSERT(view);
    view->expandAll();
}

#include "mainwindow.moc"
