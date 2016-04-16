#include "transformfunctionaffine.h"

TransformFunctionAffine::TransformFunctionAffine(int d) : TransformFunctionBase(d)
{
    //this will only call the virtual function at this level
    //not the derived sub-classes
    //a work around?
    resetParameters();
}

void TransformFunctionAffine::resetParameters()
{
    currentParamerters.resize( dimension*(dimension + 1) );
    currentParamerters.setZero();
    for(int i = 0; i < dimension; i++)
        currentParamerters(i*(dimension + 1) + i) = 1.0;
    updateBulkTransform();
}

void TransformFunctionAffine::getDerivative(Eigen::MatrixXd &jocobian, Eigen::Vector3f coor) const
{
    // here jacobin is acctually a transpose of real jacobin
    jocobian.setZero();

    for(int i = 0; i < dimension; i ++)
    {
        for(int j = 0; j < dimension; j ++)
        {
            jocobian(i*(dimension + 1) + j, i) = coor[j];
        }
        jocobian(i*(dimension + 1) +dimension,i) = 1.0;

    }

//    this->m_Jacobian.Fill( 0.0 );

//     const InputVectorType v = p - this->GetCenter();

//     unsigned int blockOffset = 0;

//     for(unsigned int block=0; block < NInputDimensions; block++)
//       {
//       for(unsigned int dim=0; dim < NOutputDimensions; dim++ )
//         {
//         this->m_Jacobian( block , blockOffset + dim ) = v[dim];
//         }

//       blockOffset += NInputDimensions;

//       }

//     for(unsigned int dim=0; dim < NOutputDimensions; dim++ )
//       {
//       this->m_Jacobian( dim , blockOffset + dim ) = 1.0;
//       }

//     return this->m_Jacobian;

}

bool TransformFunctionAffine::isValid() const
{
    bool allzeros = true;
    for(int i = 0; i < currentParamerters.rows(); i++)
    {
        if(currentParamerters[i] != 0)
        {
            allzeros = false;
            break;
        }
    }
    if(!allzeros)
        return true;
    return false;
}

TransformFunctionBase *TransformFunctionAffine::clone()
{
    TransformFunctionBase * newfunc = new TransformFunctionAffine(dimension);
    newfunc->setCurrentParameters(currentParamerters);
    return newfunc;
}

bool TransformFunctionAffine::initialParametersWithPointPairs(const Eigen::Matrix3Xf &fixedpts, const Eigen::Matrix3Xf &movingpts)
{
    if(fixedpts.cols() == 8 && movingpts.cols() == 8)
    {
        for(int i = 0; i < dimension; i++)
            currentParamerters(i*(dimension + 1) + i) = movingpts(i,7)/fixedpts(i,7);
    }
    updateBulkTransform();
    return true;
}

float TransformFunctionAffine::getScalingFactor()
{
    return 1.0;
}

void TransformFunctionAffine::updateBulkTransform()
{

    for(int i = 0; i < 3; i++)
    {
        if(i < dimension)
        {
            int j;
            for(j = 0; j < dimension ; j++)
                fixedToMovingBulkTransform(i,j) = currentParamerters[i*(dimension + 1) + j];
            for( ; j < 3; j++)
                fixedToMovingBulkTransform(i,j) = 0;
            fixedToMovingBulkTransform(i,3) = currentParamerters[i*(dimension + 1) + dimension];
        }
        else
        {
            for(int j = 0; j < 4 ; j++)
                fixedToMovingBulkTransform(i,j) = 0;
        }
    }

    movingToFixedBulkTransform = fixedToMovingBulkTransform.inverse();
}
