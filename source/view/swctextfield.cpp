#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_EXPOSE_NATIVE_WIN32

#include <GL/GLFW/glfw3.h>
#include <GL/GLFW/glfw3native.h>
#include <ctime>
#include <cmath>

#include "../../../BMfont/Text2D.h"
#include "../../../Aligner/aligner.h"
#include "../../../ColorCollection/include/colorcollection.h"

#include "../../include/view/utils/swchelper.h"
#include "../../include/view/utils/swcgfx.h"
#include "../../include/view/swctextfield.h"

const char NEW_LINE = '\n';

static clock_t timer;

static bool isNewLine(char c)
{
    return c == NEW_LINE;
}

namespace swc
{

//The C A R E T

Caret::Caret() :
     color{0, 0, 0}
    ,blink_time(550)
    ,visible(false)
    ,focus(false)
    ,current_index(0)
{

}

void Caret::blink()
{
    if (focus)
    if (std::clock() > timer + blink_time)
    {
        timer = std::clock();
        visible = !visible;
    }
}

void Caret::drawCaret(uint32_t height, uint8_t opacity, int z)
{
    if (visible)
    {
        glPushMatrix();
            glTranslatef(pos.x, pos.y, z);
            glColor4ub(color.r, color.g, color.b, opacity);
            drawLine(height);
        glPopMatrix();
    }
}

void Caret::moveUp()
{
    textLineRect.pop();
}

void Caret::moveDown()
{
}

void Caret::moveLeft(swcText* txt)
{
    if (current_index == 0) return;

    const char to_string[] { txt->text[--current_index], '\0'};

    if (isNewLine(to_string[0]))
    {
        pos.x = 0;
        moveLine(txt->getFontHeight(), -1);
    } else
    {
        pos.x -= Text2D::getWidth(*txt->getFont(), to_string);
    }
}

void Caret::moveRight(swcText* txt)
{
    if (current_index == txt->text.length()) return;

    const char to_string[] { txt->text[current_index++], '\0'};

    if (isNewLine(to_string[0]))
   {
        pos.x = 0;
        moveLine(txt->getFontHeight(), +1);
   } else
   {
        pos.x += Text2D::getWidth(*txt->getFont(), to_string);
   }
}

void Caret::moveLine(uint64_t height, int step)
{
    for (int i = 0;i < std::abs(step); ++i)
    {
        pos.y += height * step;
    }
}

void Caret::moveNextLine(int height)
{
    Caret::TextLineRect r;
    r.right = pos.x;
    r.index = current_index++;

    textLineRect.push(r);

    pos.y += height;
    pos.x = 0;
}

void Caret::moveToLastLine(const spFont& font, const std::string& line)
{

}

void Caret::moveToEnd(const spFont& font, const std::string& str)
{

}

swcTextField::swcTextField()
{
    setSize(256, 256);
    setColor(ALL_CORNER_SIDE(), cc::getColor(COLORS::WHITE));

    text_align = ALIGN::LEFT | ALIGN::TOP;
    attachParentRect(this);
}

void swcTextField::impUpdate()
{
    caret.blink();
}

void swcTextField::impPostPaint()
{
    drawText();

    glPushMatrix();
    glTranslatef(getX(), getY(), getZ());
    caret.drawCaret(getFontHeight(), opacity, getZ());
    glPopMatrix();
}

void swcTextField::impOnClickOnce()
{
    caret.focus = true;
    caret.visible = true;
    timer = std::clock();
}

void swcTextField::impOnUnclick()
{
    caret.focus = false;
    caret.visible = false;
}

void swcTextField::impOnInputText(const Event& event)
{
    text.insert(caret.current_index, event.key_char);
    caret.moveRight(this);

    timer = std::clock();
    caret.visible = true;
}

void swcTextField::impOnInputKey(const Event& event)
{
    if (event.action == GLFW_RELEASE) return;

    timer = std::clock();
    caret.visible = true;

    switch (event.key_code)
    {
    case GLFW_KEY_BACKSPACE:
        actionOnDelete();
        break;

    case GLFW_KEY_DELETE:
        if (caret.current_index + 1 <= text.length())
        {
            text.erase(caret.current_index, 1);
        }
        break;

    case GLFW_KEY_ENTER:
    case GLFW_KEY_KP_ENTER:
        actionOnEnter();
        break;

    case GLFW_KEY_UP:
        caret.moveUp();
        break;

    case GLFW_KEY_DOWN:
        caret.moveDown();
        break;

    case GLFW_KEY_LEFT:

        caret.moveLeft(this);
        break;

    case GLFW_KEY_RIGHT:

        caret.moveRight(this);
        break;

    }
}

void swcTextField::impOnHover()
{
    SetCursor(LoadCursor(NULL, IDC_IBEAM));
}

void swcTextField::impOnRestOnce()
{

}

void swcTextField::setFont(const spFont& font)
{
    swcText::setFont(font);


}

void swcTextField::actionOnDelete()
{
    if (caret.current_index <= 0) return;

    caret.moveLeft(this);
    text.erase(caret.current_index, 1);
}

void swcTextField::actionOnEnter()
{
    text += NEW_LINE;
    caret.moveNextLine(getFontHeight());
}

}
