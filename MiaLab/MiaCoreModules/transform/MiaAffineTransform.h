#ifndef MIAAFFINETRANSFORM_H
#define MIAAFFINETRANSFORM_H
#include "miapoint4d.h"
#include <Eigen/Dense>
class MIACOREMODULESSHARED_EXPORT MiaAffineTransform
{
public:
    MiaAffineTransform()
    {
        for(int i = 0; i < 16 ; i ++ )
        {
            if( i % 5 == 0)
            {
                toPatientSystemMatrix[i] = 1.0;
                fromPatientSystemMatrix[i] = 1.0;
            }
            else
            {
                toPatientSystemMatrix[i] = 0.0;
                fromPatientSystemMatrix[i] = 0.0;
            }
        }

    }

    MiaAffineTransform (float orientationMatrix[],float spacing[], float offset[])
    {
        int i,j;
        for( i = 0; i < 3 ; i++)
            for( j = 0; j < 3; j++)
            {
                toPatientSystemMatrix[ i*4 + j ] = orientationMatrix[ j*3 + i] * spacing[j];
            }
        for( i = 0; i < 3 ; i++)
        {
            toPatientSystemMatrix[ i*4 + 3 ] = offset[i] * spacing[3];
        }
        for( j = 0; j < 3; j++)
        {
            toPatientSystemMatrix[ 3*4 + j ] = 0;
        }
        toPatientSystemMatrix[ 15 ] = 1.0f;
        computeInverseMatrix();

    }
    MiaAffineTransform (const MiaPoint4D& axis,float angle) // create a matrix with rotation around the fiven axis
    {
        MiaPoint4D normalize_axis =  axis;
        normalize_axis.normalize();
        double cos_a = cos(angle);
        double sin_a = sin(angle);
        double x = normalize_axis.pos[0];
        double y = normalize_axis.pos[1];
        double z = normalize_axis.pos[2];
        toPatientSystemMatrix[0] = cos_a + x*x*(1.0-cos_a);
        toPatientSystemMatrix[1] = x*y*(1.0-cos_a) - z*sin_a;
        toPatientSystemMatrix[2] = x*z*(1.0-cos_a) + y*sin_a;
        toPatientSystemMatrix[4] = y*x*(1.0-cos_a) + z*sin_a;
        toPatientSystemMatrix[5] = cos_a + y*y*(1.0-cos_a);
        toPatientSystemMatrix[6] = y*z*(1.0-cos_a) - x*sin_a;
        toPatientSystemMatrix[8] = z*x*(1.0-cos_a) - y*sin_a;
        toPatientSystemMatrix[9] = z*y*(1.0-cos_a) + x*sin_a;
        toPatientSystemMatrix[10] = cos_a + z*z*(1-cos_a);
        int i;
        for( i = 0; i < 3 ; i++)
        {
            toPatientSystemMatrix[ i*4 + 3 ] = 0;
            toPatientSystemMatrix[ 3*4 + i ] = 0;
        }

        toPatientSystemMatrix[ 15 ] = 1.0f;
        computeInverseMatrix();

    }
    Eigen::Affine3f toEigenTransform()
    {
        Eigen::Affine3f t;
        int i,j;
        for( i = 0; i < 3 ; i++)
            for( j = 0; j < 4; j++)
            {
                t(i,j) = toPatientSystemMatrix[ i*4 + j ];
            }
        return t;
    }

    void translate(float offset[])
    {
        for(int i = 0; i < 3 ; i++)
        {
            toPatientSystemMatrix[ i*4 + 3 ] += offset[i];
        }
        computeInverseMatrix();
    }
    void scale(float s[])
    {
        for(int i = 0; i < 3 ; i++)
            for(int j = 0; j < 3; j++)
            {
                toPatientSystemMatrix[i*4 + j] *= s[i];
            }
        computeInverseMatrix();
    }

    void computeInverseMatrix()
    {
        //compute inverted transform matrix
        float orientationMatrix[9];
        int i,j;
        for( i = 0; i < 3 ; i++)
            for( j = 0; j < 3; j++)
            {
                orientationMatrix[ j*3 + i] = toPatientSystemMatrix[ i*4 + j ];
            }
        float inverseOrientationMatrix[9];
        inverse3by3Matrix(orientationMatrix, inverseOrientationMatrix);
        for( i = 0; i < 3 ; i++)
            for( j = 0; j < 3; j++)
            {
                fromPatientSystemMatrix[ i*4 + j ] = inverseOrientationMatrix[ j*3 + i];
            }
        float offset[3];
        for( i = 0; i < 3 ; i++)
        {
           offset[i] = toPatientSystemMatrix[ i*4 + 3 ];
        }
        for( i = 0; i < 3 ; i++)
        {
            fromPatientSystemMatrix[ i*4 + 3 ] = -offset[0] * fromPatientSystemMatrix[ i*4 + 0 ]
                                                 -offset[1] * fromPatientSystemMatrix[ i*4 + 1 ]
                                                 -offset[2] * fromPatientSystemMatrix[ i*4 + 2 ];
        }

        for( j = 0; j < 3; j++)
        {
            fromPatientSystemMatrix[ 3*4 + j ] = 0;
        }

        fromPatientSystemMatrix[ 15 ] = 1.0f;
    }

    MiaPoint4D transform(const MiaPoint4D& inpt) const
    {
        MiaPoint4D outpt;
        int i,j;
        for( i = 0; i < 4; i ++)
        {
            outpt.pos[i] = 0.0;
            for( j = 0 ; j < 3 ; j++ )
            {
                outpt.pos[i] += inpt.pos[j] * toPatientSystemMatrix[i*4 + j];
            }
            outpt.pos[i] += toPatientSystemMatrix[i*4 + 3];


        }
        outpt.pos[3] = inpt.pos[3] * toPatientSystemMatrix[15];
        return outpt;
    }
    MiaPoint4D inversedTransform(const MiaPoint4D& inpt) const
    {
        MiaPoint4D outpt;

        int i,j;
        for( i = 0; i < 4; i ++)
        {
            outpt.pos[i] = 0.0;
            for( j = 0 ; j < 3 ; j++ )
            {
                outpt.pos[i] += inpt.pos[j] * fromPatientSystemMatrix[i*4 + j];
            }

            outpt.pos[i] += fromPatientSystemMatrix[i*4 + 3];

        }
        outpt.pos[3] = inpt.pos[3]*fromPatientSystemMatrix[15];
        return outpt;
    }

    MiaAffineTransform& operator=(const MiaAffineTransform& other) // Assignment operator
    {
        for(int i = 0; i < 16 ; i ++ )
        {
            toPatientSystemMatrix[i] = other.toPatientSystemMatrix[i];
            fromPatientSystemMatrix[i] = other.fromPatientSystemMatrix[i];
        }
        return *this;
    }
    MiaAffineTransform operator* (const MiaAffineTransform& other) const
    {
          MiaAffineTransform result;
          for(int i = 0 ; i < 4; i++)
              for(int j = 0; j < 4; j++)
              {
                  result.toPatientSystemMatrix[i*4 + j] = 0.0;
                  for(int k = 0 ; k < 4; k++)
                      result.toPatientSystemMatrix[i*4 + j] += toPatientSystemMatrix[i*4 + k ]*other.toPatientSystemMatrix[k*4 + j];
              }
          result.computeInverseMatrix();
          return result;
    }
    void flipLeftRight()
    {
        toPatientSystemMatrix[0] = -1.0 * toPatientSystemMatrix[0];
        toPatientSystemMatrix[4] = -1.0 * toPatientSystemMatrix[4];
        toPatientSystemMatrix[8] = -1.0 * toPatientSystemMatrix[8];

        fromPatientSystemMatrix[0] = -1.0 * fromPatientSystemMatrix[0];
        fromPatientSystemMatrix[1] = -1.0 * fromPatientSystemMatrix[1];
        fromPatientSystemMatrix[2] = -1.0 * fromPatientSystemMatrix[2];
        fromPatientSystemMatrix[3] = -1.0 * fromPatientSystemMatrix[3];
    }
    void flipUpDown()
    {
        toPatientSystemMatrix[1] = -1.0 * toPatientSystemMatrix[1];
        toPatientSystemMatrix[5] = -1.0 * toPatientSystemMatrix[5];
        toPatientSystemMatrix[9] = -1.0 * toPatientSystemMatrix[9];

        fromPatientSystemMatrix[4] = -1.0 * fromPatientSystemMatrix[4];
        fromPatientSystemMatrix[5] = -1.0 * fromPatientSystemMatrix[5];
        fromPatientSystemMatrix[6] = -1.0 * fromPatientSystemMatrix[6];
        fromPatientSystemMatrix[7] = -1.0 * fromPatientSystemMatrix[7];
    }
    float toPatientSystemMatrix[16];
    float fromPatientSystemMatrix[16];
    static void inverse3by3Matrix(float *inm, float *outm)
    {
        float detinm=inm[0]*inm[4]*inm[8]+inm[1]*inm[5]*inm[6]+inm[2]*inm[3]*inm[7]-inm[2]*inm[4]*inm[6]-inm[1]*inm[3]*inm[8]-inm[0]*inm[5]*inm[7];
        if(detinm==0) return;
        outm[0] = (inm[4]*inm[8]-inm[5]*inm[7])/detinm;
        outm[1] = (inm[2]*inm[7]-inm[1]*inm[8])/detinm;
        outm[2] = (inm[1]*inm[5]-inm[2]*inm[4])/detinm;
        outm[3] = (inm[5]*inm[6]-inm[3]*inm[8])/detinm;
        outm[4] = (inm[0]*inm[8]-inm[2]*inm[6])/detinm;
        outm[5] = (inm[2]*inm[3]-inm[0]*inm[5])/detinm;
        outm[6] = (inm[3]*inm[7]-inm[4]*inm[6])/detinm;
        outm[7] = (inm[1]*inm[6]-inm[0]*inm[7])/detinm;
        outm[8] = (inm[0]*inm[4]-inm[1]*inm[3])/detinm;
    }
};
#endif // MIAAFFINETRANSFORM_H
