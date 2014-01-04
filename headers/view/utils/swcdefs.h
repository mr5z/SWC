#ifndef SWCDEFS_H
#define SWCDEFS_H

#include <stdint-gcc.h>
#include <limits.h>

#include <vector>
#include <memory>
#include <functional>

namespace swc
{

typedef std::shared_ptr < class swcObject    > spObject;
typedef std::shared_ptr < class swcText      > spText;
typedef std::shared_ptr < class swcLabel     > spLabel;
typedef std::shared_ptr < class swcButton    > spButton;
typedef std::shared_ptr < class swcTextBox   > spTextBox;
typedef std::shared_ptr < class swcTextView  > spTextView;
typedef std::shared_ptr < class swcCheckBox  > spCheckBox;
typedef std::shared_ptr < class swcComboBox  > spComboBox;
typedef std::shared_ptr < class swcSlider    > spSlider;
typedef std::shared_ptr < class swcScrollbar > spScrollbar;
typedef std::shared_ptr < class swcDocument  > spDocument;
typedef std::shared_ptr < class swcWindow    > spWindow;

typedef std::vector< spObject > vspObject;

typedef std::function<void () > EventExec;
typedef std::function<void (const struct Event& event) > EventFunctor;

typedef uint8_t rgb[3];
typedef uint8_t rgba[4];

enum BORDER_SIDE : int
{
    B_TOP       = 1 << 0,
    B_RIGHT     = 1 << 1,
    B_BOTTOM    = 1 << 2,
    B_LEFT      = 1 << 3
};

enum CORNER_LOCATION : int
{
    C_NW        = 1 << 0,
    C_NE        = 1 << 1,
    C_SE        = 1 << 2,
    C_SW        = 1 << 3
};

enum DRAG_AXIS : int
{
    D_NONE      = 1 << 0,
    D_X         = 1 << 1,
    D_Y         = 1 << 2
};

constexpr int ALL_BORDER_SIDE()
{
    return B_TOP | B_RIGHT | B_BOTTOM | B_LEFT;
}

constexpr int ALL_CORNER_SIDE()
{
    return C_NW | C_NE | C_SE | C_SW;
}

}

#endif // SWCDEFS_H
