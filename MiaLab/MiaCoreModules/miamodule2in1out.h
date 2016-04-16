#ifndef MIAMODULE2IN1OUT_H
#define MIAMODULE2IN1OUT_H

#include "miamodule.h"

class MIACOREMODULESSHARED_EXPORT MiaModule2In1Out : public MiaModule
{
    Q_OBJECT
public:
    explicit MiaModule2In1Out(MiaNetwork *parent = 0);
    virtual ~MiaModule2In1Out();
    void setSecondInputModule( MiaModule* input);
signals:
    void requestUpdateSecondInput(MiaImageND** img);
public slots:
protected:
    MiaImageND* secondInputImage;
    MiaModule* secondInputModule;
    bool needImageTypeMatch;
    ImageType secondImageRequiredType;
    virtual bool reloadInputImage();
};

#endif // MIAMODULE2IN1OUT_H
