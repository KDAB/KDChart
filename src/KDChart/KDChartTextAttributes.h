/****************************************************************************
** Copyright (C) 2001-2016 Klaralvdalens Datakonsult AB.  All rights reserved.
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

#ifndef KDCHARTTEXTATTRIBUTES_H
#define KDCHARTTEXTATTRIBUTES_H

#include <QDebug>
#include <QMetaType>
#include "KDChartGlobal.h"
#include "KDChartMeasure.h"

QT_BEGIN_NAMESPACE
class QPen;
class QFont;
class QTextDocument;
QT_END_NAMESPACE

namespace KDChart {

    /**
     * \brief A set of text attributes.
     *
     * TextAttributes encapsulates settings that have to do with
     * text. This includes font, fontsize, color, whether the text
     * is rotated, etc
     */
class KDCHART_EXPORT TextAttributes
{
public:
  TextAttributes();
  TextAttributes( const TextAttributes& );
  TextAttributes &operator= ( const TextAttributes& );
  bool operator==( const TextAttributes& ) const;
  inline bool operator!=( const TextAttributes& other ) const
  { return !operator==(other); }

  ~TextAttributes();

  /**
   * Set whether the text is to be rendered at all.
   * \param visible Whether the text is visible.
   */
  void setVisible( bool visible );

  /**
   * \return Whether the text is visible.
   */
  bool isVisible() const;

  /**
   * Set the font to be used for rendering the text.
   *
   * \note All of the font's attributes will be used - except of its size!
   * To specify the size please use setFontSize (or setMinimalFontSize, resp.)
   *
   * \param font The font to use.
   *
   * \sa setFontSize, setMinimalFontSize
   */
  void setFont( const QFont& font );

  /**
   * \return The font that is used for rendering text.
   */
  QFont font() const;

  /**
   * Set the size of the font used for rendering text.
   * \param measure The measure to use.
   * \see Measure
   */
  void setFontSize( const Measure & measure );

  /**
   * \return The measure used for the font size.
   */
  Measure fontSize() const;

  /**
   * Set the minimal size of the font used for rendering text.
   * \param measure The measure to use.
   * \see Measure
   */
  void setMinimalFontSize( const Measure & measure );

  /**
   * \return The measure used for the minimal font size.
   */
  Measure minimalFontSize() const;

  /**
   * \brief Returns the font size that is used at drawing time.
   *
   * This method is called at drawing time. It returns the
   * font size as it is used for rendering text, taking into account
   * any measures that were set via setFontSize and/or setMinimalFontSize.
   */
  qreal calculatedFontSize( const QSizeF &referenceSize,
                            KDChartEnums::MeasureOrientation autoReferenceOrientation ) const;

  /**
   * \brief Returns the font size that is used at drawing time.
   *
   * This method is called at drawing time. It returns the
   * font size as it is used for rendering text, taking into account
   * any measures that were set via setFontSize and/or setMinimalFontSize.
   */
#if QT_VERSION < 0x040400 || defined(Q_COMPILER_MANGLES_RETURN_TYPE)
  const qreal calculatedFontSize(
#else
  qreal calculatedFontSize(
#endif
        const QObject*                   autoReferenceArea,
        KDChartEnums::MeasureOrientation autoReferenceOrientation ) const;

  /**
   * \brief Returns the font in the size that is used at drawing time.
   *
   * This method is called at drawing time. It returns the
   * font as it is used for rendering text, taking into account
   * any measures that were set via setFontSize and/or setMinimalFontSize.
   */
  const QFont calculatedFont(
        const QObject*                   autoReferenceArea,
        KDChartEnums::MeasureOrientation autoReferenceOrientation ) const;

  /**
   * \return Whether the text has an absolute font size set.
   */
  bool hasAbsoluteFontSize() const;

  /**
   * Set whether the text should be automatically rotated as
   * needed when space is tight.
   * \param autoRotate Whether text should be automatically rotated.
   */
  void setAutoRotate( bool autoRotate );

  /**
   * \return Whether text is automatically rotated when space is
   * tight.
   */
  bool autoRotate() const;

  /**
   * Set whether the text should automatically be shrunk if
   * space is tight.
   * \param autoShrink Whether text should be auto-shrunk.
   */
  void setAutoShrink( bool autoShrink );

  /**
   * \return Whether text is automatically shrunk if space is
   * tight.
   */
  bool autoShrink() const;

  /**
   * Set the rotation angle to use for the text.
   *
   * \note For axis titles the rotation angle can be set to one of the
   * following angles: 0, 90, 180, 270
   * Any other values specified will be replaced by the next smaller
   * one of the allowed values, so no matter what you set the rotation
   * will always be one of these four values.
   *
   * \param rotation The rotation angle.
   */
  void setRotation( int rotation );
  void resetRotation();
  bool hasRotation() const;

  /**
   * \return The rotation angle used for rendering the text.
   */
  int rotation() const;

  /**
   * Set the pen to use for rendering the text.
   * \param pen The pen to use.
   */
  void setPen( const QPen& pen );

  /**
   * \return The pen used for rendering the text.
   */
  QPen pen() const;

  /**
   * \return The document used for the drawing the text or NULL if the
   * default document is used.
   */
  QTextDocument* textDocument() const;

  /**
   * Sets the document to use for the text. The previous document is deleted.
   */
  void setTextDocument(QTextDocument* layout);

  // FIXME KDChartEnums::TextLayoutPolicy?

private:
  KDCHART_DECLARE_PRIVATE_BASE_VALUE( TextAttributes )

}; // End of class TextAttributes

}

#if !defined(QT_NO_DEBUG_STREAM)
KDCHART_EXPORT QDebug operator<<(QDebug, const KDChart::TextAttributes& );
#endif /* QT_NO_DEBUG_STREAM */

KDCHART_DECLARE_SWAP_SPECIALISATION( KDChart::TextAttributes )

QT_BEGIN_NAMESPACE
Q_DECLARE_TYPEINFO( KDChart::TextAttributes, Q_MOVABLE_TYPE );
QT_END_NAMESPACE
Q_DECLARE_METATYPE( KDChart::TextAttributes )

#endif // KDCHARTTEXTATTRIBUTES_H
