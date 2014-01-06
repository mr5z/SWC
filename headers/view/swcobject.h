#ifndef SWCOBJECT_H
#define SWCOBJECT_H

#include <array>
#include <vector>

#include "../../include/controller/swcevent.h"
#include "swcrectangle.h"

namespace swc
{

class swcObject :
    public swcRectangle,
    public swcEvent
{
public:

    swcObject();
    swcObject(const swcObject& other);
    swcObject(swcObject&& other);

    virtual ~swcObject();

    void update();

    void setAltColorOnHover         (const swcCornerColors &color);
    void setAltColorOnClick         (const swcCornerColors &color);
    void setBorderAltColorOnHover   (const swcCornerColors &color);
    void setBorderAltColorOnClick   (const swcCornerColors &color);

    virtual inline const swcObject &getByRef() const {
        return *this;
    }

    inline void show() {
        visible = true;
        enable = true;
    }

    inline void hide() {
        visible = false;
        enable = false;
    }

    bool isClick()      const;
    bool isClickOnce()  const;
    bool isRelease()    const;
    bool isUnclick()    const;
    bool isTicked()     const;
    bool isHovering()   const;

    bool isHovering(int X, int Y, uint32_t W, uint32_t H) const;

    inline bool isHovering(int X, int Y) const {
        return
            X >= x && X <= getRight() &&
            Y >= y && Y <= getBottom();
    }

    template<typename T>
    inline bool isHovering(const swcPoint<T> &point) const {
        return
            point.x >= x && point.x <= getRight() &&
            point.y >= y && point.y <= getBottom();
    }

    inline bool isActive() const {
        return active;
    }

    inline void triggerClick() {
        trigger_click = true;
    }

    inline void triggerHover() {
        trigger_hover = true;
    }

    bool color_swap;
    bool border_color_swap;
    int drag_axis;

protected:

    virtual inline void impPrePaint    () { /*(*/ }
    virtual inline void impPostPaint   () { /*!*/ }
    virtual inline void impSpecialPaint() { /*)*/ }

    virtual inline void impUpdate     () { /*F*/ }
    virtual inline void impOnClick     () { /*U*/ }
    virtual inline void impOnClickOnce () { /*C*/ }
    virtual inline void impOnRelease   () { /*K*/ }
    virtual inline void impOnUnclick   () { /* */ }
    virtual inline void impOnHover     () { /*Y*/ }
    virtual inline void impOnHoverOnce () { /*O*/ }
    virtual inline void impOnRest      () { /*U*/ }
    virtual inline void impOnRestOnce  () { /*!*/ }

    virtual inline void computeComponentProperties() { }
    void addComponent(swcObject *component);
    void reverseComponentsOrder();

    std::vector<swcObject*> components;

private:

    void impShowComponents();

    bool active;
    bool trigger_click;
    bool trigger_hover;
    bool captured;
    bool click;
    bool click_once;
    bool hover;
    bool hover_once;
    bool release;
    bool unclick;

    swcPoint<int> diffPos;

    static bool mouse_hover_lock;
    static bool mouse_click_lock;

    static swcObject *__object;

friend class swcRTree;
friend class swcWindow;
friend class swcApplication;

};

}

#endif // SWCOBJECT_H
