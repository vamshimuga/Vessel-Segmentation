#ifndef MIAIMAGEREGISTRATIONFILTER_H
#define MIAIMAGEREGISTRATIONFILTER_H

#include "MiaCoreModules/miamodule.h"
class CostFunctionBase;
class TransformFunctionBase;
class GaussNewtonOptimizer;
enum MIALEVELSETMODULESSHARED_EXPORT TransformationMethod { MIA_Transform_Affine, MIA_Transform_Rigid3D, MIA_Transform_FixedCenteredRigid3D, MIA_Transform_ConstrainedRigid3D };
class MIALEVELSETMODULESSHARED_EXPORT MiaImageRegistrationFilter : public MiaModule
{
    Q_OBJECT
public:
    explicit MiaImageRegistrationFilter(MiaNetwork *parent = 0, int dim = 3);
    void setMovingImageInputModule( MiaModule* input);
    void setWeightingImageInputModule( MiaModule* input);
    virtual ~MiaImageRegistrationFilter();
    virtual void setSampleMethodToKeyPoints();
    virtual void setSampleMethodToRandomPoints(int size);
    virtual void setSampleMethodToAllPoints();//default
    virtual void setSampleMethodToDownSample(int interval);
    virtual void setSampleMethodToUsingMovingImage();
    void setBackGroundValue(float value);
    void saveCurrentTransformToFile(QDataStream &out);
    void loadTransformFromFile(QDataStream& in);
    Eigen::VectorXf getCurrentTransform();
    TransformationMethod getCurrentTransformationMethod() {return currentTransformationMethod;}
    void setCurrentTransformParameter(const Eigen::VectorXf& transform);
    void setCurrentTransformParameter(const QVector<float>& parameters);
    void setRotationCenter(MiaPoint4D rotationcenter);
    void setFixedTransformation(MiaPoint4D translation);
    void invertTransform();
    void setTranformationMethod(TransformationMethod method);
    void parseTransformFile(QDataStream &in, Eigen::VectorXf& parameters);
signals:
    void requestUpdateMovingImage(MiaImageND** img);
    void requestUpdateWeightingImage(MiaImageND** img);
    void requestUpdateKeyPoints(std::vector<int>& list);
    void registrationUpdate();
    void registrationComplete();
public slots:
    void forwardReigstrationUpdate();
    void forwardRegistrationComplete();
    void startRegistration();
    void registrationRunIntoError();
    void resetRegistration();
    void setMaximumIteration(int n);
protected:
    MiaImageND* movingImage;
    MiaImageND* weightingImage;
    MiaModule* movingImageInputModule;
    MiaModule* weightingImagingInputModule;
    virtual bool runOperation();
    virtual bool reloadInputImage();
    virtual bool reAllocOutputImage();
    GaussNewtonOptimizer* optimizer;
    CostFunctionBase* costFuction;
    TransformFunctionBase* transformFunction;
    float backgroundValue;
    QTime benchMarkerTimer;
    Eigen::VectorXf specifiedInitialParameters;
    TransformationMethod currentTransformationMethod;
    int transformFunctionDimension;

};

#endif // MIAIMAGEREGISTRATIONFILTER_H
