/****************************************************************************
** Copyright (C) 2001-2011 Klaralvdalens Datakonsult AB.  All rights reserved.
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

#ifndef __ADJUSTEDCARTESIANAXISSERIALIZER_H__
#define __ADJUSTEDCARTESIANAXISSERIALIZER_H__

#include <KDChartAxesSerializer>

class AdjustedCartesianAxisSerializer : public KDChart::AxesSerializer
{
    Q_OBJECT

    Q_DISABLE_COPY( AdjustedCartesianAxisSerializer )

public:
    explicit AdjustedCartesianAxisSerializer();
    virtual ~AdjustedCartesianAxisSerializer();

    /** reimp */
    virtual void saveElement( QDomDocument& doc, QDomElement& e, const QObject* obj ) const;
    /** reimp */
    virtual bool parseElement( const QDomElement& container, QObject* ptr ) const;
};

#endif
