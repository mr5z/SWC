#ifndef SWCTEXT_H
#define SWCTEXT_H

#include <memory>
#include <string>

typedef std::shared_ptr< class BMfont> spFont;

namespace swc
{

class swcText
{
public:
    swcText();
    swcText(const swcText &other);

    virtual void setText(const std::string& text);

    virtual inline void setFont(const spFont &font) {
        this->font = font;
        computeTextSize();
    }

    inline void setFontScale(float font_scale) {
        this->font_scale = font_scale;
        computeTextSize();
    }

    inline const std::string& getText() {
        return text;
    }

    spFont getFont() const;

    inline float getFontScale() const {
        return font_scale;
    }

    int getFontHeight() const;

    uint32_t getTextWidth() const;
    uint32_t getTextHeight() const;

    int text_align;
    int text_margin_top;
    int text_margin_right;
    int text_margin_bottom;
    int text_margin_left;

protected:

    void showText();

    void attachParentRect(swcRectangle *rect);

    void computeTextSize();

    std::string text;

    uint32_t text_width;
    uint32_t text_height;

    spFont font;

    float font_scale;

    class swcRectangle *rect;
};

}

//std::string& operator<<(std::string & lhs, const std::string& other) {
//    return lhs += other;
//}
//

#endif // SWCTEXT_H
