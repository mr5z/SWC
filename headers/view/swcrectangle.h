#ifndef SWCRECTANGLE_H
#define SWCRECTANGLE_H

#include "../../include/view/utils/swcdefs.h"
#include "../view/utils/swccornercolors.h"
#include "../view/utils/swcPoint.h"

namespace swc
{

class swcRectangle :
    public swcPoint<int>
{
public:

    swcRectangle();
    swcRectangle(const swcRectangle& other);
    virtual ~swcRectangle() { }

    void draw();

    virtual const inline swcRectangle& getByRef() const {
        return *this;
    }

    inline int getX() const {
        return x;
    }

    inline int getY() const {
        return y;
    }

    static inline int getZ() {
        return z_index;
    }

    inline uint32_t getWidth() const {
        return width;
    }

    inline uint32_t getHeight() const {
        return height;
    }

    inline int getRight() const {
        return x + width;
    }

    inline int getBottom() const {
        return y + height;
    }

    inline int getPosTop() const {
        return y - border_size_top;
    }

    inline int getPosLeft() const {
        return x - border_size_left;
    }

    inline int getPosBottom() const {
        return y + height + border_size_bottom;
    }

    inline int getPosRight() const {
        return x + width + border_size_right;
    }

    inline int getPosMidX() const {
        return x + width * 0.5;
    }

    inline int getPosMidY() const {
        return y + height * 0.5;
    }

    inline int getTotalWidth() const {
        return border_size_left + width + border_size_right;
    }

    inline int getTotalHeight() const {
        return border_size_top + height + border_size_bottom;
    }

    inline uint8_t getOpacity() const {
        return opacity;
    }

    inline uint32_t getTextureID() const {
        return texture_id;
    }

    virtual inline void setX(int x) {
        this->x = x;
    }

    virtual inline void setY(int y) {
        this->y = y;
    }

    static inline void setZ(int z_index) {
        z_index = z_index;
    }

    virtual inline void setWidth(uint32_t width) {
        this->width = width;
    }

    virtual inline void setHeight(uint32_t height) {
        this->height = height;
    }

    virtual inline void setPosition(int x, int y) {
        this->x = x;
        this->y = y;
    }

    virtual inline void setSize(uint32_t width, uint32_t height) {
        this->width = width;
        this->height = height;
    }

    void setBorderSize(BORDER_SIDE location, uint32_t size);
    void setBorderSize(int flag, uint32_t size);

    void setBorderRadius    (CORNER_LOCATION location, int x, int y);
    void setColor           (CORNER_LOCATION corner, const class swcColor &color);
    void setAltColor        (CORNER_LOCATION corner, const class swcColor &color);
    void setBorderColor     (BORDER_SIDE location, const class swcColor &color);
    void setAltBorderColor  (BORDER_SIDE location, const class swcColor &color);

    void setBorderRadius    (int flag, int x, int y);
    void setColor           (int flag, const class swcColor &color);
    void setAltColor        (int flag, const class swcColor &color);
    void setBorderColor     (int flag, const class swcColor &color);
    void setAltBorderColor  (int flag, const class swcColor &color);

    void setOpacity(uint8_t opacity, int flag = ALL_BORDER_SIDE());

    inline void setTextureID(uint32_t texture_id) {
        this->texture_id = texture_id;
    }

    static int z_index;
    bool visible;

protected:

    virtual void impShowComponents() { }

    inline virtual void impPrePaint    () { }
    inline virtual void impPostPaint   () { }
    inline virtual void impSpecialPaint() { }

    std::array<uint8_t, 4 * 4 * 3> colors;
    std::array<uint8_t, 4 * 4 * 4> border_colors;
    std::array<uint8_t, 4 * 4 * 3> alt_colors;
    std::array<uint8_t, 4 * 4 * 4> alt_border_colors;

    swcPoint<uint32_t> radius_top_right;
    swcPoint<uint32_t> radius_top_left;
    swcPoint<uint32_t> radius_bottom_right;
    swcPoint<uint32_t> radius_bottom_left;

    swcCornerColors _colors;
    swcCornerColors _alt_colors;
    swcCornerColors _border_color;
    swcCornerColors _border_alt_color;

    uint32_t width;
    uint32_t height;

    uint32_t texture_id;

    uint32_t border_size_top;
    uint32_t border_size_right;
    uint32_t border_size_bottom;
    uint32_t border_size_left;

    bool set_alt_colors_hover;
    bool set_alt_colors_click;
    bool set_border_alt_colors_hover;
    bool set_border_alt_colors_click;

    uint8_t opacity;

private:

    inline bool hasTexture() const {
        return texture_id != 0;
    }

    inline bool needReOrder() const {
        return opacity != UCHAR_MAX;
    }

    void drawInnerCornerCurves(bool en_multisample);
    void drawOuterCornerX(bool en_multisample);

};

}

#endif // SWCRECTANGLE_H
