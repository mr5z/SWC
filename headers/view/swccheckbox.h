#ifndef SWCCHECKBOX_H
#define SWCCHECKBOX_H

#include "swcobject.h"
#include "swclabel.h"

namespace swc
{

class swcCheckBox :
    public swcObject
{
public:
    swcCheckBox();

    void setX(int x);
    void setY(int y);
    void setPosition(int x, int y);

    void setSize(uint32_t size);

    void setCheckColor(const rgb color);

    void setFont(const spFont &font);

    void setText(const std::string &text);

    inline void setFontScale(float font_scale) {
        label.setFontScale(font_scale);
    }

    inline bool &checked() {
        return toggleCheck;
    }

protected:
private:

    void setWidth(uint32_t width) { }
    void setHeight(uint32_t height) { }
    void setSize(uint32_t width, uint32_t height) { }

    void impSpecialPaint();
    void impOnClickOnce();
    void impOnHoverOnce();
    void impOnRestOnce();

    void reComputeComponentProperties();

    swcObject box;
    swcObject marker;
    swcLabel label;

    rgb checkColor;
    bool toggleCheck;
};

}

#endif // SWCCHECKBOX_H
