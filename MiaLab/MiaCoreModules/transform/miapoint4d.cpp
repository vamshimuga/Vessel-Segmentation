#include "miapoint4d.h"
#include <stdexcept>

QDataStream& operator<<(QDataStream& stream, const MiaPoint4D& pt )
{
    stream<<pt.pos[0]<<pt.pos[1]<<pt.pos[2]<<pt.pos[3];
    return stream;
}

QDataStream& operator>>(QDataStream& stream, MiaPoint4D& pt )
{
    float x[4];
    stream>>x[0]>>x[1]>>x[2]>>x[3];

    pt.setPos(x);
    return stream;
}
