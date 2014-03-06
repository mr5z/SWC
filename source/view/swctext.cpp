#include <assert.h>

#include "XAligner/XAligner.h"
#include "FTGL/ftgl.h"

#include "swcdefaults.h"
#include "swcgfx.h"
#include "swcrectangle.h"
#include "swctext.h"

using namespace swc;

swcText::
    swcText(const std::string &text, swcRectangle *rect) :
///-----------
     rect                   (rect)
    ,font_scale             (1)
    ,font_face_size         (12)
    ,font_face_resolution   (100)
    ,font                   (getDefaultFont("fonts\\AlegreyaSans-Regular.otf",
                                             font_face_size,
                                             font_face_resolution))
    ,text_offset            (new FTPoint())
    ,text_spacing           (new FTPoint())
    ,text_align             (ALIGN::MIDDLE_X | ALIGN::MIDDLE_Y)
    ,text_alpha             (0xff)
    ,text                   (text)
{
    validateTextLayout();
}

swcText::
    ~swcText()
///-----------
{
    //I'm willing to sacrifice the stack allocation
    //just to not include in the header the FTGL !
    delete text_offset;
    delete text_spacing;
}

const pFont
swcText::
    getFont() const
///-----------
{
    return font;
}

float
swcText::
    getFontScale() const
///-----------
{
    return font_scale;
}

void
swcText::
    setText(const std::string& text)
///-----------
{
//    if (this->text == text) return;

    this->text = text;

    validateTextLayout();
}

void
swcText::
    setTextAlign(int text_align)
///-----------
{
    this->text_align = text_align;

    computeTextAlignment();
}

void
swcText::
    setTextColor(const swcRGB text_color)
///-----------
{
    this->text_color = text_color;
}

void
swcText::
    setFont(const pFont font)
///-----------
{

    this->font = font;

    validateTextLayout();
}

void
swcText::
    setFontScale(float font_scale)
///-----------
{
    this->font_scale = font_scale;

    validateTextLayout();
}

void
swcText::
    drawText()
///-----------
{
    assert(font != nullptr);

    if (!isFontInit() || text.empty()) return;

    pushMatrix();

    //apply vertex attributes

    moveTo( text_pos.x ,text_pos.y );
    glRasterPos2f( text_pos.x , text_pos.y );
    scaleBy( font_scale, -font_scale );
    fillColor( text_color, text_alpha );

//    glEnable(GL_MULTISAMPLE);

    font->Render
    (
       text.c_str()
     , text.length()
     , *text_offset
     , *text_spacing
     , FTGL::RenderMode::RENDER_FRONT
    );

//    glDisable(GL_MULTISAMPLE);

    popMatrix();
}

float
swcText::
    getTextWidth() const
///-----------
{
    if (!isFontInit()) return 0;

    FTBBox box = font->BBox(text.c_str(), text.length(), *text_offset, *text_spacing);

    return (box.Upper().X() - box.Lower().X()) * font_scale;
}

float
swcText::
    getTextHeight() const
///-----------
{
    if (!isFontInit()) return 0;

    FTBBox box = font->BBox(text.c_str());

    return (box.Upper().Y() - box.Lower().Y()) * font_scale;
}

swcPoint<swcText::Type>
swcText::
    getTextPos() const
///-----------
{
    return text_pos;
}

bool
swcText::
    isFontInit() const
///-----------
{
    return font != nullptr;
}

const std::string&
swcText::
    getText() const
///-----------
{
    return text;
}

void
swcText::
    attachToRect(class swcRectangle *rect)
///-----------
{
    this->rect = rect;
}

void
swcText::
    validateTextLayout()
///-----------
{

    if (rect == nullptr || text.empty()) return;

    assert(rect != nullptr);

    text_size = { getTextWidth(), getTextHeight() };

    if ( rect->getWidth() < text_size.width )
        rect->setWidth( text_size.width );
    if ( rect->getHeight() < text_size.height )
        rect->setHeight( text_size.height );

    computeTextAlignment();

    dbgPrint("wasted!");
}

void
swcText::
    computeTextAlignment()
///-----------
{
    X::AlignRect
    (
    text_align,
    text_pos.x, text_pos.y,
    text_size.width, text_size.height,
#ifdef SWC_SIZE_TYPE_IS_FLOAT
    rect->getWidth(), rect->getHeight()
#else
    static_cast<float>(rect->getWidth()), static_cast<float>(rect->getHeight())
#endif
    );

    float x_offset = font->BBox(text.c_str()).Lower().X();
    float y_offset = font->BBox(text.c_str()).Upper().Y();

    text_pos.x -= x_offset * font_scale;
    text_pos.y += y_offset * font_scale;
}
