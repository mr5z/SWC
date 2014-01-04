#include "../../../ColorCollection/include/colorcollection.h"
#include "../../../Aligner/aligner.h"

#include "../../include/controller/swcinputhandler.h"
#include "../../include/view/utils/swchelper.h"
#include "../../include/view/utils/swccolor.h"
#include "../../include/view/swcscrollbar.h"

#include <iostream>

const uint32_t DEFAULT_SIZE = 18;

namespace swc
{

//base-scrollbar
swcScrollbar::swcScrollbar() :
    objects(nullptr),
    distancePos(-1),
    diffPos(0),
    button_padding(4),
    step(10)
{
    //ctor

    using namespace cc;

    setColor(ALL_CORNER_SIDE(), getColor(COLORS::GRAY7));
    setBorderColor(ALL_BORDER_SIDE(), getColor(COLORS::WHITE));
    setBorderSize(ALL_BORDER_SIDE(), 1);

    btnA.setColor(ALL_CORNER_SIDE(), getColor(COLORS::GRAY1));
    btnA.setBorderColor(ALL_BORDER_SIDE(), getColor(COLORS::WHITE));
    btnA.onClick = [this]()
    {
        scrollPrev(getStep());
    };

    btnB.setColor(ALL_CORNER_SIDE(), getColor(COLORS::GRAY1));
    btnB.setBorderColor(ALL_BORDER_SIDE(), getColor(COLORS::WHITE));
    btnB.onClick = [this]()
    {
        scrollNext(getStep());
    };

    thumb.setBorderColor(ALL_BORDER_SIDE(), getColor(COLORS::WHITE));
    thumb.setSize(DEFAULT_SIZE - button_padding, DEFAULT_SIZE - button_padding);

    addComponent(&btnA);
    addComponent(&btnB);
    addComponent(&thumb);
}

void swcScrollbar::setThumbBorderSize(int flag, uint32_t size)
{
    thumb.setBorderSize(flag, size);
}

void swcScrollbar::setThumbBorderRadius(int flag, int x, int y)
{
    thumb.setBorderRadius(flag, x - button_padding, y - button_padding);
}

void swcScrollbar::setButtonABorderSize(int flag, uint32_t size)
{
    btnA.setBorderSize(flag, size);
}

void swcScrollbar::setButtonBBorderSize(int flag, uint32_t size)
{
    btnB.setBorderSize(flag, size);
}

void swcScrollbar::setButtonABorderRadius(int flag, int x, int y)
{
    btnA.setBorderRadius(flag, x - button_padding, y - button_padding);
}

void swcScrollbar::setButtonBBorderRadius(int flag, int x, int y)
{
    btnB.setBorderRadius(flag, x - button_padding, y - button_padding);
}

//vertical-scrollbar
swcVScrollbar::swcVScrollbar()
{
    setSize(DEFAULT_SIZE + 2, 200);

    thumb.setColor(C_NW | C_SW, cc::getColor(COLORS::GRAY5));
    thumb.setColor(C_NE | C_SE, cc::getColor(COLORS::GRAY1));

    thumb.onClickOnce = [this]
    {
        diffPos = swcMouse::instance().y - thumb.getY();
    };

    thumb.onClick = [this]
    {
        int newY = (swcMouse::instance().y - diffPos) - thumb.getY();

        if (newY < 0)
            scrollPrev(-newY);
        else if (newY > 0)
            scrollNext(+newY);
    };
}

void swcVScrollbar::impOnClick()
{
    if (swcMouse::instance().lastClickPoint.y < thumb.getPosMidY())
        scrollPrev(getStep());
    else if (swcMouse::instance().lastClickPoint.y > thumb.getPosMidY())
        scrollNext(getStep());
}

void swcVScrollbar::setX(int x)
{
    swcObject::setX(x);

    reComputeComponentProperties();
}

void swcVScrollbar::setY(int y)
{
    swcObject::setY(y);

    reComputeComponentProperties();
}

void swcVScrollbar::setPosition(int x, int y)
{
    swcObject::setPosition(x, y);

    reComputeComponentProperties();
}

void swcVScrollbar::setWidth(uint32_t width)
{
    swcObject::setWidth(width);

    btnA.setWidth(width - button_padding);
    btnB.setWidth(width - button_padding);
    thumb.setWidth(width - button_padding);

    reComputeComponentProperties();
}

void swcVScrollbar::setHeight(uint32_t height)
{
    swcObject::setHeight(height);

    reComputeComponentProperties();
}

void swcVScrollbar::setSize(uint32_t width, uint32_t height)
{
    swcObject::setSize(width, height);

    btnA.setSize(width - button_padding, width - button_padding);
    btnB.setSize(width - button_padding, width - button_padding);
    thumb.setWidth(width - button_padding);

    reComputeComponentProperties();
}

void swcVScrollbar::setSize(uint32_t size)
{
    setSize(DEFAULT_SIZE, size);
}

void swcVScrollbar::scrollPrev(int newY)
{
    int y1 = thumb.getY();
    int y2 = thumb.getY() - newY;

    while (canMovePrev())
    {
        thumb.setY(y1--);
        if (y1 == y2 || !canMovePrev())
        {
            funcRun(onScrollPrev);
            break;
        }

        if (isSet())
        {
            for (auto &i : *objects)
            {
                int newY = i->getY() + 1;

                i->setY(newY);
            }
        }
    }

    saveThumbLocation();
}

void swcVScrollbar::scrollNext(int newY)
{
    int y1 = thumb.getY();
    int y2 = thumb.getY() + newY;

    while (canMoveNext())
    {
        thumb.setY(y1++);
        if (y1 == y2 || !canMoveNext())
        {
            funcRun(onScrollNext);
            break;
        }

        if (isSet())
        {
            for (auto &i : *objects)
            {
                int newY = i->getY() - 1;

                i->setY(newY);
            }
        }
    }

    saveThumbLocation();
}

bool swcVScrollbar::canMovePrev() const
{
    return thumb.getPosTop() > btnA.getPosBottom() + 1;
}

bool swcVScrollbar::canMoveNext() const
{
    return thumb.getPosBottom() < btnB.getPosTop() - 1;
}

void swcVScrollbar::reComputeComponentProperties()
{
    computeXposition
    (
    ALIGN::MIDDLE_X | ALIGN::TOP,
    btnA, getByRef(), 0, +(button_padding * 0.5)
    );

    computeXposition
    (
    ALIGN::MIDDLE_X | ALIGN::BOTTOM,
    btnB, getByRef(), 0, -(button_padding * 0.5)
    );

    computeXposition
    (
    ALIGN::MIDDLE_X | ALIGN::TOP,
    thumb, getByRef(), 0, btnA.getTotalHeight() + 1 + (button_padding * 0.5)
    );

    loadThumbLocation();

}

void swcVScrollbar::setThumbSize(uint32_t size)
{
    if (size < getThumbMaxSize())
        thumb.setHeight(size);
}

//horizontal-scrollbar
swcHScrollbar::swcHScrollbar()
{
    setSize(200, DEFAULT_SIZE + 2);

    thumb.setColor(C_NW | C_NE, cc::getColor(COLORS::GRAY5));
    thumb.setColor(C_SE | C_SW, cc::getColor(COLORS::GRAY1));

    thumb.onClickOnce = [this]()
    {
        diffPos = swcMouse::instance().x - thumb.getX();
    };

    thumb.onClick = [this]()
    {
        int newX = (swcMouse::instance().x - diffPos) - thumb.getX();

        if (newX < 0)
            scrollPrev(-newX);
        else if (newX > 0)
            scrollNext(+newX);
    };
}

void swcHScrollbar::impOnClick()
{
    if (swcMouse::instance().lastClickPoint.x < thumb.getPosMidX())
        scrollPrev(getStep());
    else if (swcMouse::instance().lastClickPoint.x > thumb.getPosMidX())
        scrollNext(getStep());
}

void swcHScrollbar::setX(int x)
{
    swcObject::setX(x);

    reComputeComponentProperties();
}

void swcHScrollbar::setY(int y)
{
    swcObject::setY(y);

    reComputeComponentProperties();
}

void swcHScrollbar::setPosition(int x, int y)
{
    swcObject::setPosition(x, y);

    reComputeComponentProperties();
}

void swcHScrollbar::setWidth(uint32_t width)
{
    swcObject::setWidth(width);

    reComputeComponentProperties();
}

void swcHScrollbar::setHeight(uint32_t height)
{
    swcObject::setHeight(height);

    btnA.setHeight(height - button_padding);
    btnB.setHeight(height - button_padding);
    thumb.setHeight(height - button_padding);

    reComputeComponentProperties();
}

void swcHScrollbar::setSize(uint32_t width, uint32_t height)
{
    swcObject::setSize(width, height);

    btnA.setSize(height - button_padding, height - button_padding);
    btnB.setSize(height - button_padding, height - button_padding);
    thumb.setHeight(height - button_padding);

    reComputeComponentProperties();
}

void swcHScrollbar::setSize(uint32_t size)
{
    setSize(size, DEFAULT_SIZE);
}

void swcHScrollbar::scrollPrev(int newX)
{
    int x1 = thumb.getX();
    int x2 = thumb.getX() - newX;

    while (canMovePrev())
    {
        thumb.setX(x1--);
        if (x1 == x2 || !canMovePrev())
        {
            funcRun(onScrollPrev);
            break;
        }

        if (isSet())
        {
            for (auto &i : *objects)
            {
                int newX = i->getX() + 1;

                i->setX(newX);
            }
        }
    }

    saveThumbLocation();
}

void swcHScrollbar::scrollNext(int newX)
{
    int x1 = thumb.getX();
    int x2 = thumb.getX() + newX;

    while (canMoveNext())
    {
        thumb.setX(x1++);
        if (x1 == x2 || !canMoveNext())
        {
            funcRun(onScrollNext);
            break;
        }

        if (isSet())
        {
            for (auto &i : *objects)
            {
                int newX = i->getX() - 1;

                i->setX(newX);
            }
        }
    }

    saveThumbLocation();
}

bool swcHScrollbar::canMovePrev() const
{
    return thumb.getPosLeft() > btnA.getPosRight() + 1;
}

bool swcHScrollbar::canMoveNext() const
{
    return thumb.getPosRight() < btnB.getPosLeft() - 1;
}

void swcHScrollbar::reComputeComponentProperties()
{
    computeXposition
    (
    ALIGN::LEFT | ALIGN::MIDDLE_Y,
    btnA, getByRef(), +(button_padding * 0.5), 0
    );

    computeXposition
    (
    ALIGN::RIGHT | ALIGN::MIDDLE_Y,
    btnB, getByRef(), -(button_padding * 0.5), 0
    );

    computeXposition
    (
    ALIGN::LEFT | ALIGN::MIDDLE_Y,
    thumb, getByRef(), btnA.getTotalWidth() + 1 + (button_padding * 0.5), 0
    );

    loadThumbLocation();
}


void swcHScrollbar::setThumbSize(uint32_t size)
{
    if (size < getThumbMaxSize())
        thumb.setWidth(size);
}

}
