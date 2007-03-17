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
#include "KDXMLTools.h"

#include <QBuffer>
#include <QByteArray>
#include <QImage>
#include <QImageWriter>

#include <zlib.h>

namespace KDXML {

    void createBoolNode( QDomDocument& doc, QDomNode& parent,
            const QString& elementName, bool value )
    {
        QDomElement newElement =
            doc.createElement( elementName );
        parent.appendChild( newElement );
        QDomText elementContent =
            doc.createTextNode( value ? "true" : "false" );
        newElement.appendChild( elementContent );
    }


    void createOrientationNode( QDomDocument& doc, QDomNode& parent,
            const QString& elementName, Qt::Orientation value )
    {
        QDomElement newElement =
            doc.createElement( elementName );
        parent.appendChild( newElement );
        QDomText elementContent =
            doc.createTextNode( (Qt::Vertical == value) ? "vertical" : "horizontal" );
        newElement.appendChild( elementContent );
    }



    void createSizeNode( QDomDocument& doc, QDomNode& parent,
            const QString& elementName, const QSize& value )
    {
        QDomElement newElement =
            doc.createElement( elementName );
        parent.appendChild( newElement );
        newElement.setAttribute( "Width", value.width() );
        newElement.setAttribute( "Height", value.height() );
    }


    void createSizeFNode( QDomDocument& doc, QDomNode& parent,
                         const QString& elementName, const QSizeF& value )
    {
        QDomElement newElement =
                doc.createElement( elementName );
        parent.appendChild( newElement );
        newElement.setAttribute( "Width", value.width() );
        newElement.setAttribute( "Height", value.height() );
    }


    void createIntNode( QDomDocument& doc, QDomNode& parent,
            const QString& elementName, int value )
    {
        QDomElement newElement =
            doc.createElement( elementName );
        parent.appendChild( newElement );
        QDomText elementContent =
            doc.createTextNode( QString::number( value ) );
        newElement.appendChild( elementContent );
    }


    void createDoubleNode( QDomDocument& doc, QDomNode& parent,
                           const QString& elementName, double value )
    {
        QDomElement newElement =
                doc.createElement( elementName );
        parent.appendChild( newElement );
        QDomText elementContent =
                doc.createTextNode( QString::number( value ) );
        newElement.appendChild( elementContent );
    }


    void createRealNode( QDomDocument& doc, QDomNode& parent,
                         const QString& elementName, qreal value )
    {
        QDomElement newElement =
                doc.createElement( elementName );
        parent.appendChild( newElement );
        QDomText elementContent =
                doc.createTextNode( QString::number( value ) );
        newElement.appendChild( elementContent );
    }


    void createStringNode( QDomDocument& doc, QDomNode& parent,
            const QString& elementName,
            const QString& text )
    {
        QDomElement newElement =
            doc.createElement( elementName );
        parent.appendChild( newElement );
        QDomText elementContent =
            doc.createTextNode( text );
        newElement.appendChild( elementContent );
    }

    void createStringNodeIfContent( QDomDocument& doc, QDomNode& parent,
                           const QString& elementName,
                           const QString& text )
    {
        if( ! text.isEmpty() )
            createStringNode( doc, parent, elementName, text );
    }

    void createColorNode( QDomDocument& doc, QDomNode& parent,
            const QString& elementName, const QColor& color )
    {
        QDomElement colorElement = doc.createElement( elementName );
        parent.appendChild( colorElement );
        colorElement.setAttribute( "Red",
                QString::number( color.red() ) );
        colorElement.setAttribute( "Green",
                QString::number( color.green() ) );
        colorElement.setAttribute( "Blue",
                QString::number( color.blue() ) );
    }


    void createBrushNode( QDomDocument& doc, QDomNode& parent,
            const QString& elementName, const QBrush& brush )

    {
        QDomElement brushElement = doc.createElement( elementName );
        parent.appendChild( brushElement );
        createColorNode( doc, brushElement, "Color", brush.color() );
        createStringNode( doc, brushElement, "Style",
                KDXML::brushStyleToString( brush.style() ) );
        QPixmap pix( brush.texture() );
        if( ! pix.isNull() )
            createPixmapNode( doc, brushElement, "Pixmap", pix );
    }


    void createPixmapNode( QDomDocument& doc, QDomNode& parent,
            const QString& elementName, const QPixmap& pixmap )
    {
        QDomElement pixmapElement = doc.createElement( elementName );
        parent.appendChild( pixmapElement );

        createStringNode( doc, pixmapElement, "Format", "XPM.GZ" );
        if( pixmap.isNull() ){
            // we store an empty image without any data
            createIntNode( doc, pixmapElement, "Length", 0 );
        }else{
            // Convert the pixmap to an image, save that image to an in-memory
            // XPM representation and compress this representation. This
            // conforms to the file format Qt Designer uses.
            QByteArray ba;
            QBuffer buffer( &ba );
            buffer.open( QIODevice::WriteOnly );
            QImageWriter imgio( &buffer, "XPM" );
            QImage image( pixmap.toImage() );
            imgio.write( image );
            buffer.close();
            ulong len = ba.size() * 2;
            QByteArray bazip;
            bazip.reserve( len );
            ::compress(  (uchar*) bazip.data(), &len, (uchar*) ba.data(), ba.size() );
            QString dataString;
            static const char hexchars[] = "0123456789abcdef";
            for ( int i = 0; i < (int)len; ++i ) {
                uchar c = (uchar) bazip[i];
                dataString += hexchars[c >> 4];
                dataString += hexchars[c & 0x0f];
            }
            createIntNode( doc, pixmapElement, "Length", ba.size() );
            createStringNode( doc, pixmapElement, "Data", dataString );
        }
    }


    void createRectNode( QDomDocument& doc, QDomNode& parent,
            const QString& elementName, const QRect& rect )
    {
        QDomElement rectElement = doc.createElement( elementName );
        parent.appendChild( rectElement );
        QDomElement xElement = doc.createElement( "X" );
        rectElement.appendChild( xElement );
        QDomText xContent = doc.createTextNode( QString::number( rect.x() ) );
        xElement.appendChild( xContent );
        QDomElement yElement = doc.createElement( "Y" );
        rectElement.appendChild( yElement );
        QDomText yContent = doc.createTextNode( QString::number( rect.y() ) );
        yElement.appendChild( yContent );
        QDomElement widthElement = doc.createElement( "Width" );
        rectElement.appendChild( widthElement );
        QDomText widthContent = doc.createTextNode( QString::number( rect.width() ) );
        widthElement.appendChild( widthContent );
        QDomElement heightElement = doc.createElement( "Height" );
        rectElement.appendChild( heightElement );
        QDomText heightContent = doc.createTextNode( QString::number( rect.height() ) );
        heightElement.appendChild( heightContent );
    }


    void createStringListNodes( QDomDocument& doc, QDomNode& parent,
            const QString& elementName,
            const QStringList* list )
    {
        if( !list )
            return;

        for( QStringList::ConstIterator it = list->begin();
                it != list->end(); ++it ) {
            QDomElement element = doc.createElement( elementName );
            parent.appendChild( element );
            QDomText elementContent = doc.createTextNode( *it );
            element.appendChild( elementContent );
        }
    }


    void createFontNode( QDomDocument& doc, QDomNode& parent,
            const QString& elementName, const QFont& font )
    {
        QDomElement fontElement = doc.createElement( elementName );
        parent.appendChild( fontElement );
        createStringNode( doc, fontElement, "Family", font.family() );
        createIntNode( doc, fontElement, "PointSize", font.pointSize() );
        createIntNode( doc, fontElement, "Weight", font.weight() );
        createBoolNode( doc, fontElement, "Italic", font.italic() );
    }


    void createPenNode( QDomDocument& doc, QDomNode& parent,
            const QString& elementName, const QPen& pen )
    {
        QDomElement penElement = doc.createElement( elementName );
        parent.appendChild( penElement );
        createIntNode( doc, penElement, "Width", pen.width() );
        createColorNode( doc, penElement, "Color", pen.color() );
        createStringNode( doc, penElement, "Style", penStyleToString( pen.style() ) );
    }


    void createDateTimeNode( QDomDocument& doc, QDomNode& parent,
            const QString& elementName, 
            const QDateTime& datetime )
    {
        QDomElement dateTimeElement = doc.createElement( elementName );
        parent.appendChild( dateTimeElement );
        createDateNode( doc, dateTimeElement, "Date", datetime.date() );
        createTimeNode( doc, dateTimeElement, "Time", datetime.time() );
    }


    void createDateNode( QDomDocument& doc, QDomNode& parent,
            const QString& elementName, const QDate& date )
    {
        QDomElement dateElement = doc.createElement( elementName );
        parent.appendChild( dateElement );
        dateElement.setAttribute( "Year", QString::number( date.year() ) );
        dateElement.setAttribute( "Month", QString::number( date.month() ) );
        dateElement.setAttribute( "Day", QString::number( date.day() ) );
    }


    void createTimeNode( QDomDocument& doc, QDomNode& parent,
            const QString& elementName, const QTime& time )
    {
        QDomElement timeElement = doc.createElement( elementName );
        parent.appendChild( timeElement );
        timeElement.setAttribute( "Hour",
                QString::number( time.hour() ) );
        timeElement.setAttribute( "Minute",
                QString::number( time.minute() ) );
        timeElement.setAttribute( "Second",
                QString::number( time.second() ) );
        timeElement.setAttribute( "Millisecond",
                QString::number( time.msec() ) );
    }


    QString penStyleToString( Qt::PenStyle style )
    {
        switch( style ) {
            case Qt::NoPen:
                return "NoPen";
            case Qt::SolidLine:
                return "SolidLine";
            case Qt::DashLine:
                return "DashLine";
            case Qt::DotLine:
                return "DotLine";
            case Qt::DashDotLine:
                return "DashDotLine";
            case Qt::DashDotDotLine:
                return "DashDotDotLine";
            default: // should not happen
                return "SolidLine";
        }
    }



    QString brushStyleToString( Qt::BrushStyle style )
    {
        // PENDING(kalle) Support custom patterns
        switch( style ) {
            case Qt::NoBrush:
                return "NoBrush";
            case Qt::SolidPattern:
                return "SolidPattern";
            case Qt::Dense1Pattern:
                return "Dense1Pattern";
            case Qt::Dense2Pattern:
                return "Dense2Pattern";
            case Qt::Dense3Pattern:
                return "Dense3Pattern";
            case Qt::Dense4Pattern:
                return "Dense4Pattern";
            case Qt::Dense5Pattern:
                return "Dense5Pattern";
            case Qt::Dense6Pattern:
                return "Dense6Pattern";
            case Qt::Dense7Pattern:
                return "Dense7Pattern";
            case Qt::HorPattern:
                return "HorPattern";
            case Qt::VerPattern:
                return "VerPattern";
            case Qt::CrossPattern:
                return "CrossPattern";
            case Qt::BDiagPattern:
                return "BDiagPattern";
            case Qt::FDiagPattern:
                return "FDiagPattern";
            case Qt::DiagCrossPattern:
                return "DiagCrossPattern";
            default: // should not happen (but can for a custom pattern)
                return "SolidPattern";
        }
    }


    bool findIntAttribute( const QDomElement& e, const QString & name, int& attr )
    {
        bool bOK = false;
        if( e.hasAttribute( name ) ){
            int val = e.attribute( name ).toInt( &bOK );
            if( bOK )
                attr = val;
        }
        return bOK;
    }

    bool readStringNode( const QDomElement& element, QString& value )
    {
        value = element.text();
        return true;
    }


    bool readIntNode( const QDomElement& element, int& value )
    {
        bool ok = false;
        int temp = element.text().toInt( &ok );
        if( ok )
            value = temp;
        return ok;
    }


    bool readDoubleNode( const QDomElement& element, double& value )
    {
        bool ok = false;
        double temp = element.text().toDouble( &ok );
        if( ok )
            value = temp;
        return ok;
    }


    bool readRealNode( const QDomElement& element, qreal& value )
    {
        bool ok = false;
        qreal temp = element.text().toDouble( &ok );
        if( ok )
            value = temp;
        return ok;
    }


    bool readBoolNode( const QDomElement& element, bool& value )
    {
        if( element.text() == "true" ) {
            value = true;
            return true;
        } else if( element.text() == "false" ) {
            value = false;
            return true;
        } else
            return false;
    }


    bool readOrientationNode( const QDomElement& element, Qt::Orientation& value )
    {
        if( element.text() == "vertical" ) {
            value = Qt::Vertical;
            return true;
        } else if( element.text() == "horizontal" ) {
            value = Qt::Horizontal;
            return true;
        } else
            return false;
    }


    bool readSizeNode( const QDomElement& element, QSize& value )
    {
        bool ok = false;
        int width, height;
        if( element.hasAttribute(      "Width" ) ) {
            width = element.attribute( "Width" ).toInt( &ok );
            if( ok && element.hasAttribute( "Height" ) ) {
                height = element.attribute( "Height" ).toInt( &ok );
                if( ok ){
                    value.setWidth(  width );
                    value.setHeight( height );
                }
            }
        }
        return ok;
    }


    bool readSizeFNode( const QDomElement& element, QSizeF& value )
    {
        bool ok = false;
        qreal width, height;
        if( element.hasAttribute(      "Width" ) ) {
            width = element.attribute( "Width" ).toDouble( &ok );
            if( ok && element.hasAttribute( "Height" ) ) {
                height = element.attribute( "Height" ).toDouble( &ok );
                if( ok ){
                    value.setWidth(  width );
                    value.setHeight( height );
                }
            }
        }
        return ok;
    }


    bool readColorNode( const QDomElement& element, QColor& value )
    {
        bool ok = true;
        int red=0, green=0, blue=0;
        if( element.hasAttribute( "Red" ) ) {
            bool redOk = false;
            red = element.attribute( "Red" ).toInt( &redOk );
            ok = ok & redOk;
        }
        if( element.hasAttribute( "Green" ) ) {
            bool greenOk = false;
            green = element.attribute( "Green" ).toInt( &greenOk );
            ok = ok & greenOk;
        }
        if( element.hasAttribute( "Blue" ) ) {
            bool blueOk = false;
            blue = element.attribute( "Blue" ).toInt( &blueOk );
            ok = ok & blueOk;
        }

        if( ok )
            value.setRgb( red, green, blue );

        return ok;
    }


    bool readBrushNode( const QDomElement& element, QBrush& brush )
    {
        bool ok = true;
        QColor tempColor;
        Qt::BrushStyle tempStyle=Qt::SolidPattern;
        QPixmap tempPixmap;
        QDomNode node = element.firstChild();
        while( !node.isNull() ) {
            QDomElement element = node.toElement();
            if( !element.isNull() ) { // was really an element
                QString tagName = element.tagName();
                if( tagName == "Color" ) {
                    ok = ok & readColorNode( element, tempColor );
                } else if( tagName == "Style" ) {
                    QString value;
                    ok = ok & readStringNode( element, value );
                    tempStyle = stringToBrushStyle( value );
                } else if( tagName == "Pixmap" ) {
                    ok = ok & readPixmapNode( element, tempPixmap );
                } else {
                    qDebug( "Unknown tag in brush" );
                }
            }
            node = node.nextSibling();
        }

        if( ok ) {
            brush.setColor( tempColor );
            brush.setStyle( tempStyle );
            if( !tempPixmap.isNull() )
                brush.setTexture( tempPixmap );
        }

        return ok;
    }


    bool readPixmapNode( const QDomElement& element, QPixmap& pixmap )
    {
        bool ok = true;
        unsigned long tempLength;
        QString tempData;
        QDomNode node = element.firstChild();
        while( !node.isNull() ) {
            QDomElement element = node.toElement();
            if( !element.isNull() ) { // was really an element
                QString tagName = element.tagName();
                if( tagName == "Format" ) {
                    QString formatName;
                    ok = ok & readStringNode( element, formatName );
#ifndef NDEBUG
                    if( formatName != "XPM.GZ" )
                        qDebug( "Unsupported pixmap format in XML file" );
#endif
                } else if( tagName == "Length" ) {
                    int itempLength;
                    ok = ok & readIntNode( element, itempLength );
                    tempLength = itempLength;
                } else if( tagName == "Data" ) {
                    ok = ok & readStringNode( element, tempData );
                } else {
                    qDebug( "Unknown tag in Pixmap" );
                }
            }
            node = node.nextSibling();
        }

        if( ok ) {
            if( 0 < tempLength ) {
                // Decode the image file format in the same way Qt Designer does.
                char *ba = new char[ tempData.length() / 2 ];
                for ( int i = 0; i < (int)tempData.length() / 2; ++i ) {
                    char h = tempData[ 2 * i ].toLatin1();
                    char l = tempData[ 2 * i  + 1 ].toLatin1();
                    uchar r = 0;
                    if ( h <= '9' )
                        r += h - '0';
                    else
                        r += h - 'a' + 10;
                    r = r << 4;
                    if ( l <= '9' )
                        r += l - '0';
                    else
                        r += l - 'a' + 10;
                    ba[ i ] = r;
                }

                if( tempData.length() * 5 > tempLength )
                    tempLength = tempData.length() * 5;
                QByteArray baunzip;
                baunzip.reserve( tempLength );
                ::uncompress( (uchar*) baunzip.data(), &tempLength,
                        (uchar*) ba, tempData.length()/2 );
                QImage image;
                image.loadFromData( (const uchar*)baunzip.data(), tempLength, "XPM" );

                if( image.isNull() )
                    pixmap = QPixmap(); // This is _not_ an error, we just read a NULL pixmap!
                else
                    pixmap = QPixmap::fromImage( image, 0 );
            } else
                pixmap = QPixmap(); // This is _not_ an error, we just read a empty pixmap!
        }

        return ok;
    }


    bool readPenNode( const QDomElement& element, QPen& pen )
    {
        bool ok = true;
        int tempWidth;
        QColor tempColor;
        Qt::PenStyle tempStyle=Qt::SolidLine;
        QDomNode node = element.firstChild();
        while( !node.isNull() ) {
            QDomElement element = node.toElement();
            if( !element.isNull() ) { // was really an element
                QString tagName = element.tagName();
                if( tagName == "Width" ) {
                    ok = ok & readIntNode( element, tempWidth );
                } else if( tagName == "Color" ) {
                    ok = ok & readColorNode( element, tempColor );
                } else if( tagName == "Style" ) {
                    QString value;
                    ok = ok & readStringNode( element, value );
                    tempStyle = stringToPenStyle( value );
                } else {
                    qDebug( "Unknown tag in brush" );
                }
            }
            node = node.nextSibling();
        }

        if( ok ) {
            pen.setWidth( tempWidth );
            pen.setColor( tempColor );
            pen.setStyle( tempStyle );
        }

        return ok;
    }

    bool readFontNode( const QDomElement& element, QFont& font )
    {
        bool ok = true;
        QString family;
        int pointSize, weight;
        bool italic;
        int charSet;
        QDomNode node = element.firstChild();
        while( !node.isNull() ) {
            QDomElement element = node.toElement();
            if( !element.isNull() ) { // was really an element
                QString tagName = element.tagName();
                if( tagName == "Family" ) {
                    ok = ok & readStringNode( element, family );
                } else if( tagName == "PointSize" ) {
                    ok = ok & readIntNode( element, pointSize );
                } else if( tagName == "Weight" ) {
                    ok = ok & readIntNode( element, weight );
                } else if( tagName == "Italic" ) {
                    ok = ok & readBoolNode( element, italic );
                } else if( tagName == "CharSet" ) {
                    ok = ok & readIntNode( element, charSet );
                } else {
                    qDebug( "Unknown tag in color map" );
                }
            }
            node = node.nextSibling();
        }

        if( ok ) {
            font.setFamily( family );
            font.setPointSize( pointSize );
            font.setWeight( weight );
            font.setItalic( italic );
        }

        return ok;
    }

    bool readRectNode( const QDomElement& element, QRect& value )
    {
        bool ok = true;
        int width, height, x, y;
        QDomNode node = element.firstChild();
        while( !node.isNull() ) {
            QDomElement element = node.toElement();
            if( !element.isNull() ) { // was really an element
                QString tagName = element.tagName();
                if( tagName == "Width" ) {
                    ok = ok & readIntNode( element, width );
                } else if( tagName == "Height" ) {
                    ok = ok & readIntNode( element, height );
                } else if( tagName == "X" ) {
                    ok = ok & readIntNode( element, x );
                } else if( tagName == "Y" ) {
                    ok = ok & readIntNode( element, y );
                } else {
                    qDebug( "Unknown tag in rect" );
                }
            }
            node = node.nextSibling();
        }

        if( ok ) {
            value.setX( x );
            value.setY( y );
            value.setWidth( width );
            value.setHeight( height );
        }

        return ok;
    }



    bool readDateTimeNode( const QDomElement& element, QDateTime& datetime )
    {
        bool ok = true;
        QDate tempDate;
        QTime tempTime;
        QDomNode node = element.firstChild();
        while( !node.isNull() ) {
            QDomElement element = node.toElement();
            if( !element.isNull() ) { // was really an element
                QString tagName = element.tagName();
                if( tagName == "Date" ) {
                    ok = ok & readDateNode( element, tempDate );
                } else if( tagName == "Time" ) {
                    ok = ok & readTimeNode( element, tempTime );
                } else {
                    qDebug( "Unknown tag in datetime" );
                }
            }
            node = node.nextSibling();
        }

        if( ok ) {
            datetime.setDate( tempDate );
            datetime.setTime( tempTime );
        }

        return ok;
    }


    bool readDateNode( const QDomElement& element, QDate& value )
    {
        bool ok = true;
        int year=0, month=0, day=0;
        if( element.hasAttribute( "Year" ) ) {
            bool yearOk = false;
            year = element.attribute( "Year" ).toInt( &yearOk );
            ok = ok & yearOk;
        }
        if( element.hasAttribute( "Month" ) ) {
            bool monthOk = false;
            month = element.attribute( "Month" ).toInt( &monthOk );
            ok = ok & monthOk;
        }
        if( element.hasAttribute( "Day" ) ) {
            bool dayOk = false;
            day = element.attribute( "Day" ).toInt( &dayOk );
            ok = ok & dayOk;
        }

        if( ok )
            value.setYMD( year, month, day );

        return ok;
    }



    bool readTimeNode( const QDomElement& element, QTime& value )
    {
        bool ok = true;
        int hour=0, minute=0, second=0, msec=0;
        if( element.hasAttribute( "Hour" ) ) {
            bool hourOk = false;
            hour = element.attribute( "Hour" ).toInt( &hourOk );
            ok = ok & hourOk;
        }
        if( element.hasAttribute( "Minute" ) ) {
            bool minuteOk = false;
            minute = element.attribute( "Minute" ).toInt( &minuteOk );
            ok = ok & minuteOk;
        }
        if( element.hasAttribute( "Second" ) ) {
            bool secondOk = false;
            second = element.attribute( "Second" ).toInt( &secondOk );
            ok = ok & secondOk;
        }
        if( element.hasAttribute( "Millisecond" ) ) {
            bool msecOk = false;
            msec = element.attribute( "Millisecond" ).toInt( &msecOk );
            ok = ok & msecOk;
        }

        if( ok )
            value.setHMS( hour, minute, second, msec );

        return ok;
    }



    Qt::PenStyle stringToPenStyle( const QString& style )
    {
        if( style == "NoPen" )
            return Qt::NoPen;
        else if( style == "SolidLine" )
            return Qt::SolidLine;
        else if( style == "DashLine" )
            return Qt::DashLine;
        else if( style == "DotLine" )
            return Qt::DotLine;
        else if( style == "DashDotLine" )
            return Qt::DashDotLine;
        else if( style == "DashDotDotLine" )
            return Qt::DashDotDotLine;
        else // should not happen
            return Qt::SolidLine;
    }


    Qt::BrushStyle stringToBrushStyle( const QString& style )
    {
        // PENDING(kalle) Support custom patterns
        if( style == "NoBrush" )
            return Qt::NoBrush;
        else if( style == "SolidPattern" )
            return Qt::SolidPattern;
        else if( style == "Dense1Pattern" )
            return Qt::Dense1Pattern;
        else if( style == "Dense2Pattern" )
            return Qt::Dense2Pattern;
        else if( style == "Dense3Pattern" )
            return Qt::Dense3Pattern;
        else if( style == "Dense4Pattern" )
            return Qt::Dense4Pattern;
        else if( style == "Dense5Pattern" )
            return Qt::Dense5Pattern;
        else if( style == "Dense6Pattern" )
            return Qt::Dense6Pattern;
        else if( style == "Dense7Pattern" )
            return Qt::Dense7Pattern;
        else if( style == "HorPattern" )
            return Qt::HorPattern;
        else if( style == "VerPattern" )
            return Qt::VerPattern;
        else if( style == "CrossPattern" )
            return Qt::CrossPattern;
        else if( style == "BDiagPattern" )
            return Qt::BDiagPattern;
        else if( style == "FDiagPattern" )
            return Qt::FDiagPattern;
        else if( style == "DiagCrossPattern" )
            return Qt::DiagCrossPattern;
        else // should not happen (but can with custom patterns)
            return Qt::SolidPattern;
    }

}
