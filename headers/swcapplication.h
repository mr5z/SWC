#ifndef SWCAPPLICATION_H
#define SWCAPPLICATION_H

#include <vector>
#include <memory>

#include "swcinputhandler.h"
#include "swcwindowhandler.h"
#include "swcwidget.h"

namespace swc
{

class swcApplication :
     public swcInputHandler
    ,public swcWindowHandler
{
    friend class swcInputHandler;
public:
    swcApplication(int argc, char *argv[]);
    virtual ~swcApplication();

    int
    run();

    void
    exit();

    void validate(bool event_occured);

    static swcApplication *getCurrentApp();

    struct Controls
    {
        using vWidgets = std::vector<swcWidget*>;
        Controls() :
             widget_tab_counter(-1)
        {

        }

        ~Controls();

        void
        add(swcWidget *widgets);

        void
        clearWidgets();

        inline vWidgets
        &getWidgets() {
            return widgets;
        }

        swcWidget *findTabbedWidget()
        {
            selectNextWidget();

            for (auto &widget : widgets)
            {
                tabbed_widget = recurseTabWidget( widget );

                if ( tabbed_widget != nullptr )
                {
                    return tabbed_widget;
                }
            }

            return nullptr;
        }

        void selectNextWidget()
        {
//            widget_tab_counter = (widget_tab_counter + 1) % (swcWidget::tab_index_counter + 1);

            widget_tab_counter = (widget_tab_counter < swcWidget::tab_index_counter) ? widget_tab_counter + 1 : 0;

            tabbed_widget = nullptr;
        }

//    private:

        swcWidget *recurseTabWidget(swcWidget *widget)
        {
            for (auto &component : widget->components)
            {
                recurseTabWidget( component );
            }

            if ( tabbed_widget != nullptr )
                return tabbed_widget;

            if (widget->tab_index == widget_tab_counter && widget->enable )
            {
                if ( widget->accepts_tab )
                {
                    tabbed_widget = widget;

                    return widget;
                }
            }

            return nullptr;
        }

        int64_t widget_tab_counter;

        vWidgets widgets;

        swcWidget *tabbed_widget;
    };

    Controls controls;

    void registerCallbacks();

protected:

    virtual void mainKeyboardControls(int key, int state);

private:

    void
    controlMouseMove(int x, int y);

    void
    controlMouseClick(int button, int state);

    void
    controlMouseScroll(int dirX, int dirY);

    void
    controlKeyboardChar(uint32_t key_char);

    void
    controlKeyboardKey(int key_code, int state);

    void
    interact(float dt);

    void
    display();

    friend void
    setCurrentApp(swcApplication *app);

    ///logic functions
    static void recurseMouseMove   (swcWidget *widget, int x, int y);
    static void recurseMouseClick  (swcWidget *widget, int button, int state);
    static void recurseMouseScroll (swcWidget *widget, int dirX, int dirY);
    static void recurseKeyboardChar(swcWidget *widget, uint32_t key_char);
    static void recurseKeyboardKey (swcWidget *widget, int key_code, int state);
    static void recurseUpdate      (swcWidget *widget, float dt);

    bool event_occured;
    bool exit_app;
};

}

#endif // SWCAPPLICATION_H
