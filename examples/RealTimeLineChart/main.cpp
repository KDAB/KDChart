//------------------------------------------------------------------------
// COPYRIGHT (C) 2010
// THIS SOFTWARE IS THE PROPERTY OF ROCHE DIAGNOSTICS GRAZ GMBH, AUSTRIA:
// ALL RIGHTS RESERVED NO PART OF THIS SOFTWARE MAY BE USED OR COPIED IN
// ANY WAY WITHOUT THE PRIOR WRITTEN PERMISSION OF ROCHE DIAGNOSTICS GRAZ
// GMBH, AUSTRIA
//------------------------------------------------------------------------

/*!
 * \file %FILENAME%
 * \brief $MODULE$ Class
 * \author $AUTHOR$ <$EMAIL$>
 */

#include <QtGui/QApplication>
#include "Dialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.show();

    return a.exec();
}
