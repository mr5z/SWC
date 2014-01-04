#include <GL/GLEW/glew.h>

#include <cmath>
#include <vector>
#include <ctime>

#include <boost/thread/once.hpp>

#include "../../../include/view/utils/swcgfx.h"

#include "../../../include/view/swcobject.h"

enum
{
    R,
    G,
    B
};

namespace swc
{

static const uint32_t arcIndices[] =
{
    0 ,
    1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10,
    11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
    21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
    31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
    51, 52, 53, 54, 55, 56, 57, 58, 59, 60,
    61, 62, 63, 64, 65, 66, 67, 68, 69, 70,
    71, 72, 73, 74, 75, 76, 77, 78, 79, 80,
    81, 82, 83, 84, 85, 86, 87, 88, 89, 90,
    91
};

void drawElements(int mode, GLsizei size, GLenum vtype, const GLvoid *vertex, const uint8_t *color, const GLenum *index)
{
    glVertexPointer(2, vtype, 0, vertex);
    glColorPointer(4, GL_UNSIGNED_BYTE, 0, color);
    glDrawElements(mode, size, GL_UNSIGNED_INT, index);
}


void drawArc(int x, int y,
             int startAngle, int endAngle,
             uint32_t radiusX, uint32_t radiusY,
             const rgb color,
             uint8_t opacity)
{
    if (radiusX <= 0 || radiusY <= 0) return;

    constexpr float DTR = 3.14159 / 180;

    float cx, cy;
    int step;

    static std::vector<float> verts;
    static std::vector<uint8_t> colors;

    static float radian_angle_collections_cos[360 + 1];
    static float radian_angle_collections_sin[360 + 1];

    static boost::once_flag flag = BOOST_ONCE_INIT;
    boost::call_once
    (
        []
        {
            for (int i = 0;i <= 360; ++i)
            {
                radian_angle_collections_cos[i] = cos(i * DTR);
                radian_angle_collections_sin[i] = sin(i * DTR);
            }
        }
     , flag
    );

    if (startAngle < endAngle)
    {
        step = +1;
        ++ endAngle;
    } else
    {
        step = -1;
        -- endAngle;
    }

    verts.clear();
    colors.clear();

    verts.push_back(x);
    verts.push_back(y);

    colors.push_back(color[0]);
    colors.push_back(color[G]);
    colors.push_back(color[B]);
    colors.push_back(opacity);

    while (startAngle != endAngle)
    {
        cx = radian_angle_collections_cos[startAngle] * radiusX;
        cy = radian_angle_collections_sin[startAngle] * radiusY;

        verts.push_back(x + cx);
        verts.push_back(y - cy);

        colors.push_back(color[R]);
        colors.push_back(color[G]);
        colors.push_back(color[B]);
        colors.push_back(opacity);

        startAngle += step;
    }

    drawElements(GL_POLYGON, sizeof(arcIndices) / sizeof(arcIndices[0]), GL_FLOAT,
                 &verts[0], &colors[0], &arcIndices[0]);

}

void drawTriRect(bool type, int x, int y, uint32_t w, uint32_t h, const rgb c1, const rgb c2, uint8_t opacity)
{
    static uint32_t verts[12];
    int width = x + w;
    int height = y + h;

    verts[ 0] = x     ; verts[ 1] = y;
    verts[ 2] = width ; verts[ 3] = y;
    verts[ 5] = height; verts[ 6] = x;
    verts[ 9] = height; verts[10] = width;

    if (type)
    {
        verts[ 4] = width;
        verts[ 7] = y;
        verts[ 8] = x;
        verts[11] = height;
    } else
    {
        verts[ 4] = x;
        verts[ 7] = height;
        verts[ 8] = width;
        verts[11] = y;
    }

    const uint8_t colors[]
    {
        c1[R], c1[G], c1[B], opacity,
        c1[R], c1[G], c1[B], opacity,
        c1[R], c1[G], c1[B], opacity,

        c2[R], c2[G], c2[B], opacity,
        c2[R], c2[G], c2[B], opacity,
        c2[R], c2[G], c2[B], opacity
    };

    static const unsigned int index[]
    {
        0, 1, 2, 3, 4, 5
    };

    drawElements(GL_TRIANGLES, sizeof(index) / sizeof(index[0]), GL_INT,
                 &verts[0], &colors[0], &index[0]);
}

void drawTexturedQuad(const swc::swcRectangle &rect)
{
    static const float texcoord[] =
    {
        0, 0,
        1, 0,
        0, 1,
        1, 1,
    };

    static uint32_t verts[8]{0};

    verts[2] = rect.getWidth(),
    verts[5] = rect.getHeight(),
    verts[6] = rect.getWidth(),
    verts[7] = rect.getHeight();

    static const uint32_t index[] { 0, 1, 2, 3 };

    static uint8_t color []
    {
        255, 255, 255, 255,
        255, 255, 255, 255,
        255, 255, 255, 255,
        255, 255, 255, 255
    };

    color[ 3] = rect.getOpacity();
    color[ 7] = rect.getOpacity();
    color[11] = rect.getOpacity();
    color[15] = rect.getOpacity();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glBindTexture(GL_TEXTURE_2D, rect.getTextureID());

    glTexCoordPointer(2, GL_FLOAT, 0, texcoord);

    drawElements(GL_TRIANGLE_STRIP, sizeof(index) / sizeof(index[0]), GL_INT,
                 &verts[0], &color[0], &index[0]);

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glDisable(GL_TEXTURE_2D);
}

void drawCheckMark(float size, float w, float h, float xoffset, float yoffset, float padding, const rgb color, uint8_t opacity)
{
    float verts[] =
    {
        padding, h * 0.4f,
        xoffset, h - size - padding,
        padding, h * 0.4f + size,
        xoffset, h - padding,
        w - padding, yoffset + padding,
        w - padding, yoffset + padding + size
    };

    uint8_t colors[] =
    {
        color[0], color[1], color[2], opacity,
        color[0], color[1], color[2], opacity,
        color[0], color[1], color[2], opacity,
        color[0], color[1], color[2], opacity,
        color[0], color[1], color[2], opacity,
        color[0], color[1], color[2], opacity
    };

    static uint32_t index[] =
    {
        0, 1, 2, 3, 1, 4, 3, 5
    };

    //glEnable(GL_MULTISAMPLE_ARB);
    drawElements(GL_TRIANGLE_STRIP, sizeof(index) / sizeof(index[0]), GL_FLOAT, &verts[0], &colors[0], &index[0]);
    //glDisable(GL_MULTISAMPLE_ARB);
}

void drawLine(int height)
{
    glBegin(GL_LINES);
    glVertex2i(0, 0);
    glVertex2i(0, height);
    glEnd();
}

}
