#ifndef SWCTEXTBOX_H
#define SWCTEXTBOX_H

#include "utils/swccolor.h"
#include "swcobject.h"
#include "swctext.h"

namespace swc
{

class swcTextBox :
    public swcObject,
    public swcText
{
public:
    swcTextBox();

    virtual void setFont(const spFont& font);

    void setCaretColor(const rgb color);

    void setSelection(int start, uint64_t end);

    int blink_delay;
    std::string place_holder;

protected:

    void moveCaretRow(int step);
    void moveCaretCol(int step);

    inline void setCaretPosX(int x) {
        caretPos.x = x;
    }

    inline void setCaretPosY(int y) {
        caretPos.y = y;
    }

    virtual void impOnEnter() { }

private:

    void impPostPaint();
    void impUpdate();

    void impOnClickOnce();
    void impOnUnclick();

    void impOnInputText(const Event& event);
    void impOnInputKey(const Event& event);

    uint64_t caret_index_row;
    swcPoint<int> caretPos;
    bool show_caret;
    rgb caret_color;
};

}

#endif // SWCTEXTBOX_H
