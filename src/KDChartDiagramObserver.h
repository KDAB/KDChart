#ifndef __KDCHARTDIAGRAMOBSERVER_H_
#define __KDCHARTDIAGRAMOBSERVER_H_

#include "KDChartGlobal.h"

#include <QObject>
#include <QPointer>
#include <QModelIndex>

class QAbstractItemModel;

namespace KDChart {

    class AbstractDiagram;

    /**
     * \brief A DiagramObserver watches the associated diagram for
     * changes and deletion and emits corresponsing signals.
     */
    class KDCHART_EXPORT DiagramObserver : public QObject
    {
        Q_OBJECT
    public:
       /**
         * Constructs a new observer observing the given diagram.
         */
        explicit DiagramObserver( AbstractDiagram * diagram, QObject* parent = 0 );
        ~DiagramObserver();

        const AbstractDiagram* diagram() const;
        AbstractDiagram* diagram();

    Q_SIGNALS:
        /** This signal is emitted immediately before the diagram is
          * being destroyed. */
        void diagramDestroyed( AbstractDiagram* diagram );
        /** Emitted when a diagram is being destroyed, but before its data is invalidated **/
        void diagramAboutToBeDestroyed( AbstractDiagram* diagram );
        /** This signal is emitted whenever the data of the diagram changes. */
        void diagramDataChanged( AbstractDiagram* diagram );
        /** This signal is emitted whenever any of the data of the diagram was set (un)hidden. */
        void diagramDataHidden( AbstractDiagram* diagram );
        /** This signal is emitted whenever the attributes of the diagram change. */
        void diagramAttributesChanged( AbstractDiagram* diagram );

    private Q_SLOTS:
        void slotDestroyed(QObject*);
        void slotAboutToBeDestroyed();
        void slotHeaderDataChanged(Qt::Orientation,int,int);
        void slotDataChanged(QModelIndex,QModelIndex);
        void slotDataChanged();
        void slotDataHidden();
        void slotAttributesChanged();
        void slotAttributesChanged(QModelIndex,QModelIndex);
        void slotModelsChanged();

    private:
        void init();

        AbstractDiagram*    m_diagram;
        QPointer<QAbstractItemModel> m_model;
        QPointer<QAbstractItemModel> m_attributesmodel;
   };
}

#endif // KDChartDiagramObserver_H
