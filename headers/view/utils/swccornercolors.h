#ifndef SWCCORNERCOLORS_H
#define SWCCORNERCOLORS_H

#include "swcdefs.h"

#include "swccolor.h"

namespace swc
{

class swcCornerColors
{
public:
    swcCornerColors();
    swcCornerColors(const swcColor& all_corner_colors);
    swcCornerColors(const swcColor& nw, const swcColor& ne, const swcColor& se, const swcColor& sw);
    swcCornerColors(const swcCornerColors& other);
    swcCornerColors(swcCornerColors&& other) noexcept;

    void setSideColor(int flag, const swcColor &side_colors);

    swcColor nw;
    swcColor ne;
    swcColor se;
    swcColor sw;
};

}

#endif // SWCCORNERCOLORS_H
