/* -*- Mode: C++ -*-
   KDChart - a multi-platform charting engine
   */

/****************************************************************************
 ** Copyright (C) 2001-2006 Klarälvdalens Datakonsult AB.  All rights reserved.
 **
 ** This file is part of the KD Chart library.
 **
 ** This file may be distributed and/or modified under the terms of the
 ** GNU General Public License version 2 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.
 **
 ** Licensees holding valid commercial KD Chart licenses may use this file in
 ** accordance with the KD Chart Commercial License Agreement provided with
 ** the Software.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** See http://www.kdab.net/kdchart for
 **   information about KD Chart Commercial License Agreements.
 **
 ** Contact info@kdab.net if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/

#include "KDChartPalette.h"
#include <QBrush>
#include <QVector>

//FIXME(khz): use an internal libfakes library instead of this internal haeder file
#include "kdchart_platformdef.h"

using namespace KDChart;

namespace {
    static Palette * s_defaultPalette = 0;
    static Palette * s_subduedPalette = 0;
    static Palette * s_rainbowPalette = 0;
}

#define d d_func()

class Palette::Private
{
public:
    explicit Private() {}
    ~Private() {}

    QVector<QBrush> brushes;
};

const Palette& Palette::defaultPalette()
{
  if ( !s_defaultPalette ) {
    s_defaultPalette = new Palette;
    s_defaultPalette->addBrush( Qt::red );
    s_defaultPalette->addBrush( Qt::green );
    s_defaultPalette->addBrush( Qt::blue );
    s_defaultPalette->addBrush( Qt::cyan );
    s_defaultPalette->addBrush( Qt::magenta );
    s_defaultPalette->addBrush( Qt::yellow );
    s_defaultPalette->addBrush( Qt::darkRed );
    s_defaultPalette->addBrush( Qt::darkGreen );
    s_defaultPalette->addBrush( Qt::darkBlue );
    s_defaultPalette->addBrush( Qt::darkCyan );
    s_defaultPalette->addBrush( Qt::darkMagenta );
    s_defaultPalette->addBrush( Qt::darkYellow );
  }
  return *s_defaultPalette;
}

const Palette& Palette::subduedPalette()
{
  if ( !s_subduedPalette ) {
    s_subduedPalette = new Palette;
    s_subduedPalette->addBrush( QColor( 0xe0,0x7f,0x70 ) );
    s_subduedPalette->addBrush( QColor( 0xe2,0xa5,0x6f ) );
    s_subduedPalette->addBrush( QColor( 0xe0,0xc9,0x70 ) );
    s_subduedPalette->addBrush( QColor( 0xd1,0xe0,0x70 ) );
    s_subduedPalette->addBrush( QColor( 0xac,0xe0,0x70 ) );
    s_subduedPalette->addBrush( QColor( 0x86,0xe0,0x70 ) );
    s_subduedPalette->addBrush( QColor( 0x70,0xe0,0x7f ) );
    s_subduedPalette->addBrush( QColor( 0x70,0xe0,0xa4 ) );
    s_subduedPalette->addBrush( QColor( 0x70,0xe0,0xc9 ) );
    s_subduedPalette->addBrush( QColor( 0x70,0xd1,0xe0 ) );
    s_subduedPalette->addBrush( QColor( 0x70,0xac,0xe0 ) );
    s_subduedPalette->addBrush( QColor( 0x70,0x86,0xe0 ) );
    s_subduedPalette->addBrush( QColor( 0x7f,0x70,0xe0 ) );
    s_subduedPalette->addBrush( QColor( 0xa4,0x70,0xe0 ) );
    s_subduedPalette->addBrush( QColor( 0xc9,0x70,0xe0 ) );
    s_subduedPalette->addBrush( QColor( 0xe0,0x70,0xd1 ) );
    s_subduedPalette->addBrush( QColor( 0xe0,0x70,0xac ) );
    s_subduedPalette->addBrush( QColor( 0xe0,0x70,0x86 ) );
  }
  return *s_subduedPalette;
}

const Palette& Palette::rainbowPalette()
{
  if ( !s_rainbowPalette ) {
    s_rainbowPalette = new Palette;
    s_rainbowPalette->addBrush( QColor(255,  0,196) );
    s_rainbowPalette->addBrush( QColor(255,  0, 96) );
    s_rainbowPalette->addBrush( QColor(255, 128,64) );
    s_rainbowPalette->addBrush( Qt::yellow );
    s_rainbowPalette->addBrush( Qt::green );
    s_rainbowPalette->addBrush( Qt::cyan );
    s_rainbowPalette->addBrush( QColor( 96, 96,255) );
    s_rainbowPalette->addBrush( QColor(160,  0,255) );
    for( int i=8; i<16; ++i )
        s_rainbowPalette->addBrush( s_rainbowPalette->getBrush(i-8).color().light(), i );
  }
  return *s_rainbowPalette;
}

Palette::Palette( QObject *parent )
  : QObject( parent ), _d( new Private )
{
    // this line intentionally left empty
}

Palette::~Palette()
{
    delete _d; _d = 0;
}



Palette::Palette( const Palette& r )
    : QObject(), _d( new Private( *r.d ) )
{
}

Palette& Palette::operator=( const Palette& r )
{
    if( this == &r )
        return *this;

    *d = *r.d;

    return *this;
}

bool Palette::isValid() const
{
  return d->brushes.size() >= 1;
}

int Palette::size() const
{
  return d->brushes.size();
}

void Palette::addBrush( const QBrush& brush, int position )
{
  if ( position == -1 || position >= size() ) {
    d->brushes.append( brush );
  } else {
    d->brushes.insert( position, brush );
  }
  emit changed();
}

QBrush Palette::getBrush( int position ) const
{
  if ( !isValid() ) return QBrush();
  return d->brushes.at( position % size() );
}

void Palette::removeBrush( int position )
{
  if ( position >= size() ) return;
  d->brushes.remove( position );
  emit changed();
}

