#include <GL/GLEW/glew.h>

#include "../../include/view/utils/swchelper.h"
#include "../../include/view/utils/swcgfx.h"
#include "../../include/view/utils/swccolor.h"

#include "../../include/view/swcrectangle.h"

bool BOOL_EN_MSAA = true;

static uint32_t indices[] =
{
    0 , 1 , 9 , 8 ,
    9 , 2 , 3 , 10,
    11, 10, 4 , 5 ,
    6 , 8 , 11, 7 ,
    8 , 9 , 10, 11
};

static const uint32_t outer_border_indices[] =
{
    0 , 1 , 2 , 3 ,
    4 , 5 , 6 , 7 ,
    8 , 9 , 10, 11,
    12, 13, 14, 15
};

namespace swc
{

int swcRectangle::z_index = 0;

swcRectangle::swcRectangle() :
     visible                    (true)
    ,texture_id                 (0)
    ,border_size_top            (0)
    ,border_size_right          (0)
    ,border_size_bottom         (0)
    ,border_size_left           (0)
    ,set_alt_colors_hover       (false)
    ,set_alt_colors_click       (false)
    ,set_border_alt_colors_hover(false)
    ,set_border_alt_colors_click(false)
{
    setOpacity(255);
}

swcRectangle::swcRectangle(const swcRectangle& other) :
     visible                     (other.visible)
    ,texture_id                  (other.texture_id)
    ,border_size_top             (other.border_size_top)
    ,border_size_right           (other.border_size_right)
    ,border_size_bottom          (other.border_size_bottom)
    ,border_size_left            (other.border_size_left)
    ,set_alt_colors_hover        (other.set_alt_colors_hover)
    ,set_alt_colors_click        (other.set_alt_colors_click)
    ,set_border_alt_colors_hover (other.set_border_alt_colors_hover)
    ,set_border_alt_colors_click (other.set_border_alt_colors_click)
{

}

void swcRectangle::draw()
{
    if (!visible) return;

    const uint32_t outer_border_vertices[] =
    {
        //top
        radius_top_left.x, -border_size_top,
        width - radius_top_right.x, -border_size_top,
        width - radius_top_right.x, 0,
        radius_top_left.x, 0,
        //right
        width, radius_top_right.y,
        width + border_size_right, radius_top_right.y,
        width + border_size_right, height - radius_bottom_right.y,
        width, height - radius_bottom_right.y,
        //bottom
        radius_bottom_left.x, height,
        width - radius_bottom_right.x, height,
        width - radius_bottom_right.x, height + border_size_bottom,
        radius_bottom_left.x, height + border_size_bottom,
        //left
        -border_size_left, radius_top_left.y,
        0, radius_top_left.y,
        0, height - radius_bottom_left.y,
        -border_size_left, height - radius_bottom_left.y,
    };

    const uint32_t inner_vertices[] =
    {
        radius_top_left.x, 0,
        width - radius_top_right.x, 0,
        width, radius_top_right.y,
        width, height - radius_bottom_right.y,
        width - radius_bottom_right.x, height,
        radius_bottom_left.x, height,
        0, radius_top_left.y,
        0, height - radius_bottom_left.y,
        radius_top_left.x, radius_top_left.y,
        width - radius_top_right.x, radius_top_right.y,
        width - radius_bottom_right.x, height - radius_bottom_right.y,
        radius_bottom_left.x, height - radius_bottom_left.y,
    };

    //paint behind
    impPrePaint();

    glPushMatrix();

    glTranslatef(getX(), getY(), getZ());

//    glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA);
//    glBlendFunc(GL_ONE, GL_ZERO);

    if (needReOrder())
    {

        //setup stencils
        glClearStencil(0x0);
        glClear(GL_STENCIL_BUFFER_BIT);

        glEnable(GL_STENCIL_TEST);
        glStencilFunc(GL_ALWAYS, 0x0, 0x0);

        glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);

        //draw inner curved corners
        drawInnerCornerCurves(BOOL_EN_MSAA);

        //draw inner rectangle (cover)
        drawElements(GL_QUADS, sizeof(indices) / sizeof(indices[0]), GL_INT,
                     &inner_vertices[0], &colors[0], &indices[0]);

        //draw a texturized quad
        if (hasTexture())
        {
    //        glBlendFuncSeparate(GL_ZERO, GL_ONE, GL_SRC_COLOR, GL_ZERO);
            glBlendFunc(GL_ONE, GL_ZERO);
            drawTexturedQuad(getByRef());
        }

        glStencilFunc(GL_EQUAL, 0x0, 0xff);

        //draw outer curved/solid corners
        drawOuterCornerX(BOOL_EN_MSAA);

         //draw outer side borders
         drawElements(GL_QUADS, sizeof(outer_border_indices) / sizeof(outer_border_indices[0]), GL_INT,
                      &outer_border_vertices[0], &border_colors[0], &outer_border_indices[0]);

        glDisable(GL_STENCIL_TEST);

    } else
    {

         //draw outer side borders
         drawElements(GL_QUADS, sizeof(outer_border_indices) / sizeof(outer_border_indices[0]), GL_INT,
                      &outer_border_vertices[0], &border_colors[0], &outer_border_indices[0]);

        //draw outer curved/solid corners
        drawOuterCornerX(BOOL_EN_MSAA);

        //draw inner curved corners
        drawInnerCornerCurves(BOOL_EN_MSAA);

        //draw inner rectangle (cover)
        drawElements(GL_QUADS, sizeof(indices) / sizeof(indices[0]), GL_INT,
                     &inner_vertices[0], &colors[0], &indices[0]);

        //draw a texturized quad
        if (hasTexture())
        {
            glBlendFunc(GL_ONE, GL_ZERO);
            drawTexturedQuad(getByRef());
        }
    }

    glPopMatrix();

    //paint above the rectangle
    impPostPaint();
    //paint the components
    impShowComponents();
    //paint above all the others
    impSpecialPaint();

}
void swcRectangle::drawInnerCornerCurves(bool en_multisample)
{

    (en_multisample) ? glEnable(GL_MULTISAMPLE_ARB) : glDisable(GL_MULTISAMPLE_ARB);

    //border-radius-top-left
    drawArc(radius_top_left.x, radius_top_left.y, 180, 90,
            radius_top_left.x, radius_top_left.y,
            &colors[0], opacity);
    //border-radius-top-right
    drawArc(width - radius_top_right.x, radius_top_right.y, 90, 0,
            radius_top_right.x, radius_top_right.y,
            &colors[4], opacity);
    //border-radius-bottom-right
    drawArc(width - radius_bottom_right.x, height - radius_bottom_right.y, 270, 360,
            radius_bottom_right.x, radius_bottom_right.y,
            &colors[12], opacity);
    //border-radius-bottom-left
    drawArc(radius_bottom_left.x, height - radius_bottom_left.y, 180, 270,
            radius_bottom_left.x, radius_bottom_left.y,
            &colors[20], opacity);
    if (en_multisample) glDisable(GL_MULTISAMPLE_ARB);
}

void swcRectangle::drawOuterCornerX(bool en_multisample)
{
    (en_multisample) ?  glEnable(GL_MULTISAMPLE_ARB) : glDisable(GL_MULTISAMPLE_ARB);

    //outer-border-corner-top-left
    if (radius_top_left == 0)
        drawTriRect(true,
            -border_size_left, -border_size_top, border_size_left, border_size_top,
            &border_colors[0], &border_colors[48], opacity);
    else
        drawArc(radius_top_left.x, radius_top_left.y, 180, 90,
            radius_top_left.x + border_size_left, radius_top_left.y + border_size_top,
            &border_colors[0], opacity);

    //outer-border-corner-top-right
    if (radius_top_right == 0)
        drawTriRect(false,
            width, -border_size_top, border_size_right, border_size_top,
            &border_colors[0], &border_colors[16], opacity);
    else
        drawArc(width - radius_top_right.x, radius_top_right.y, 90, 0,
            radius_top_right.x + border_size_right, radius_top_right.y + border_size_top,
            &border_colors[4], opacity);

    //outer-border-corner-bottom-right
    if (radius_bottom_right == 0)
        drawTriRect(true,
            width, height, border_size_right, border_size_bottom,
            &border_colors[16], &border_colors[32], opacity);
    else
        drawArc(width - radius_bottom_right.x, height - radius_bottom_right.y, 270, 360,
            radius_bottom_right.x + border_size_right, radius_bottom_right.y + border_size_bottom,
            &border_colors[12],opacity);

    //outer-border-corner-bottom-left
    if (radius_bottom_left == 0)
    drawTriRect(false,
            -border_size_left, height, border_size_left, border_size_bottom,
            &border_colors[48], &border_colors[32],
            opacity);
    else
        drawArc(radius_bottom_left.x, height - radius_bottom_left.y, 180, 270,
            radius_bottom_left.x + border_size_left, radius_bottom_left.y + border_size_bottom,
            &border_colors[20], opacity);

    if (en_multisample) glDisable(GL_MULTISAMPLE_ARB);
}

void swcRectangle::setOpacity(uint8_t opacity, int flag) {
    this->opacity = opacity;

    if (flag & ALL_BORDER_SIDE())
    for (int i = 3; i < (4 * 4 * 4); i += 4)
    {
        if (i < (4 * 4 * 3))
        {
            colors[i] = opacity;
        alt_colors[i] = opacity;
        }
        border_colors[i] = opacity;
    alt_border_colors[i] = opacity;
    } else
    {
        switch (flag)
        {
        case B_TOP:
            break;
        case B_RIGHT:
            break;
        case B_BOTTOM:
            break;
        case B_LEFT:
            break;
        }
    }
}

void swcRectangle::setColor(CORNER_LOCATION corner, const swcColor &color)
{
    switch (corner)
    {
    case C_NW:
        {
        copyRGB(color, &colors[ 0]);
        copyRGB(color, &colors[24]);
        copyRGB(color, &colors[32]);
        _colors.nw = color;
        } break;
    case C_NE:
        {
        copyRGB(color, &colors[ 4]);
        copyRGB(color, &colors[ 8]);
        copyRGB(color, &colors[36]);
        _colors.ne = color;
        } break;
    case C_SE:
        {
        copyRGB(color, &colors[12]);
        copyRGB(color, &colors[16]);
        copyRGB(color, &colors[40]);
        _colors.se = color;
        } break;
    case C_SW:
        {
        copyRGB(color, &colors[20]);
        copyRGB(color, &colors[28]);
        copyRGB(color, &colors[44]);
        _colors.sw = color;
        } break;
    }
}

void swcRectangle::setColor(int flag, const swcColor &color)
{
    if (flag & C_NW) setColor(C_NW, color);
    if (flag & C_NE) setColor(C_NE, color);
    if (flag & C_SE) setColor(C_SE, color);
    if (flag & C_SW) setColor(C_SW, color);
}


void swcRectangle::setAltColor(CORNER_LOCATION corner, const swcColor &color) {

    switch (corner)
    {
    case C_NW:
        {
        copyRGB(color, &alt_colors[ 0]);
        copyRGB(color, &alt_colors[24]);
        copyRGB(color, &alt_colors[32]);
        }

        break;
    case C_NE:
        {
        copyRGB(color, &alt_colors[ 4]);
        copyRGB(color, &alt_colors[ 8]);
        copyRGB(color, &alt_colors[36]);
        }

        break;
    case C_SE:
        {
        copyRGB(color, &alt_colors[12]);
        copyRGB(color, &alt_colors[16]);
        copyRGB(color, &alt_colors[40]);
        }

        break;
    case C_SW:
        {
        copyRGB(color, &alt_colors[20]);
        copyRGB(color, &alt_colors[28]);
        copyRGB(color, &alt_colors[44]);
        }

        break;
    }
}

void swcRectangle::setAltColor(int flag, const swcColor &color)
{
    if (flag & C_NE) setAltColor(C_NE, color);
    if (flag & C_NW) setAltColor(C_NW, color);
    if (flag & C_SE) setAltColor(C_SE, color);
    if (flag & C_SW) setAltColor(C_SW, color);
}

void swcRectangle::setBorderColor(BORDER_SIDE side, const swcColor &color) {
    switch (side)
    {
    case B_TOP:
        {
        copyRGB(color, &border_colors[ 0]);
        copyRGB(color, &border_colors[ 4]);
        copyRGB(color, &border_colors[ 8]);
        copyRGB(color, &border_colors[12]);
        }
        break;
    case B_RIGHT:
        {
        copyRGB(color, &border_colors[16]);
        copyRGB(color, &border_colors[20]);
        copyRGB(color, &border_colors[24]);
        copyRGB(color, &border_colors[28]);
        }
        break;
    case B_BOTTOM:
        {
        copyRGB(color, &border_colors[32]);
        copyRGB(color, &border_colors[36]);
        copyRGB(color, &border_colors[40]);
        copyRGB(color, &border_colors[44]);
        }
        break;
    case B_LEFT:
        {
        copyRGB(color, &border_colors[48]);
        copyRGB(color, &border_colors[52]);
        copyRGB(color, &border_colors[56]);
        copyRGB(color, &border_colors[60]);
        }
        break;
    }
}

void swcRectangle::setBorderColor(int flag, const swcColor &color)
{
    if (flag & B_TOP)    setBorderColor(B_TOP, color);
    if (flag & B_RIGHT)  setBorderColor(B_RIGHT, color);
    if (flag & B_BOTTOM) setBorderColor(B_BOTTOM, color);
    if (flag & B_LEFT)   setBorderColor(B_LEFT, color);
}

void swcRectangle::setAltBorderColor(BORDER_SIDE side, const swcColor &color) {
    switch (side)
    {
    case B_TOP:
        {
        copyRGB(color, &alt_border_colors[ 0]);
        copyRGB(color, &alt_border_colors[ 4]);
        copyRGB(color, &alt_border_colors[ 8]);
        copyRGB(color, &alt_border_colors[12]);
        }
        break;
    case B_RIGHT:
        {
        copyRGB(color, &alt_border_colors[16]);
        copyRGB(color, &alt_border_colors[20]);
        copyRGB(color, &alt_border_colors[24]);
        copyRGB(color, &alt_border_colors[28]);
        }
        break;
    case B_BOTTOM:
        {
        copyRGB(color, &alt_border_colors[32]);
        copyRGB(color, &alt_border_colors[36]);
        copyRGB(color, &alt_border_colors[40]);
        copyRGB(color, &alt_border_colors[44]);
        }
        break;
    case B_LEFT:
        {
        copyRGB(color, &alt_border_colors[48]);
        copyRGB(color, &alt_border_colors[52]);
        copyRGB(color, &alt_border_colors[56]);
        copyRGB(color, &alt_border_colors[60]);
        }
        break;
    }
}
void swcRectangle::setAltBorderColor(int flag, const swcColor &color)
{
    if (flag & B_TOP)    setAltBorderColor(B_TOP, color);
    if (flag & B_RIGHT)  setAltBorderColor(B_RIGHT, color);
    if (flag & B_BOTTOM) setAltBorderColor(B_BOTTOM, color);
    if (flag & B_LEFT)   setAltBorderColor(B_LEFT, color);
}
void swcRectangle::setBorderSize(BORDER_SIDE side, uint32_t size)
{
    switch (side)
    {
    case B_TOP:     border_size_top     = size; break;
    case B_RIGHT:   border_size_right   = size; break;
    case B_BOTTOM:  border_size_bottom  = size; break;
    case B_LEFT:    border_size_left    = size; break;
    }
}

void swcRectangle::setBorderSize(int flag, uint32_t size)
{
    if (flag & B_TOP)    setBorderSize(B_TOP, size);
    if (flag & B_BOTTOM) setBorderSize(B_BOTTOM, size);
    if (flag & B_RIGHT)  setBorderSize(B_RIGHT, size);
    if (flag & B_LEFT)   setBorderSize(B_LEFT, size);
}

void swcRectangle::setBorderRadius(CORNER_LOCATION location, int x, int y)
{
    if (x > width * 0.5) x = width * 0.5;
    if (y > height * 0.5) y = height * 0.5;

    switch (location)
    {
    case C_NW:
        radius_top_left(x, y);
        break;
    case C_NE:
        radius_top_right(x, y);
        break;
    case C_SE:
        radius_bottom_right(x, y);
        break;
    case C_SW:
        radius_bottom_left(x, y);
        break;
    }
}

void swcRectangle::setBorderRadius(int flag, int x, int y)
{
    if (flag & C_NW) setBorderRadius(C_NW, x, y);
    if (flag & C_NE) setBorderRadius(C_NE, x, y);
    if (flag & C_SE) setBorderRadius(C_SE, x, y);
    if (flag & C_SW) setBorderRadius(C_SW, x, y);
}

}
