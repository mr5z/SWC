#ifndef SWCWIDGET_H
#define SWCWIDGET_H

#include <vector>
#include <functional>

#include "debugger.h"
#include "swcrectangle.h"


namespace swc
{

class swcToolTip;

using ResizeEvent = std::function<void (int width, int height)>;
using DragEvent = std::function<bool (int direction)>;
using Delegate = std::function<void()>;

class swcWidget :
    public swcRectangle
{
    friend class swcApplication;
public:
    ///ctor, copy-ctor, move-ctor, dtor
    swcWidget();
    swcWidget(const swcWidget &other);
    swcWidget(swcWidget &&other) noexcept;
    virtual ~swcWidget();

    ///guess what's this
    void
    show();

    ///guess what's this
    void
    hide();

    ///if `hasParent()` returns to true, `this` will moved to top of `parent`'s stack
    bool
    bringToFront();

    bool
    bringToBack();

    void
    attachToolTip(TPtr<swcToolTip> &tooltip);

    TPoint
    getAbsX() const;

    TPoint
    getAbsY() const;

    TPoint
    getAbsPosMidX() const;

    TPoint
    getAbsPosMidY() const;

    swcPoint<int>
    getClickPoint() const;

    void setWidth(TSize width) final;
    void setHeight(TSize height) final;
    void setSize(TSize width, TSize height) final;

    ///common events (self explanatory)
    void
    setColorOnHover        (const swcRGB color);
    void
    setColorOnClick        (const swcRGB color);
    void
    setBorderColorOnHover  (const swcRGB color, int flags = ALL_BORDER_SIDE);
    void
    setBorderColorOnClick  (const swcRGB color, int flags = ALL_BORDER_SIDE);

    ///setting this to 'true' will receive all user inputs
    void
    setFocus(bool focus);

    void
    setParent(TPtr<swcWidget> parent);

    void
    setAlignment(int alignment, const swcPoint<TPoint> offset_point);

    bool
    hasParent() const;

    bool
    hasToolTip() const;

    bool
    hasFocus() const;

    bool
    isMouseHover() const;

    bool
    isMouseLeave() const;

    bool
    isClick() const;

    ///simplified delegate events
    DragEvent onDragX;      //continuous execution
    DragEvent onDragY;      //continuous execution

    Delegate onHovering;    //continuous execution
    Delegate onLongClick;   //continuous execution
    Delegate onMouseOver;   //execute once
    Delegate onMouseLeave;  //execute once
    Delegate onClick;       //execute once
    Delegate onFocus;       //execute once

    int drag_axis;

protected:

    ///add component widget
    void
    addComponent(TPtr<swcWidget>component);

    ///move certain component to top of stack so it will be painted on top
    bool
    bringToFront(TPtr<swcWidget> component);

    bool
    bringToBack(TPtr<swcWidget> component);

    ///compute their orientation
    void
    computeLayout();

    virtual void
    handleMouseMove(int x, int y) { }
    virtual void
    handleMouseClick(int button, int state) { }
    virtual void
    handleMouseScroll(int dirX, int dirY) { }
    virtual void
    handleKeyboardKey(int key, int state) { }
    virtual void
    handleKeyboardChar(int key) { }

    virtual void
    handleMouseOver() { }
    virtual void
    handleMouseHovering() { }
    virtual void
    handleMouseLeave() { }

    virtual void
    handleSingleMouseClick() { }
    virtual void
    handleDoubleMouseClick() { }
    virtual void
    handleLongMouseClick() { }

    virtual void
    handleGainFocus() { }
    virtual void
    handleLoseFocus() { }

    virtual void
    handleResizeWidth(TSize width) { }
    virtual void
    handleResizeHeight(TSize height) { }
    virtual void
    handleResize(TSize width, TSize height) { }

    ///Required to prevent object-slicing
    virtual inline
    swcWidget *clone() const {
        return (new swcWidget(*this));
    }

    ///parent widget in w/c we base our alignment
    TPtr<swcWidget> parent;

    ///children a.k.a components
    std::vector<TPtr<swcWidget>> components;

    swcPoint<int> click_point;

private:

    ///non-virtual function version of 'update()'
    void
    notify();

    ///event fetcher
    ///this should be called only by the main controller (swcApplication)
    void
    performMouseMove(int x, int y);
    void
    performMouseClick(int button, int state);
    void
    performMouseScroll(int dirX, int dirY);
    void
    performKeyboardChar(int key);
    void
    performKeyboardKey(int key, int state);

    ///reserved for animations and threading
    virtual void
    update(float dt) { }

    ///paint all components above
    void
    paintComponents() final;

    ///default action on hover once
    void
    defaultActionHoverOnce();
    ///default action on single click without waiting for left button to release
    void
    defaultActionSingleClick();

    ///our dearly friend to help us compute component's alignment
    friend void
    hComputeLayout(swcWidget &component);

    ///I have no choice but to do this
    void
    setArrayRGB(bool border, swcRGB *colors);

    ///
    void
    showToolTip();

    ///backup colors
    swcRGB alt_click_colors;
    swcRGB alt_hover_colors;
    swcRGB alt_border_click_colors[4];
    swcRGB alt_border_hover_colors[4];

    ///color trackers
    swcRGB colors_on_click;
    swcRGB colors_on_hover;
    swcRGB colors_border_on_click[4];
    swcRGB colors_border_on_hover[4];

    ///
    int64_t tab_index;

    ///padding in parent rectangle
    swcPoint<TPoint> offset_point;

    ///guess what?
    int alignment;

    TPtr<swcToolTip> tooltip;

    ///event trackers
    bool trigger; //raises control over the widget
    bool mouse_hover;
    bool mouse_leave;
    bool click;
    bool show_tab_indicator;

    ///misc event trackers
    bool set_color_hover;
    bool set_color_click;
    bool set_border_color_hover;
    bool set_border_color_click;

    ///reference to other widgets
    ///main purpose: swap focus
    static TPtr<swcWidget> other_widget;

    ///
    static int64_t tab_index_counter;

public:

    bool enable;
    bool focus;
    bool accepts_tab;
    bool allow_tab_indicator;
};

}

#endif // SWCWIDGET_H
