/* -*- Mode: C++ -*-
   KDChart - a multi-platform charting engine
   */

/****************************************************************************
 ** Copyright (C) 2005 Klarälvdalens Datakonsult AB.  All rights reserved.
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

#ifndef KDCHARTTHREEDATTRIBUTES_P_H
#define KDCHARTTHREEDATTRIBUTES_P_H

#include <KDChartThreeDAttributes.h>

namespace KDChart {

class ThreeDAttributes::Private
{
    friend class ThreeDAttributes;
public:
    Private();

private:
    bool enabled;
    int height;
    int depth;
};

inline ThreeDAttributes::ThreeDAttributes( Private * p ) : _d( p ) { init(); }

}

#endif // KDCHARTTHREEDATTRIBUTES_P_H
