/****************************************************************************
** Copyright (C) 2001-2013 Klaralvdalens Datakonsult AB.  All rights reserved.
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <TableModel.h>
#include <KDChartDataValueAttributes>

namespace KDChart {
    class Chart;
    class BarDiagram;
}

class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow( QWidget* parent = 0 );

private slots:
    void on_scopeOneBarRB_toggled(  bool checked );
    void on_scopeBarDatasetSB_valueChanged( int i );
    void on_scopeBarItemSB_valueChanged(    int i );
    void on_scopeDatasetRB_toggled( bool checked );
    void on_scopeDatasetSB_valueChanged( int i );
    void on_scopeCommonRB_toggled( bool checked );

    void on_paintValuesCB_toggled( bool checked );

    void on_fontCombo_currentIndexChanged( const QString & text );
    void on_relativeSizeSB_valueChanged( int i );
    void on_minimumSizeSB_valueChanged(  int i );
    void on_rotationSB_valueChanged( int i );

    void on_posPosCombo_currentIndexChanged(   const QString & text );
    void on_posAlignCombo_currentIndexChanged( const QString & text );
    void on_posPadHoriSB_valueChanged( int i );
    void on_posPadVertSB_valueChanged( int i );
    void on_negPosCombo_currentIndexChanged(   const QString & text );
    void on_negAlignCombo_currentIndexChanged( const QString & text );
    void on_negPadHoriSB_valueChanged( int i );
    void on_negPadVertSB_valueChanged( int i );

    void on_labelLE_textEdited(  const QString & text );
    void on_prefixLE_textEdited( const QString & text );
    void on_suffixLE_textEdited( const QString & text );

private:
    const QModelIndex currentIndex() const;
    const KDChart::DataValueAttributes attributes() const;
    const char* positionToScreenName( const KDChart::Position& pos ) const;
    const Qt::Alignment alignmentFromScreeName( const QString& name ) const;
    const QString alignmentToScreenName( const Qt::Alignment& align ) const;
    void populateWidgets();
    void setAttributes( const KDChart::DataValueAttributes& da );

    KDChart::Chart* m_chart;
    KDChart::BarDiagram* m_bars;
    TableModel m_model;
};


#endif /* MAINWINDOW_H */

