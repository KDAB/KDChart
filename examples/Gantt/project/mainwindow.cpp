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
#include <QBrush>
#include <QPainter>

#include <KDGanttGlobal>
#include <KDGanttView>
#include <KDGanttItemDelegate>
#include <KDGanttDateTimeGrid>
#include <KDGanttStyleOptionGanttItem>
#include <KDGanttConstraintModel>

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

///////////////////////////////////////////////////////////////////////////////
// Provide custom background and foreground
///////////////////////////////////////////////////////////////////////////////

class DateTimeGrid : public KDGantt::DateTimeGrid
{
public:
    DateTimeGrid(QObject* parent=0) { 
        setParent(parent);
    }
    ~DateTimeGrid() { }

    virtual void paintUserDefinedHeader(QPainter* painter, const QRectF& headerRect, const QRectF& exposedRect, qreal offset, const KDGantt::DateTimeScaleFormatter* formatter, QWidget* widget = 0);
    virtual void drawBackground(QPainter* painter, const QRectF& rect);
    virtual void drawForeground(QPainter* painter, const QRectF& rect);
};

void DateTimeGrid::drawBackground(QPainter* painter, const QRectF& rect)
{
    QBrush brush(Qt::Dense5Pattern);
    brush.setColor(Qt::lightGray);

    QRectF r = computeRect(QDateTime::currentDateTime(),
                           QDateTime::currentDateTime().addDays(2), 
                           rect);
    painter->fillRect(r, brush);
}

void DateTimeGrid::drawForeground(QPainter* painter, const QRectF& rect)
{
    painter->save();

    QRectF r = computeRect(QDateTime::currentDateTime(),
                           QDateTime::currentDateTime().addDays(2), 
                           rect);

    static QString text("Holiday");
    QFont font = painter->font();
    font.setPixelSize(r.width()/5);

    QFontMetrics fm(font);
    int width = fm.width(text);
    int height = fm.boundingRect(text).height();

    painter->translate(r.center());
    painter->translate(-width/2, height/2);
    painter->setFont(font);
    painter->drawText(0, 0, text);

    painter->restore();
}

void DateTimeGrid::paintUserDefinedHeader( QPainter* painter, const QRectF& headerRect, const QRectF& exposedRect, qreal offset, const KDGantt::DateTimeScaleFormatter* formatter, QWidget* widget)
{
    const QStyle* const style = widget ? widget->style() : QApplication::style();

    QDateTime dt = formatter->currentRangeBegin( mapToDateTime( offset + exposedRect.left() ) ).toUTC();
    qreal x = mapFromDateTime( dt );

    while( x < exposedRect.right() + offset ) {
        const QDateTime next = formatter->nextRangeBegin( dt );
        const qreal nextx = mapFromDateTime( next );

        QStyleOptionHeader opt;
        if ( widget ) opt.init( widget );
        opt.rect = QRectF( x - offset+1, headerRect.top(), qMax( 1., nextx-x-1 ), headerRect.height() ).toAlignedRect();
        opt.textAlignment = formatter->alignment();
        opt.text = formatter->text( dt );

        // use white text on black background
        opt.palette.setColor(QPalette::Button, QColor("black"));
        opt.palette.setColor(QPalette::ButtonText, QColor("white"));

        style->drawControl( QStyle::CE_Header, &opt, painter, widget );

        dt = next;
        x = nextx;
    }
}

MainWindow::MainWindow( QWidget* parent )
    : QMainWindow( parent ),
      m_model( new ProjectModel( this ) ),
      m_view( new KDGantt::View )
{
    m_view->setModel( m_model );
    m_view->setSelectionModel( new QItemSelectionModel(m_model));

    // slotToolsNewItem();
    m_view->leftView()->setItemDelegateForColumn( 1, new MyItemDelegate( this ) );
    m_view->setGrid(new DateTimeGrid(this));

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
    QMenu *alignMenu = toolsMenu->addMenu( tr( "Align" ) );
    alignMenu->addAction( tr( "Left" ), this, SLOT( slotAlignLeft() ) );
    alignMenu->addAction( tr( "Center" ), this, SLOT( slotAlignCenter() ) );
    alignMenu->addAction( tr( "Right" ), this, SLOT( slotAlignRight() ) );
    alignMenu->addAction( tr( "Hidden" ), this, SLOT( slotAlignHidden() ) );
    toolsMenu->addSeparator();
    toolsMenu->addAction( tr( "Collapse All" ), this, SLOT( slotCollapseAll() ) );
    toolsMenu->addAction( tr( "Expand All" ), this, SLOT( slotExpandAll() ) );
    
    mb->addMenu( toolsMenu );
    
    /*
    slotToolsNewItem();
    slotToolsNewItem();
    slotToolsNewItem();
    for(int i = 0; i < 3; ++i) {
        m_model->setData(m_model->index(i,2,QModelIndex()), qVariantFromValue(QDateTime::currentDateTime().addDays(i)), KDGantt::StartTimeRole);
        m_model->setData(m_model->index(i,3,QModelIndex()), qVariantFromValue(QDateTime::currentDateTime().addDays(i+1)), KDGantt::EndTimeRole);
    }
    m_view->setConstraintModel(new KDGantt::ConstraintModel(m_view));
    m_view->constraintModel()->addConstraint(KDGantt::Constraint(m_model->index(0,0,QModelIndex()),m_model->index(1,0,QModelIndex())));
    m_view->constraintModel()->addConstraint(KDGantt::Constraint(m_model->index(1,0,QModelIndex()),m_model->index(2,0,QModelIndex())));
    */
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
    // don't use the treeview's collapseAll/expandAll methods but use the one provided by the
    // view cause that one will take care to update everyt6hing as needed.
    //QTreeView* view = qobject_cast<QTreeView*>( m_view->leftView() );
    //view->collapseAll();
    
    QModelIndex idx = m_view->selectionModel()->currentIndex();
    if ( idx.isValid() )
        m_view->collapseAll();
}

void MainWindow::slotExpandAll()
{
    // don't use the treeview's collapseAll/expandAll methods but use the one provided by the
    // view cause that one will take care to update everyt6hing as needed.
    //QTreeView* view = qobject_cast<QTreeView*>( m_view->leftView() );
    //view->expandAll();
    
    QModelIndex idx = m_view->selectionModel()->currentIndex();
    if ( idx.isValid() )
        m_view->expandAll();
}

void MainWindow::slotAlignLeft()
{
    QModelIndex idx = m_view->selectionModel()->currentIndex();
    if ( idx.isValid() ) {
        m_model->setData( idx, KDGantt::StyleOptionGanttItem::Left, KDGantt::TextPositionRole );
    }
}

void MainWindow::slotAlignCenter()
{
    QModelIndex idx = m_view->selectionModel()->currentIndex();
    if ( idx.isValid() ) {
        m_model->setData( idx, KDGantt::StyleOptionGanttItem::Center, KDGantt::TextPositionRole );
    }
}

void MainWindow::slotAlignRight()
{
    QModelIndex idx = m_view->selectionModel()->currentIndex();
    if ( idx.isValid() ) {
        m_model->setData( idx, KDGantt::StyleOptionGanttItem::Right, KDGantt::TextPositionRole );
    }
}

void MainWindow::slotAlignHidden()
{
    QModelIndex idx = m_view->selectionModel()->currentIndex();
    if ( idx.isValid() ) {
        m_model->setData( idx, KDGantt::StyleOptionGanttItem::Hidden, KDGantt::TextPositionRole );
    }
}
    
#include "mainwindow.moc"