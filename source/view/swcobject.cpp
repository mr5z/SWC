#include <assert.h>
#include <cmath>
#include <vector>
#include <algorithm>
#include <GL/GLEW/glew.h>

#include <boost/range/adaptor/reversed.hpp>

#include "../../../VectorX/vectorx.h"

#include "../../include/controller/swcinputhandler.h"
#include "../../include/controller/swcwindowhandler.h"
#include "../../include/controller/swcmouse.h"

#include "../../include/view/utils/swchelper.h"
#include "../../include/view/swcobject.h"

static int object_counter = 0;

namespace swc
{

bool swcObject::mouse_hover_lock = false;
bool swcObject::mouse_click_lock = false;
swcObject *swcObject::__object = nullptr;

swcObject::swcObject() :
     enable             (true)
    ,color_swap         (false)
    ,border_color_swap  (false)
    ,drag_axis          (D_NONE)
    ,active             (false)
    ,trigger_click      (false)
    ,trigger_hover      (false)
    ,captured           (false)
    ,click              (false)
    ,click_once         (false)
    ,hover              (false)
    ,hover_once         (false)
    ,release            (false)
    ,unclick            (false)
{
    //ctor
    DEBUG_PRINT(++object_counter << " swcObjects constructed!");
}

swcObject::swcObject(const swcObject& other) :
     enable             (other.enable)
    ,color_swap         (other.color_swap)
    ,border_color_swap  (other.border_color_swap)
    ,active             (other.active)
{

}

swcObject::swcObject(swcObject&& other)
{

}

swcObject::~swcObject()
{
    if (--object_counter != 0)
    {
        DEBUG_PRINT("swcObject id: " << object_counter << " destructed!");
    } else
    {
        DEBUG_PRINT("all swcObjects destructed!");
    }
}

void swcObject::impShowComponents()
{
    for (auto &i : components)
    {
        i->update();
        i->draw();
    }
}

void swcObject::update()
{
    if (!enable) return;

    impUpdate();

    for (auto &i : boost::adaptors::reverse(components))
    {
        if (!i->enable) continue;

        if (i->isHovering(mouse()))
        {

            if (__object != nullptr) __object->hover = false;

            i->hover = true;
            this->hover = false;

            __object = &(*i);

            break;

        } else i->hover = false;

    }

    if (swcInputHandler::isLeftButtonClick() && hover)
    {
        if (isHovering(mouse().lastClickPoint) /*&& !mouse_click_lock*/)
        {
            click = true;
            release = false;
            mouse_click_lock = true;
            setFocus(true);
        }
    }

    if (swcInputHandler::isLeftButtonRelease() && click)
    {
        click = false;
        release = true;
        mouse_click_lock = false;
    }

    //mouse hover actions
    if (isHovering())
    {
        if (!hover_once)
        {
            trigger_hover = false;

            if (color_swap)         std::swap(colors, alt_colors);
            if (border_color_swap)  std::swap(border_colors, alt_border_colors);

            impOnHoverOnce();
            funcRun(onHoverOnce);
//            DEBUG_PRINT("mouse hovered");
        }

        hover_once = true;

        impOnHover();
        funcRun(onHover);

    } else
    {
        if (hover_once) {

            if (color_swap)         std::swap(colors, alt_colors);
            if (border_color_swap)  std::swap(border_colors, alt_border_colors);

            impOnRestOnce();
            funcRun(onRestOnce);
//            DEBUG_PRINT("mouse unhovered");

        }

        hover_once = false;

        impOnRest();
        funcRun(onRest);
    }

    //mouse click/release actions
    if (isClick())
    {
        if (!captured)
        {
            click_once = true;
            active = true;

            diffPos(mouse().x - getX(),
                    mouse().y - getY());

            impOnClickOnce();
            funcRun(onClickOnce);
            DEBUG_PRINT("mouse clicked once!");

        } else click_once = false;

        captured = true;

        trigger_click = false;
        unclick = false;

        if (drag_axis != D_NONE)
        {
            if (drag_axis & D_X) setX(mouse().x - diffPos.x);
            if (drag_axis & D_Y) setY(mouse().y - diffPos.y);
        }

        impOnClick();
        funcRun(onClick);
//        DEBUG_PRINT("mouse click holding!");

    } else if (isRelease())
    {

        captured = false;

        impOnRelease();
        funcRun(onRelease);
        DEBUG_PRINT("mouse released!");

    } else if (isUnclick())
    {
        unclick = true;
        active = false;
        setFocus(false);

        impOnUnclick();
        funcRun(onUnclick);

        DEBUG_PRINT("mouse unclick");
    }

}

void swcObject::setAltColorOnHover(const swcCornerColors &color)
{
    set_alt_colors_hover = true;
}

void swcObject::setAltColorOnClick(const swcCornerColors &color)
{
    set_alt_colors_click = true;
}

void swcObject::setBorderAltColorOnHover(const swcCornerColors &color)
{
    set_border_alt_colors_hover = true;
}

void swcObject::setBorderAltColorOnClick(const swcCornerColors &color)
{
    set_border_alt_colors_click = true;
}

bool swcObject::isClick() const {
    return click || trigger_click;
}

bool swcObject::isClickOnce() const {
    return click_once || trigger_click;
}

bool swcObject::isRelease() const {
    return release && captured;
}

bool swcObject::isUnclick() const {
    return !isHovering(mouse().lastClickPoint) &&
           !unclick && release;
}

bool swcObject::isTicked() const {
    return isHovering(mouse().prevClickPoint);
}

bool swcObject::isHovering() const {
    return hover || trigger_hover;
}

void swcObject::addComponent(swcObject *component)
{
    components.push_back(component);
}

void swcObject::reverseComponentsOrder()
{
    std::reverse(components.begin(), components.end());
}

}
