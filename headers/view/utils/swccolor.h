#ifndef SWCCOLOR_H
#define SWCCOLOR_H

#include <bits/move.h>

#include "swcdefs.h"

namespace swc
{

class swcColor
{
public:
    swcColor();
    swcColor(const swcColor &other);
    swcColor(uint8_t r, uint8_t g, uint8_t b);
    swcColor(const rgb color);

    inline swcColor& operator = (const swcColor &other) {
        r = other.r;
        g = other.g;
        b = other.b;
        return *this;
    }

    inline swcColor& operator = (const rgb other) {
        r = other[0];
        g = other[1];
        b = other[2];
        return *this;
    }

    uint8_t r;
    uint8_t g;
    uint8_t b;

};

}

#endif // SWCCOLOR_H
