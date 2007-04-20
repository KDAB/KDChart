/* -*- Mode: C++ -*-
   KDChart - a multi-platform charting engine
   */

/****************************************************************************
 ** Copyright (C) 2001-2003 Klarälvdalens Datakonsult AB.  All rights reserved.
 **
 ** This file is part of the KDChart library.
 **
 ** This file may be distributed and/or modified under the terms of the
 ** GNU General Public License version 2 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.
 **
 ** Licensees holding valid commercial KDChart licenses may use this file in
 ** accordance with the KDChart Commercial License Agreement provided with
 ** the Software.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** See http://www.klaralvdalens-datakonsult.se/?page=products for
 **   information about KDChart Commercial License Agreements.
 **
 ** Contact info@klaralvdalens-datakonsult.se if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/
#ifndef __KDXMLTOOLS_H__
#define __KDXMLTOOLS_H__

#include <qdom.h>
#include <QPen>
#include <QString>
#include <QColor>
#include <QBrush>
#include <QSize>
#include <QRect>
#include <QFont>
#include <QStringList>
#include <QDateTime>
#include <QModelIndex>

namespace KDXML {
    QString penStyleToString( Qt::PenStyle style );
    QString penCapStyleToString( Qt::PenCapStyle style );
    QString penJoinStyleToString( Qt::PenJoinStyle style );

    Qt::PenStyle stringToPenStyle( const QString& style );
    Qt::PenCapStyle stringToPenCapStyle( const QString& style );
    Qt::PenJoinStyle stringToPenJoinStyle( const QString& style );
    QString brushStyleToString( Qt::BrushStyle style );
    Qt::BrushStyle stringToBrushStyle( const QString& style );

    void setBoolAttribute( QDomElement& element,
            const QString& attrName, bool value );
    void setDoubleAttribute( QDomElement& element,
            const QString& attrName, double value );

    void createNodeWithAttribute( QDomDocument& doc, QDomNode& parent,
                                  const QString& elementName,
                                  const QString& attrName,
                                  const QString& attrText );

    void createBoolNode( QDomDocument& doc, QDomNode& parent,
            const QString& elementName, bool value );
    void createOrientationNode( QDomDocument& doc, QDomNode& parent,
            const QString& elementName, Qt::Orientation value );
    void createAlignmentNode( QDomDocument& doc, QDomNode& parent,
            const QString& elementName, Qt::Alignment value );
    void createSizeNode( QDomDocument& doc, QDomNode& parent,
                         const QString& elementName, const QSize& value );
    void createSizeFNode( QDomDocument& doc, QDomNode& parent,
                          const QString& elementName, const QSizeF& value );
    void createIntNode( QDomDocument& doc, QDomNode& parent,
            const QString& elementName, int value );
    void createDoubleNode( QDomDocument& doc, QDomNode& parent,
                           const QString& elementName, double value );
    void createRealNode( QDomDocument& doc, QDomNode& parent,
                           const QString& elementName, qreal value );
    void createStringNode( QDomDocument& doc, QDomNode& parent,
                           const QString& elementName,
                           const QString& text );
    void createStringNodeIfContent( QDomDocument& doc, QDomNode& parent,
                                    const QString& elementName,
                                    const QString& text );
    void createPointFNode( QDomDocument& doc, QDomNode& parent,
                           const QString& elementName,
                           const QPointF& point );
    void createColorNode( QDomDocument& doc, QDomNode& parent,
                          const QString& elementName, const QColor& color );
    void createBrushNode( QDomDocument& doc, QDomNode& parent,
            const QString& elementName, const QBrush& brush );
    void createPixmapNode( QDomDocument& doc, QDomNode& parent,
            const QString& elementName, const QPixmap& pixmap );
    void createRectNode( QDomDocument& doc, QDomNode& parent,
            const QString& elementName, const QRect& rect );
    void createStringListNodes( QDomDocument& doc, QDomNode& parent,
            const QString& elementName,
            const QStringList* thelist ); // PCH: This is required to avoid MSVC compiler warnings and errors.
    void createFontNode( QDomDocument& doc, QDomNode& parent,
            const QString& elementName, const QFont& font );

    void createPenNode( QDomDocument& doc, QDomNode& parent,
            const QString& elementName, const QPen& pen );
    void createDateTimeNode( QDomDocument& doc, QDomNode& parent,
            const QString& elementName,
            const QDateTime& datetime );
    void createDateNode( QDomDocument& doc, QDomNode& parent,
            const QString& elementName, const QDate& date );
    void createTimeNode( QDomDocument& doc, QDomNode& parent,
            const QString& elementName, const QTime& time );
    void createModelIndexNode( QDomDocument& doc, QDomNode& parent,
                               const QString& elementName, const QModelIndex& idx );

    void createRealPairNode( QDomDocument& doc, QDomNode& parent,
                             const QString& elementName, const QPair<qreal, qreal> & pair );

    void createPositionBooleansNode(
            QDomDocument& doc, QDomNode& parent, const QString& elementName,
            bool unknown, bool center,
            bool northWest, bool north, bool northEast,
            bool east, bool southEast, bool south, bool southWest, bool west,
            bool floating );

    bool findStringAttribute( const QDomElement& e, const QString & name, QString& attr );
    bool findIntAttribute(    const QDomElement& e, const QString & name, int&  attr );
    bool findDoubleAttribute( const QDomElement& e, const QString & name, double& attr );
    bool findBoolAttribute(   const QDomElement& e, const QString & name, bool& attr );

    bool readIntNode( const QDomElement& element, int& value );
    bool readStringNode( const QDomElement& element, QString& value );
    bool readStringListNode( const QDomElement& element, QStringList& value );
    bool readDoubleNode( const QDomElement& element, double& value );
    bool readRealNode( const QDomElement& element, qreal& value );
    bool readBoolNode( const QDomElement& element, bool& value );
    bool readOrientationNode( const QDomElement& element, Qt::Orientation& value );
    bool readAlignmentNode(const QDomElement& element, Qt::Alignment& value );
    bool readSizeNode( const QDomElement& element, QSize& value );
    bool readSizeFNode( const QDomElement& element, QSizeF& value );
    bool readPointFNode( const QDomElement& element, QPointF& value );
    bool readColorNode( const QDomElement& element, QColor& value );
    bool readBrushNode( const QDomElement& element, QBrush& brush );
    bool readPixmapNode( const QDomElement& element, QPixmap& pixmap );
    bool readRectNode( const QDomElement& element, QRect& value );
    bool readFontNode( const QDomElement& element, QFont& font );
    bool readPenNode( const QDomElement& element, QPen& pen );
    bool readDateTimeNode( const QDomElement& element, QDateTime& datetime );
    bool readDateNode( const QDomElement& element, QDate& date );
    bool readTimeNode( const QDomElement& element, QTime& time );
    bool readRealPairNode( const QDomElement& element, QPair<qreal, qreal> & pair );
    bool readPositionBooleansNode(
            const QDomElement& element,
            bool& unknown, bool& center,
            bool& northWest, bool& north, bool& northEast,
            bool& east, bool& southEast, bool& south, bool& southWest, bool& west,
            bool& floating );
    bool readModelIndexNode(const QDomElement& element,
                            const QAbstractItemModel& model,
                            QModelIndex& idx );
    bool readModelIndexNode(const QDomElement& element,
                            bool& isValid,
                            int& column,
                            int& row );
}
#endif
