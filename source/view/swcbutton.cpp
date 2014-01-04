#include <GL/gl.h>

#include "../../../ColorCollection/include/colorcollection.h"

#include "../../include/controller/swcwindowhandler.h"
#include "../../include/controller/swcmouse.h"
#include "../../include/view/swcbutton.h"

namespace swc
{

swcButton::swcButton() :
     alt_color_click        {120, 120, 120}
    ,alt_color_click_border {0, 0, 0}
    ,color_swap_click       (false)
    ,color_swap_click_border(false)
{
    //ctor

    using namespace cc;

    attachParentRect(this);
    setSize(60, 20);
    setColor(C_NW | C_NE, getColor(COLORS::GRAY5));
    setColor(C_SE | C_SW, getColor(COLORS::GRAY1));
    setBorderColor(ALL_BORDER_SIDE(), getColor(COLORS::GRAY1));
    setAltColor(C_NW | C_NE, getColor(COLORS::GRAY5));
    setAltColor(C_SE | C_SW, getColor(COLORS::GRAY1));
    setAltBorderColor(ALL_BORDER_SIDE(), getColor(COLORS::WHITE));
    setBorderSize(ALL_BORDER_SIDE(), 1);
}

void swcButton::impPostPaint()
{
    showText();
}

void swcButton::impOnClickOnce()
{
    if (color_swap_click)
        setColor(ALL_CORNER_SIDE(), alt_color_click);
    if (color_swap_click_border)
        setBorderColor(ALL_BORDER_SIDE(), alt_color_click_border);
}

void swcButton::impOnRelease()
{
    if (color_swap_click)
        setColor(ALL_CORNER_SIDE(), alt_color_click);
    if (color_swap_click_border)
        setBorderColor(ALL_BORDER_SIDE(), alt_color_click_border);
}

}
