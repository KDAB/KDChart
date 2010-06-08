#ifndef DIAGRAMTYPE_H
#define DIAGRAMTYPE_H

#include "ui_diagramtype.h"

class DiagramType : public QDialog, public Ui::DiagramTypeDLG
{
    Q_OBJECT

public:
    DiagramType( QWidget* parent = 0 );
};

#endif // DIAGRAMTYPE_H
