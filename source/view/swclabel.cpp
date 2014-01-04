#include <GL/gl.h>

#include "../../../ColorCollection/include/colorcollection.h"
#include "../../../BMfont/BMfont.h"
#include "../../../BMfont/Text2D.h"
#include "../../../Aligner/aligner.h"

#include "../../include/controller/swcwindowhandler.h"
#include "../../include/view/utils/swccolor.h"
#include "../../include/view/swclabel.h"

namespace swc
{

swcLabel::swcLabel()
{
    //ctor

    using namespace cc;

    attachParentRect(this);

    setSize(80, 20);

    setColor(ALL_CORNER_SIDE(), getColor(COLORS::WHITE));

    setBorderColor(ALL_BORDER_SIDE(), getColor(COLORS::GRAY5));

    setBorderSize(ALL_BORDER_SIDE(), 1);

}

void swcLabel::impPostPaint()
{
//    int scissorX = getX();
//    int scissorY = swcWindowHandler::getWinHeight() - getY() - getHeight();
//    int scissorW = getWidth();
//    int scissorH = getHeight();
//
//    glPushAttrib(GL_SCISSOR_BIT);

    //glScissor(scissorX, scissorY, scissorW, scissorH);

    setSize(getTextWidth(), getTextHeight());

    showText();

    //glPopAttrib();
}

}
