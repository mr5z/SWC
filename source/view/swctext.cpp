#include <GL/GLEW/glew.h>

#include "../../../BMfont/BMfont.h"
#include "../../../BMfont/Text2D.h"
#include "../../../Aligner/aligner.h"

#include "../../include/controller/swcwindowhandler.h"
#include "../../include/view/utils/swchelper.h"
#include "../../include/view/swcrectangle.h"
#include "../../include/view/swctext.h"

namespace swc
{

swcText::swcText():
     text_align         (ALIGN::MIDDLE_X | ALIGN::MIDDLE_Y)
    ,text_margin_top    (0)
    ,text_margin_right  (0)
    ,text_margin_bottom (0)
    ,text_margin_left   (0)
    ,font_scale (1)
{

}

swcText::swcText(const swcText &other) :
     text_align (other.text_align)
    ,text       (other.text)
    ,font       (other.font)
    ,font_scale (other.font_scale)
    ,rect       (other.rect)
{

}

void swcText::attachParentRect(swcRectangle *rect) {
    this->rect = rect;
}

spFont swcText::getFont() const {
    return font;
}

int swcText::getFontHeight() const {
    return font->lineHeight * font_scale;
}

void swcText::setText(const std::string& text)
{
    this->text = text;
    computeTextSize();
}

void swcText::showText()
{
    if (font == nullptr) return;

    int text_pos_x;
    int text_pos_y;

    computePosition(text_align,
                    text_pos_x, text_pos_y,
                    text_width, text_height,
                    rect->getX(), rect->getY(),
                    rect->getWidth(), rect->getHeight());

    glPushMatrix();
        glTranslatef((text_pos_x + text_margin_left) - text_margin_right,
                     (text_pos_y + text_margin_top) - text_margin_bottom, 0);
        glScalef(font_scale, font_scale, font_scale);
        Text2D::printText(*font, text);
    glPopMatrix();
}

uint32_t swcText::getTextWidth() const {
    return (font == nullptr) ?
    0 : Text2D::getWidth(*font, text) * font_scale;
}

uint32_t swcText::getTextHeight() const {
    return (font == nullptr) ?
    0 : Text2D::getHeight(*font, text) * font_scale;
}

void swcText::computeTextSize()
{
    text_width = getTextWidth();
    text_height = getTextHeight();
}

}
