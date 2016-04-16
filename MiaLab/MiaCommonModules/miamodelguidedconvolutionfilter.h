#ifndef MIAMODELGUIDEDCONVOLUTIONFILTER_H
#define MIAMODELGUIDEDCONVOLUTIONFILTER_H

#include "miaconvolutionfilter.h"
enum MIACOMMONMODULESSHARED_EXPORT LocalPhaseType {MIA_Bright2Dark_Edge,MIA_Dark2Bright_Edge,MIA_Bright_Line, MIA_Dark_Line};

class MIACOMMONMODULESSHARED_EXPORT MiaModelGuidedConvolutionFilter : public MiaConvolutionFilter
{
    Q_OBJECT
public:
    explicit MiaModelGuidedConvolutionFilter(MiaNetwork *parent = 0);
    virtual ~MiaModelGuidedConvolutionFilter();
    void setFilterOrientation(MiaPoint4D& o){filterDirection = o;filterDirection.normalize();}
    void setSecondInputModule( MiaModule* input);
signals:
    void requestUpdateSecondInput(MiaImageND** img);
public slots:
protected:
    virtual bool runOperation();
    template <class S>
    bool runModelOperation(S* inData, float* outdata);
    virtual bool reloadInputImage();
    MiaPoint4D filterDirection;
    MiaImageND* modelImage;
    MiaModule* secondInputModule;

};

#endif // MIAMODELGUIDEDCONVOLUTIONFILTER_H
