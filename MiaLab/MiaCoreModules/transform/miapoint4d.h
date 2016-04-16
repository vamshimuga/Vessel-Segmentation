#ifndef POINT4D_H
#define POINT4D_H
#include <math.h>
#include <QDataStream>
#include "miamodules_global.h"

class MIACOREMODULESSHARED_EXPORT MiaPoint4D
{
public:
    MiaPoint4D() { pos[0] = 0; pos[1] = 0; pos[2] = 0; pos[3] = 0;}
    MiaPoint4D(float x, float y, float z, float t)
    { pos[0] = x; pos[1] = y; pos[2] = z; pos[3] = t;}
    MiaPoint4D& operator=(const MiaPoint4D& other) // Assignment operator
     {
        pos[0] = other.pos[0];
        pos[1] = other.pos[1];
        pos[2] = other.pos[2];
        pos[3] = other.pos[3];
         return *this;
     }
    MiaPoint4D operator+ (const MiaPoint4D& other) const
    {
          MiaPoint4D result;
          for(int i = 0;  i<4; i++)
              result.pos[i] = pos[i] + other.pos[i];
          return result;
    }
    MiaPoint4D operator- (const MiaPoint4D& other) const
    {
          MiaPoint4D result;
          for(int i = 0;  i<4; i++)
              result.pos[i] = pos[i] - other.pos[i];
          return result;
    }
    MiaPoint4D operator* (const float& scale) const
    {
          MiaPoint4D result;
          for(int i = 0;  i<4; i++)
              result.pos[i] = pos[i] * scale;
          return result;
    }
    float operator* (const MiaPoint4D& other) const
    {
          float result = 0.0f;
          for(int i = 0;  i<3; i++)
              result += pos[i] * other.pos[i];
          return result;
    }
    bool operator== (const MiaPoint4D& other) const
    {
        return (pos[0] == other.pos[0] && pos[1] == other.pos[1] && pos[2] == other.pos[2] && pos[3] == other.pos[3]);
    }
    MiaPoint4D cross (const MiaPoint4D& other) const
    {
          MiaPoint4D result;
          result.pos[0] = pos[1] * other.pos[2] - pos[2]*other.pos[1];
          result.pos[1] = pos[2] * other.pos[0] - pos[0]*other.pos[2];
          result.pos[2] = pos[0] * other.pos[1] - pos[1]*other.pos[0];
          return result;
    }
    void setPos(float* apos)
    {
        pos[0] = apos[0];
        pos[1] = apos[1];
        pos[2] = apos[2];
        pos[3] = apos[3];
    }
    double distance(const MiaPoint4D& other) const
    {
        double distance = (pos[0] - other.pos[0]) * (pos[0] - other.pos[0])
                            + (pos[1] - other.pos[1]) * (pos[1] - other.pos[1])
                            + (pos[2] - other.pos[2]) * (pos[2] - other.pos[2]) ;
        return sqrt(distance);
    }

    void normalize()
    {
        float factor = 1.0f/sqrt(pos[0]*pos[0]+pos[1]*pos[1]+pos[2]*pos[2]);
        pos[0] *= factor; pos[1] *= factor; pos[2] *= factor;
    }

    float pos[4];
};

QDataStream& operator<<(QDataStream& stream, const MiaPoint4D& pt );
QDataStream& operator>>(QDataStream& stream, MiaPoint4D& pt );

#endif // POINT4D_H
