#ifndef SWCGFX_H
#define SWCGFX_H

#include "swcdefs.h"

namespace swc
{

class swcObject;

void drawLine(int height);
void drawCheckMark(float, float, float, float, float, float, const rgb, uint8_t);
void drawElements(int mode, int size, uint32_t vtype, const void *vertex, const uint8_t *color, const uint32_t *index);
void drawTriRect(bool, int, int, uint32_t, uint32_t, const rgb, const rgb, uint8_t);
void drawTexturedQuad(const class swcRectangle&);
void drawArc(int x, int y, int startAngle, int endAngle, uint32_t radiusX, uint32_t radiusY, const rgb color, uint8_t opacity);

}

#endif // SWCGFX_H
