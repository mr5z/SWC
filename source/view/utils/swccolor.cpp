#include "../../../include/view/utils/swccolor.h"

namespace swc
{

swcColor::swcColor() :
    r(0xff),
    g(0xff),
    b(0xff)
{
    //ctor
}
swcColor::swcColor(const swcColor &other) :
    r(other.r),
    g(other.g),
    b(other.b)
{

}

swcColor::swcColor(uint8_t R, uint8_t G, uint8_t B) :
    r(R),
    g(G),
    b(B)
{

}

swcColor::swcColor(const rgb color) :
    r(color[0]),
    g(color[1]),
    b(color[2])
{

}

}
