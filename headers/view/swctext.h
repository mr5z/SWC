#ifndef SWCTEXT_H
#define SWCTEXT_H

#include <string>

#include "swcpoint.h"
#include "swcsize.h"
#include "swcrgb.h"

class FTPoint;
class FTFont;

namespace swc
{

class swcRectangle;

using pFont = TPtr<FTFont>;

class swcText
{

using Type = float;

public:

    swcText(const std::string &text, swcRectangle *rect);

    virtual ~swcText();

    ///sets text and invalidate
    ///I made this virtual so I can use it in text field (editable text input)
    virtual void
    setText(const std::string &text);

    void ///blah blah blah
    setTextAlign(int text_align);

    void ///sets the color
    setTextColor(const swcRGB text_color);

    void ///sets font and invalidate
    setFont(const pFont font);

    void ///sets font scale and invalidate
    setFontScale(Type font_scale);

    bool ///returns true if sets the font-face size
    setFontFaceSize(uint32_t face_size, uint32_t face_resolution);

    const pFont ///gets current font
    getFont() const;

    Type ///gets current font scale
    getFontScale() const;

    Type ///return largest width in multi-lined
    getTextWidth() const;

    Type ///gets text height
    getTextHeight() const;

    swcPoint<Type> ///returns current position
    getTextPos() const;

    bool ///checks if font is valid
    isFontInit() const;

    ///returns a constant reference of text
    const std::string&
    getText() const;

    void ///attached this text to corresponding rectangle
    attachToRect(class swcRectangle *rect);

protected:

    void ///specialized method to draw our text!
    drawText();

    void ///re-validate
    validateTextLayout();

    void ///re-align!
    computeTextAlignment();

    ///bounding box to be based off
    ///when aligning
    TPtr<swcRectangle> rect;

    ///font section
    Type        font_scale;
    uint32_t    font_face_size;
    uint32_t    font_face_resolution;
    pFont       font;

    ///text section
    swcPoint<Type>  text_pos;
    swcSize<Type>   text_size;
    TPtr<FTPoint>   text_offset;
    TPtr<FTPoint>   text_spacing;
    int             text_align;
    swcRGB          text_color;
    ubyte           text_alpha;
    std::string     text;

};

}

#endif // SWCTEXT_H
