#include <GL/glfw3.h>

#include <algorithm>

#include "XAligner/xaligner.h"
#include "swcgfx.h"
#include "swcmouse.h"
#include "swcwidget.h"
#include "swctooltip.h"

using namespace swc;

int ctr = 0;
int dtr = 0;

enum
{
     _A
    ,_B
    ,_C
    ,_D
};

namespace swc
{

void
    hComputeLayout(swcWidget &component)
///-----------
{
    static TPoint newX;
    static TPoint newY;

    X::AlignRect
    (
       component.alignment
     , newX, newY
     , component.getWidth()
     , component.getHeight()
     , component.parent->getWidth()
     , component.parent->getHeight()
    );

    component.setPosition
    (
       newX + component.offset_point.x
     , newY + component.offset_point.y
    );
}

}

TPtr<swcWidget> swcWidget::other_widget = nullptr;

int64_t swcWidget::tab_index_counter = -1;

swcWidget::
    swcWidget() :
///-----------
         drag_axis              ( D_NONE )
        ,tab_index              ( ++tab_index_counter )
        ,alignment              ( ALIGN::TOP | ALIGN::LEFT )
        ,parent                 ( nullptr )
        ,tooltip                ( nullptr )
        ,trigger                ( false )
        ,mouse_hover            ( false )
        ,mouse_leave            ( false )
        ,click                  ( false )
        ,show_tab_indicator     ( false )
        ,set_color_hover        ( false )
        ,set_color_click        ( false )
        ,set_border_color_hover ( false )
        ,set_border_color_click ( false )
        ,enable                 ( true )
        ,focus                  ( false )
        ,accepts_tab            ( true )
        ,allow_tab_indicator    ( true )
{
    //ctor
    dbgPrint("swcWidget constructed: " << ++ctr);
}

swcWidget::
    swcWidget(const swcWidget &other) :
///-----------
         drag_axis              ( other.drag_axis )
        ,parent                 ( other.parent )
        ,trigger                ( other.trigger )
        ,mouse_hover            ( other.mouse_hover )
        ,mouse_leave            ( other.mouse_leave )
        ,click                  ( other.click )
        ,show_tab_indicator     ( other.show_tab_indicator )
        ,set_color_hover        ( other.set_color_hover )
        ,set_color_click        ( other.set_color_click )
        ,set_border_color_hover ( other.set_border_color_hover )
        ,set_border_color_click ( other.set_border_color_click )
        ,allow_tab_indicator    ( other.allow_tab_indicator )
{
    //mtor
    dbgPrint("swcWidget copied: ");

    for ( auto &i : other.components )
    {
        addComponent( i->clone() );
    }
}

swcWidget::
    swcWidget(swcWidget &&other) noexcept :
///-----------
         drag_axis              ( other.drag_axis )
        ,parent                 ( other.parent )
        ,trigger                ( other.trigger )
        ,mouse_hover            ( other.mouse_hover )
        ,click                  ( other.click )
        ,show_tab_indicator     ( other.show_tab_indicator )
        ,set_color_hover        ( other.set_color_hover )
        ,set_color_click        ( other.set_color_click )
        ,set_border_color_hover ( other.set_border_color_hover )
        ,set_border_color_click ( other.set_border_color_click )
        ,allow_tab_indicator    ( other.allow_tab_indicator )
{
    //mtor
    dbgPrint("swcWidget moved: ");

    components = std::move( other.components );
    parent = other.parent;
    tooltip = other.tooltip;

    other.components.clear();
    other.parent = nullptr;
    other.tooltip = nullptr;
}

swcWidget::
    ~swcWidget()
///-----------
{
    //dtor

    if ( hasToolTip() )
    {
        delete tooltip;

        tooltip = nullptr;
    }
    #define every ( auto&
    #define in :
    #define perform )
    #define start {
    #define end }

    for every widget in components perform
        start
            delete widget;
        end

    #undef every
    #undef in
    #undef perform
    #undef start
    #undef end
    dbgPrint("swcWidget destructed: " << ++dtr);
}

void
swcWidget::
    paintComponents()
///-----------
{
    for ( auto &widget : components )
    {
        glPushMatrix();

        widget->draw();

        glPopMatrix();

//        clipRect(getPosLeft(), getPosTop(), getTotalWidth(), getTotalHeight());

    }

    if (show_tab_indicator && allow_tab_indicator)
    {
        drawStippledRect( *this, {0, 0, 0}, 2, 0xaaaa );
    }
}

void
swcWidget::
    notify()
///-----------
{

    ///do draggables

    if ( isClick() )
    {
        if ( drag_axis & D_X )
        {
            int new_x = mouse().curr_hover.x - click_point.x;

            if ( static_cast<int>(getX()) != new_x )
//            if ( getX() != new_x )
            {
                int dir;

                if ( new_x < getX() ) dir = DR_DISSIPATE;
                if ( new_x > getX() ) dir = DR_ACCUMULATE;

                while ( static_cast<int>(getX()) != new_x )
//                while ( getX() != new_x )
                {

                    if (onDragX != nullptr)
                    {
                        if (!onDragX(dir))
                            break;
                    }

                    setX(getX() + dir);

                }
            }
        }

        if ( drag_axis & D_Y )
        {
            int new_y = mouse().curr_hover.y - click_point.y;

            if ( static_cast<int>(getY()) != new_y )
//            if ( getY() != new_y )
            {
                int dir;

                if ( new_y < getY() ) dir = DR_DISSIPATE;
                if ( new_y > getY() ) dir = DR_ACCUMULATE;

                while ( static_cast<int>(getY()) != new_y )
//                while ( getY() != new_y )
                {

                    if (onDragY != nullptr)
                    {
                        if ( !onDragY( dir ) )
                            break;
                    }

                    setY( getY() + dir );

                }
            }
        }

        handleLongMouseClick();

        safeRun( onLongClick );
    }
}

void
swcWidget::
    performMouseMove(int x, int y)
///-----------
{
    if ( trigger )
    {
        handleMouseHovering();

        safeRun( onHovering );
    }

    if ( trigger && !mouse_hover )
    {
        mouse_hover = true;

        if ( hasToolTip() )
        {
            tooltip->start
            (
               mouse().curr_hover.x
             , mouse().curr_hover.y
            );
        }

        defaultActionHoverOnce();

        handleMouseOver();

        safeRun( onMouseOver );

    } else if (!trigger && mouse_hover)
    {
        mouse_hover = false;

        if ( hasToolTip() )
        {
            tooltip->stop();
        }

        if ( set_color_hover )
        {
            setArrayRGB( false, &alt_hover_colors );
        }

        if ( set_border_color_hover )
        {
            setArrayRGB( true, alt_border_hover_colors );
        }

        handleMouseLeave();

        safeRun( onMouseLeave );

    }

    handleMouseMove( x, y );
}

void
swcWidget::
    performMouseClick(int button, int state)
///-----------
{

    switch ( state )
    {
    case GLFW_PRESS:
        {
            switch ( button )
            {
            case GLFW_MOUSE_BUTTON_LEFT:
                {
                    if (isMouseHover())
                    {
                        if ( hasToolTip() )
                        {
                            tooltip->stop();
                        }

                        defaultActionSingleClick();
                    }
                } break;
            }
        } break;
    case GLFW_RELEASE:
        {

            switch ( button )
            {
            case GLFW_MOUSE_BUTTON_LEFT:
                {
                    if ( isClick() && set_color_click )
                    {
                        setArrayRGB( false, &alt_click_colors );
                    }

                    if ( isClick() && set_border_color_click )
                    {
                        setArrayRGB( true, alt_border_click_colors );
                    }

                    ///I put this on release because I imitate the others.
                    ///Safest place to put this?
                    if ( isClick() && isMouseHover() )
                    {
                        handleSingleMouseClick();

                        safeRun( onClick );
                    }

                    click = false;
                }
            }

        } break;
    }

    handleMouseClick( button, state );
}

void
swcWidget::
    performMouseScroll(int dirX, int dirY)
///-----------
{
    if ( !hasFocus() ) return;

    handleMouseScroll( dirX, dirY );
}

void
swcWidget::
    performKeyboardKey(int key, int state)
///-----------
{
    if (!hasFocus()) return;

    handleKeyboardKey( key, state );
}

void
swcWidget::
    performKeyboardChar(int key)
///-----------
{
    if (!hasFocus()) return;

    handleKeyboardChar( key );
}

TPoint
swcWidget::
    getAbsX() const
///-----------
{
    return hasParent() ?
        parent->getAbsX() + getX() : getX();
}

TPoint
swcWidget::
    getAbsY() const
///-----------
{
    return hasParent() ?
        parent->getAbsY() + getY() : getY();
}

TPoint
swcWidget::
    getAbsPosMidX() const
///-----------
{
    return hasParent() ?
        parent->getAbsX() + getPosMidX() : getPosMidX();
}

TPoint
swcWidget::
    getAbsPosMidY() const
///-----------
{
    return hasParent() ?
        parent->getAbsY() + getPosMidY() : getPosMidY();
}

swcPoint<int>
swcWidget::
    getClickPoint() const
///-----------
{
    return click_point;
}

void
swcWidget::
    show()
///-----------
{
    visible = true;
    enable = true;
}

void
swcWidget::
    hide()
///-----------
{
    visible = false;
    enable = false;
}

void
swcWidget::
    setWidth(TSize width)
///-----------
{
    if (width == getWidth()) return;

    swcRectangle::setWidth( width );

    handleResizeWidth( width );

    computeLayout();
}

void
swcWidget::
    setHeight(TSize height)
///-----------
{
    if (height == getHeight()) return;

    swcRectangle::setHeight( height );

    handleResizeHeight( height );

    computeLayout();
}

void
swcWidget::
    setSize(TSize width, TSize height)
///-----------
{
    if (width == getWidth() && height == getHeight()) return;

    swcRectangle::setSize( width, height );

    handleResize( width, height );

    computeLayout();
}

void
swcWidget::
    setFocus(bool focus)
///-----------
{
    if (hasFocus() && focus) return;

    this->focus = focus;

    ///only one widget at a time should received keyboard input!?
    if ( other_widget != nullptr &&
         other_widget != this )
         other_widget->setFocus( false );

    if ( focus )
    {
        if (accepts_tab)
        {
//            show_tab_indicator = true;
        }

        handleGainFocus();

        safeRun( onFocus );

        other_widget = this;
    } else
    {
        show_tab_indicator = false;

        handleLoseFocus();
    }
}

void
swcWidget::
    attachToolTip(TPtr<swcToolTip> &tooltip)
{
    this->tooltip = tooltip;

    //set it to invalid state!
    tooltip = nullptr;
}

void
swcWidget::
    setParent(swcWidget *parent)
///-----------
{
    this->parent = parent;
}

void
swcWidget::
    setAlignment(int alignment, const swcPoint<TPoint> offset_point)
///-----------
{
    this->alignment = alignment;
    this->offset_point = offset_point;
}

bool
swcWidget::
    hasFocus() const
///-----------
{
    return focus;
}

bool
swcWidget::
    hasParent() const
///-----------
{
    return parent != nullptr;
}

bool
swcWidget::
    hasToolTip() const
///-----------
{
    return tooltip != nullptr;
}

bool
swcWidget::
    isMouseHover() const
///-----------
{
    return mouse_hover;
}

bool
swcWidget::
    isMouseLeave() const
///-----------
{
    return mouse_leave;
}

bool
swcWidget::
    isClick() const
///-----------
{
    return click;
}

void
swcWidget::
    setColorOnHover(const swcRGB color)
///-----------
{
    set_color_hover = true;

    colors_on_hover = color;
}

void
swcWidget::
    setColorOnClick(const swcRGB color)
///-----------
{
    set_color_click = true;

    colors_on_click = color;
}

void
swcWidget::
    setBorderColorOnHover(const swcRGB color, int flags)
///-----------
{
    set_border_color_hover = true;

    if ( flags & B_TOP )      colors_border_on_hover[_A] = color;
    if ( flags & B_RIGHT )    colors_border_on_hover[_B] = color;
    if ( flags & B_BOTTOM )   colors_border_on_hover[_C] = color;
    if ( flags & B_LEFT )     colors_border_on_hover[_D] = color;
}

void
swcWidget::
    setBorderColorOnClick(const swcRGB color, int flags)
///-----------
{
    set_border_color_click = true;

    if ( flags & B_TOP )      colors_border_on_click[_A] = color;
    if ( flags & B_RIGHT )    colors_border_on_click[_B] = color;
    if ( flags & B_BOTTOM )   colors_border_on_click[_C] = color;
    if ( flags & B_LEFT )     colors_border_on_click[_D] = color;
}

void
swcWidget::
    setArrayRGB(bool border, swcRGB *color)
///-----------
{
    if ( !border )
    {
        setColor( color[_A] );
    } else
    {
        setBorderColor( color[_A], B_TOP );
        setBorderColor( color[_B], B_RIGHT );
        setBorderColor( color[_C], B_BOTTOM );
        setBorderColor( color[_D], B_LEFT );
    }
}

void
swcWidget::
    showToolTip()
{
    if ( hasToolTip() )
    {
        tooltip->display();
    }
}

bool
swcWidget::
    bringToFront()
///-----------
{
    if ( hasParent() )
        return parent->bringToFront( this );
    else return false;
}

bool
swcWidget::
    bringToBack()
///-----------
{
    if ( hasParent() )
        return parent->bringToBack( this );
    else return false;
}

void
swcWidget::
    defaultActionHoverOnce()
///-----------
{
    if ( set_color_hover )
    {
        alt_hover_colors = getColor();

        setArrayRGB( false, &colors_on_hover );
    }

    if ( set_border_color_hover )
    {
        alt_border_hover_colors[_A] = getRGB_border_top();
        alt_border_hover_colors[_B] = getRGB_border_right();
        alt_border_hover_colors[_C] = getRGB_border_bottom();
        alt_border_hover_colors[_D] = getRGB_border_left();

        setArrayRGB( true, colors_border_on_hover );
    }
}

void
swcWidget::
    defaultActionSingleClick()
///-----------
{
    click = true;

    setFocus( true );

    click_point.x = mouse().last_click.x - getX();
    click_point.y = mouse().last_click.y - getY();

    if ( set_color_click )
    {
        if ( set_color_hover )
        {
            alt_click_colors = alt_hover_colors;
        } else
        {
            alt_click_colors = getColor();
        }
        setArrayRGB(false, &colors_on_click);
    }

    if ( set_border_color_click )
    {
        if ( set_border_color_hover )
        {
            alt_border_click_colors[_A] = alt_border_hover_colors[_A];
            alt_border_click_colors[_B] = alt_border_hover_colors[_B];
            alt_border_click_colors[_C] = alt_border_hover_colors[_C];
            alt_border_click_colors[_D] = alt_border_hover_colors[_D];
        } else
        {
            alt_border_click_colors[_A] = getRGB_border_top();
            alt_border_click_colors[_B] = getRGB_border_right();
            alt_border_click_colors[_C] = getRGB_border_bottom();
            alt_border_click_colors[_D] = getRGB_border_left();
        }

        setArrayRGB( true, colors_border_on_click );
    }
}

void
swcWidget::
    addComponent(TPtr<swcWidget> component)
///-----------
{
    component->setParent( this );
    components.push_back( component );
}

bool
swcWidget::
    bringToFront(TPtr<swcWidget> widget)
///-----------
{
    if ( widget == nullptr )
        return false;

    if ( components.size() == 1 )
        return false;

    auto it = std::find( components.begin(), components.end(), widget );

    if (widget == *it && it != components.end())
    {
//        std::swap(widget, components.front());
        components.erase( it );
        components.push_back( widget );
        return true;
    }

    return false;
}

bool
swcWidget::
    bringToBack(TPtr<swcWidget> widget)
///-----------
{
    if ( widget == nullptr )
        return false;

    if ( components.size() == 1 )
        return false;

    auto it = std::find( components.begin(), components.end(), widget );

    if ( widget == *it && it != components.end() )
    {
        std::swap( widget, components.back() );
        return true;
    }

    return false;
}

void
swcWidget::
    computeLayout()
///-----------
{
    for ( auto &i : components )
    {
        hComputeLayout( *i );
    }
}
