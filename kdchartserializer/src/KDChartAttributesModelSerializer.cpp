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
 ** Licensees holding valid commercial KDChart1 licenses may use this file in
 ** accordance with the KDChart1 Commercial License Agreement provided with
 ** the Software.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** See http://www.klaralvdalens-datakonsult.se/?page=products for
 **   information about KDChart1 Commercial License Agreements.
 **
 ** Contact info@klaralvdalens-datakonsult.se if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/

#include "KDChartAttributesModelSerializer.h"
#include "KDChartSerializeCollector.h"
#include "KDChartAttributesSerializer.h"

#include "KDXMLTools.h"

#include <qglobal.h>
#include <QMessageBox>


/**
  \class KDChart::AttributesModelSerializer KDChartAttributesModelSerializer.h

  \brief Auxiliary methods reading/saving KD Chart data and configuration in streams.
  */


using namespace KDChart;


AttributesModelSerializer::AttributesModelSerializer()
{
    // this space left empty intentionally
}

AttributesModelSerializer::~AttributesModelSerializer()
{
    // this space left empty intentionally
}

bool AttributesModelSerializer::parseAttributesModel(
        const QDomElement& e,
        AttributesModel& model )const
{
    bool bOK = true;
    QDomNode node = e.firstChild();
    while( !node.isNull() ) {
        QDomElement element = node.toElement();
        if( !element.isNull() ) { // was really an element
            QString tagName = element.tagName();
            /*
            if( tagName == "kdchart:cartesian-coordinate-plane" ) {
                bool b;
                if( KDXML::readBoolNode( element, b ) )
                    a.setVisible( b );
            } else if( tagName == "kdchart:polar-coordinate-plane" ) {
                QPen p;
                if( KDXML::readPenNode( element, p ) )
                    a.setPen( p );
            } else {
                QPen p;
                if( KDXML::readPenNode( element, p ) )
                    a.setPen( p );
            } else {
                qDebug() << "Unknown subelement of FrameAttributes found:" << tagName;
                bOK = false;
            }
            */
        }
        node = node.nextSibling();
    }
    return bOK;
}

void AttributesModelSerializer::saveAttributesModel(
        QDomDocument& doc,
        QDomElement& e,
        const AttributesModel* model )const
{
    if( ! model ) return;

    const QString title("kdchart:attribute-models");
    // access (or append, resp.) the global list
    QDomElement* modelsList =
            SerializeCollector::instance()->findOrMakeElement( doc, title );

    QDomElement attrModelPtrElement =
            doc.createElement( "AttributesModel" );
    e.appendChild( attrModelPtrElement );

    bool wasFound;
    QDomElement modelElement =
            SerializeCollector::findOrMakeChild(
            doc,
            *modelsList,
            attrModelPtrElement,
            "kdchart:attribute-model",
            "KDChart::AttributesModel",
            model,
            wasFound );
    if( ! wasFound ){
        // save the dataMap
        {
            QDomElement dataMapElement =
                    doc.createElement( "DataMap" );
            modelElement.appendChild( dataMapElement );
            const QMap<int, QMap<int, QMap<int, QVariant> > > dataMap( model->dataMap() );
            QMap<int, QMap<int, QMap<int, QVariant> > >::const_iterator i0 = dataMap.constBegin();
            while (i0 != dataMap.constEnd()) {
                QDomElement innerMapElement1 =
                        doc.createElement( "map" );
                dataMapElement.appendChild( innerMapElement1 );
                innerMapElement1.setAttribute( "key", i0.key() );
                QMap<int, QMap<int, QVariant> >::const_iterator i1 = i0.value().constBegin();
                while (i1 != i0.value().constEnd()) {
                    QDomElement innerMapElement2 =
                            doc.createElement( "map" );
                    innerMapElement1.appendChild( innerMapElement2 );
                    innerMapElement2.setAttribute( "key", i1.key() );
                    QMap<int, QVariant>::const_iterator i2 = i1.value().constBegin();
                    while (i2 != i1.value().constEnd()) {
                        createAttributesNode(
                                doc, innerMapElement2, model, i2.key(), i2.value() );
                        ++i2;
                    }
                    ++i1;
                }
                ++i0;
            }
        }
        // save the horizontalHeaderDataMap
        {
            QDomElement horizMapElement =
                    doc.createElement( "HorizontalHeaderDataMap" );
            modelElement.appendChild( horizMapElement );
            const QMap<int, QMap<int, QVariant> > horizMap( model->horizontalHeaderDataMap() );
            QMap<int, QMap<int, QVariant> >::const_iterator i0 = horizMap.constBegin();
            while (i0 != horizMap.constEnd()) {
                QDomElement innerMapElement1 =
                        doc.createElement( "map" );
                horizMapElement.appendChild( innerMapElement1 );
                innerMapElement1.setAttribute( "key", i0.key() );
                QMap<int, QVariant>::const_iterator i1 = i0.value().constBegin();
                while (i1 != i0.value().constEnd()) {
                    createAttributesNode(
                            doc, innerMapElement1, model, i1.key(), i1.value() );
                    ++i1;
                }
                ++i0;
            }
        }
        // save the verticalHeaderDataMap
        {
            QDomElement vertMapElement =
                    doc.createElement( "VerticalHeaderDataMap" );
            modelElement.appendChild( vertMapElement );
            const QMap<int, QMap<int, QVariant> > vertMap( model->verticalHeaderDataMap() );
            QMap<int, QMap<int, QVariant> >::const_iterator i0 = vertMap.constBegin();
            while (i0 != vertMap.constEnd()) {
                QDomElement innerMapElement1 =
                        doc.createElement( "map" );
                vertMapElement.appendChild( innerMapElement1 );
                innerMapElement1.setAttribute( "key", i0.key() );
                QMap<int, QVariant>::const_iterator i1 = i0.value().constBegin();
                while (i1 != i0.value().constEnd()) {
                    createAttributesNode(
                            doc, innerMapElement1, model, i1.key(), i1.value() );
                    ++i1;
                }
                ++i0;
            }
        }
        // save the modelDataMap
        {
            QDomElement modelMapElement =
                    doc.createElement( "ModelDataMap" );
            modelElement.appendChild( modelMapElement );
            QMap<int, QVariant > modelMap( model->modelDataMap() );
            QMap<int, QVariant>::const_iterator i = modelMap.constBegin();
            while (i != modelMap.constEnd()) {
                createAttributesNode(
                        doc, modelMapElement, model, i.key(), i.value() );
                ++i;
            }
        }
        // save the palette type
        QString name;
        switch( model->paletteType() ){
            case AttributesModel::PaletteTypeDefault:
                name = "PaletteTypeDefault";
                break;
            case AttributesModel::PaletteTypeRainbow:
                name = "PaletteTypeRainbow";
                break;
            case AttributesModel::PaletteTypeSubdued:
                name = "PaletteTypeSubdued";
                break;
            default:
                Q_ASSERT( false ); // all of the types need to be handled
                break;
        }
        KDXML::createStringNode( doc, modelElement, "Palette", name );
    }
}

void AttributesModelSerializer::createAttributesNode(
        QDomDocument& doc,
        QDomElement& e,
        const AttributesModel* model,
        int role,
        const QVariant& attributes )const
{
    if( ! model ) return;

    QDomElement element =
            doc.createElement( "value" );
    e.appendChild( element );
    QString name;
    if( model->isKnownAttributesRole( role ) ){
        switch( role ) {
            case DataValueLabelAttributesRole:
                name = QString::fromLatin1("DataValueLabelAttributesRole");
                AttributesSerializer::saveDataValueAttributes(
                        doc,
                        element,
                        qVariantValue<DataValueAttributes>( attributes ),
                        "DataValueLabelAttributes" );
                break;
            case DatasetBrushRole:
                name = QString::fromLatin1("DatasetBrushRole");
                KDXML::createBrushNode( doc, element, "DatasetBrush",
                                        qVariantValue<QBrush>( attributes ) );
                break;
            case DatasetPenRole:
                name = QString::fromLatin1("DatasetPenRole");
                KDXML::createPenNode( doc, element, "DatasetPen",
                                        qVariantValue<QPen>( attributes ) );
                break;
            case ThreeDAttributesRole:
                // As of yet there is no ThreeDAttributes class,
                // and the AbstractThreeDAttributes class is pure virtual,
                // so we ignore this role for now.
                // (khz, 17.03.2007)
                /*
                name = QString::fromLatin1("ThreeDAttributesRole");
                AttributesSerializer::saveThreeDAttributes(
                        doc,
                        element,
                        qVariantValue<ThreeDAttributes>( attributes ),
                        "ThreeDAttributes" );
                */
                break;
            case LineAttributesRole:
                name = QString::fromLatin1("LineAttributesRole");
                AttributesSerializer::saveLineAttributes(
                        doc,
                        element,
                        qVariantValue<LineAttributes>( attributes ),
                        "LineAttributes" );
                break;
            case ThreeDLineAttributesRole:
                name = QString::fromLatin1("ThreeDLineAttributesRole");
                AttributesSerializer::saveThreeDLineAttributes(
                        doc,
                        element,
                        qVariantValue<ThreeDLineAttributes>( attributes ),
                        "ThreeDLineAttributes" );
                break;
            case BarAttributesRole:
                name = QString::fromLatin1("BarAttributesRole");
                AttributesSerializer::saveBarAttributes(
                        doc,
                        element,
                        qVariantValue<BarAttributes>( attributes ),
                        "BarAttributes" );
                break;
            case ThreeDBarAttributesRole:
                name = QString::fromLatin1("ThreeDBarAttributesRole");
                AttributesSerializer::saveThreeDBarAttributes(
                        doc,
                        element,
                        qVariantValue<ThreeDBarAttributes>( attributes ),
                        "ThreeDBarAttributes" );
                break;
            case PieAttributesRole:
                name = QString::fromLatin1("PieAttributesRole");
                AttributesSerializer::savePieAttributes(
                        doc,
                        element,
                        qVariantValue<PieAttributes>( attributes ),
                        "PieAttributes" );
                break;
            case ThreeDPieAttributesRole:
                name = QString::fromLatin1("ThreeDPieAttributesRole");
                AttributesSerializer::saveThreeDPieAttributes(
                        doc,
                        element,
                        qVariantValue<ThreeDPieAttributes>( attributes ),
                        "ThreeDPieAttributes" );
                break;
            case DataHiddenRole:
                name = QString::fromLatin1("DataHiddenRole");
                KDXML::createBoolNode( doc, element, "DataHidden",
                                       qVariantValue<bool>( attributes ) );
                break;
            default:
                Q_ASSERT( false ); // all of our own roles need to be handled
                break;
        }
    }else{
        name = QString::fromLatin1("ROLE:%1").arg( role );
    }
    element.setAttribute( "key", name );
}
