#ifndef SWCBUTTON_H
#define SWCBUTTON_H

#include "swcobject.h"
#include "swctext.h"
#include "utils/swccolor.h"

namespace swc
{

class swcButton :
    public swcObject,
    public swcText
{
public:
    swcButton();

    swcColor alt_color_click;
    swcColor alt_color_click_border;

    bool color_swap_click;
    bool color_swap_click_border;

protected:


private:

    void impOnClickOnce();
    void impOnRelease();

    void impPostPaint();

};

}

#endif // SWCBUTTON_H
