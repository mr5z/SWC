#include <GL/gl.h>

#include "../../../ColorCollection/include/colorcollection.h"
#include "../../../Aligner/aligner.h"

#include "../../include/controller/swcinputhandler.h"
#include "../../include/view/utils/swchelper.h"
#include "../../include/view/utils/swcdefs.h"
#include "../../include/view/utils/swcgfx.h"
#include "../../include/view/utils/swccolor.h"
#include "../../include/view/swccheckbox.h"

const int DEFAULT_SIZE = 14;

namespace swc
{

swcCheckBox::swcCheckBox() :
    toggleCheck(false)
{
    //ctor
    using namespace cc;

    setSize(DEFAULT_SIZE);
    setOpacity(0);

    box.setColor(ALL_CORNER_SIDE(), getColor(COLORS::WHITE));
    box.setBorderColor(ALL_BORDER_SIDE(), getColor(COLORS::GRAY5));
    box.setAltBorderColor(ALL_BORDER_SIDE(), getColor(COLORS::BLACK));
    box.setBorderSize(ALL_BORDER_SIDE(), 1);
    box.border_color_swap = true;

    marker.setColor(ALL_CORNER_SIDE(), getColor(COLORS::WHITE));
    marker.setBorderColor(ALL_BORDER_SIDE(), getColor(COLORS::YELLOW));
    marker.setOpacity(100);
    marker.hide();

    label.setOpacity(0);
    label.setBorderSize(ALL_BORDER_SIDE(), 0);
    label.text_align = ALIGN::LEFT | ALIGN::MIDDLE_Y;

    setCheckColor(getColor(COLORS::GREEN));

    box.enable = false;
    marker.enable = false;
    label.enable = false;

    addComponent(&box);
    addComponent(&marker);
    addComponent(&label);

    reComputeComponentProperties();
}

void swcCheckBox::setX(int x)
{
    swcObject::setX(x);

    reComputeComponentProperties();
}

void swcCheckBox::setY(int y)
{
    swcObject::setY(y);

    reComputeComponentProperties();
}

void swcCheckBox::setPosition(int x, int y)
{
    swcObject::setPosition(x, y);

    reComputeComponentProperties();
}

void swcCheckBox::setSize(uint32_t size)
{
    swcObject::setSize(size + 4, size + 4);

    box.setSize(size, size);

    marker.setSize(size * 0.5, size * 0.5);

    label.setHeight(size);

    reComputeComponentProperties();
}

void swcCheckBox::setCheckColor(const rgb color)
{
    copyRGB(color, checkColor);
}

void swcCheckBox::setFont(const spFont &font)
{
    label.setFont(font);
    label.setHeight(label.getTextHeight());
    reComputeComponentProperties();
}

void swcCheckBox::setText(const std::string &text)
{
    label.setText(text);
    label.setSize(label.getTextWidth(), label.getTextHeight());
    swcObject::setWidth(box.getTotalWidth() + label.getTotalWidth() + 4);
    reComputeComponentProperties();
}

void swcCheckBox::impSpecialPaint()
{
    if (toggleCheck)
    {
        glPushMatrix();
        glTranslatef(box.getX(), box.getY(), box.getZ());
            drawCheckMark(box.getWidth() * 0.25,
                          box.getWidth(), box.getHeight(),
                          box.getWidth() * 0.42, 0, box.getWidth() * 0.125,
                          checkColor, box.getOpacity());
        glPopMatrix();
    }
}


void swcCheckBox::impOnClickOnce()
{
    toggleCheck = !toggleCheck;
}

void swcCheckBox::impOnHoverOnce()
{
    marker.show();
}

void swcCheckBox::impOnRestOnce()
{
    marker.hide();
}

void swcCheckBox::reComputeComponentProperties()
{
    marker.setBorderSize(B_TOP | B_BOTTOM, box.getHeight() * 0.25);
    marker.setBorderSize(B_LEFT | B_RIGHT, box.getWidth() * 0.25);

    computeXposition
    (
     ALIGN::LEFT | ALIGN::MIDDLE_Y,
     box, getByRef(), 2, 0
    );

    computeXposition
    (
     ALIGN::MIDDLE_X | ALIGN::MIDDLE_Y,
     marker, box
    );

    computeXposition
    (
     ALIGN::LEFT | ALIGN::MIDDLE_Y,
     label, box, box.getWidth() + 2, 0
    );

}

}
