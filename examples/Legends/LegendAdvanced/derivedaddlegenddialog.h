#ifndef DERIVEDADDLEGENDDIALOG_H
#define DERIVEDADDLEGENDDIALOG_H

#include "ui_addlegenddialog.h"

class DerivedAddLegendDialog : public QDialog, public Ui::AddLegendDialog
{
    Q_OBJECT

public:
    DerivedAddLegendDialog( QWidget* parent = 0 );
};

#endif // DERIVEDADDLEGENDDIALOG_H
