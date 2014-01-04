#include <limits.h>
#include <ctime>
#include <GL/GLFW/glfw3.h>

#include "../../../BMfont/BMfont.h"
#include "../../../BMfont/Text2D.h"
#include "../../../Aligner/aligner.h"
#include "../../../ColorCollection/include/colorcollection.h"

#include "../../include/view/utils/swcdefs.h"
#include "../../include/view/utils/swchelper.h"
#include "../../include/view/utils/swcgfx.h"
#include "../../include/view/utils/swccolor.h"
#include "../../include/view/swctextbox.h"

static clock_t timer;
static bool toggle;

namespace swc
{

swcTextBox::swcTextBox() :
     blink_delay(550)
    ,caret_index_row(0)
    ,show_caret(false)
{
    //ctor

    using namespace cc;

    attachParentRect(this);

    setSize(140, 20);

    text_align = ALIGN::LEFT | ALIGN::MIDDLE_Y;

    setBorderColor(ALL_BORDER_SIDE(), getColor(COLORS::GRAY5));

    setColor(ALL_CORNER_SIDE(), getColor(COLORS::WHITE));

    setBorderSize(ALL_BORDER_SIDE(), 1);

    copyRGB(getColor(COLORS::BLACK), &caret_color[0]);

    text_margin_left = 4;

    caretPos.x += text_margin_left;
}

void swcTextBox::impPostPaint()
{

    showText();

    if (show_caret)
    {
        if (toggle)
        {
            glPushMatrix();
                glTranslatef(getX() + caretPos.x, getY() + caretPos.y + 2, getZ());
                glColor4ub(caret_color[0],caret_color[1],caret_color[2],0xff);
                drawLine(getFontHeight() * 0.8);
            glPopMatrix();
        }
    }
}

void swcTextBox::impUpdate()
{
    if (clock() > timer + blink_delay)
    {
        timer = clock();
        toggle = !toggle;
    }
}

void swcTextBox::setFont(const spFont& font)
{
    swcText::setFont(font);
    setHeight(getFontHeight());
}

void swcTextBox::setCaretColor(const rgb color)
{
    copyRGB(color, caret_color);
}

void swcTextBox::setSelection(int start, uint64_t end)
{

}


void swcTextBox::impOnInputText(const Event& event)
{
    std::string temp;
    text += event.key_char;
    temp += event.key_char;
    caretPos.x += Text2D::getWidth(*getFont(), temp) * getFontScale();
    if (caret_index_row < ULONG_MAX)
        ++ caret_index_row;
}

void swcTextBox::impOnInputKey(const Event& event)
{
    if (event.action == GLFW_PRESS || event.action == GLFW_REPEAT)
    {
        switch (event.key_code)
        {
        case GLFW_KEY_BACKSPACE:
            if (text == "") return;
            caretPos.x -= Text2D::getWidth(*getFont(), text.substr(text.length() - 1, text.length() - 1));
            text = text.substr(0, text.length() - 1);
            if (caret_index_row > 0)
                -- caret_index_row;
            break;

        case GLFW_KEY_UP:

            break;
        case GLFW_KEY_DOWN:

            break;
        case GLFW_KEY_LEFT:
            if (caret_index_row > 0)
                -- caret_index_row;
            caretPos.x -= Text2D::getWidth(*getFont(), text.substr(0, caret_index_row)) * font_scale;
            break;
        case GLFW_KEY_RIGHT:
            if (caret_index_row < ULONG_MAX)
                ++ caret_index_row;
            caretPos.x += Text2D::getWidth(*getFont(), text.substr(caret_index_row, 1)) * font_scale;
            break;

        case GLFW_KEY_ENTER:
        case GLFW_KEY_KP_ENTER:
            DEBUG_PRINT(text);
#ifdef DEBUG
            impOnEnter();
#endif // DEBUG
            break;

        case GLFW_KEY_LEFT_CONTROL:
            text += "";
            break;

        }
    }
}

void swcTextBox::impOnClickOnce()
{
    show_caret = true;
    timer = 0;
    toggle = false;
}

void swcTextBox::impOnUnclick()
{
    show_caret = false;
}

void swcTextBox::moveCaretCol(int step)
{
    caretPos.x = text_margin_left;
}

void swcTextBox::moveCaretRow(int step)
{
    for (int i = 0;i < step; ++i)
        caretPos.y += getFontHeight();
}

}
