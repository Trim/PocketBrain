#ifndef EMOACTOR_H
#define EMOACTOR_H


#include "emotions.h"
#include "ui_emoactor.h"

class emoactor : public QWidget, public Ui_Emoactor
{
public:
    emoactor(QWidget *parent=0, Qt::WFlags f=0);
};

#endif // EMOACTOR_H
