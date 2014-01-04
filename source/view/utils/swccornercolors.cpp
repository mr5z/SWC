#include "../../../include/view/utils/swccornercolors.h"

namespace swc
{

swcCornerColors::swcCornerColors()
{
    //ctor
}

swcCornerColors::swcCornerColors(const swcColor& all_corner_colors) :
    nw(all_corner_colors),
    ne(all_corner_colors),
    se(all_corner_colors),
    sw(all_corner_colors)
{
    //ctor
}

swcCornerColors::swcCornerColors(const swcColor& _nw, const swcColor& _ne, const swcColor& _se, const swcColor& _sw) :
    nw(_nw),
    ne(_ne),
    se(_se),
    sw(_sw)
{
    //ctor
}

swcCornerColors::swcCornerColors(const swcCornerColors& other) :
    nw(other.nw),
    ne(other.ne),
    se(other.se),
    sw(other.sw)

{
    //ctor
}

swcCornerColors::swcCornerColors(swcCornerColors&& other) noexcept :
    nw(std::move(other.nw)),
    ne(std::move(other.ne)),
    se(std::move(other.se)),
    sw(std::move(other.sw))
{
    //ctor
}

void swcCornerColors::setSideColor(int flag, const swcColor &side_colors)
{
    switch (flag)
    {
    case B_TOP:
        nw = side_colors;
        ne = side_colors;
        break;
    case B_RIGHT:
        ne = side_colors;
        se = side_colors;
        break;
    case B_BOTTOM:
        se = side_colors;
        sw = side_colors;
        break;
    case B_LEFT:
        sw = side_colors;
        nw = side_colors;
        break;
    }
}

}
