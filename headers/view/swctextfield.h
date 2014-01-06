#ifndef SWCTEXTFIELD_H
#define SWCTEXTFIELD_H

#include <queue>

#include "swctext.h"
#include "swcobject.h"

namespace swc
{

class Caret
{
public:

    Caret();

    void moveUp();
    void moveDown();
    void moveLeft(swcText* txt);
    void moveRight(swcText* txt);

    void moveLine(uint64_t height, int step);

    void moveNextLine(int height);
    void moveToLastLine(const spFont& font, const std::string& line);
    void moveToEnd(const spFont& font, const std::string& line);

    swcColor color;
    short blink_time;
    bool visible;
    bool focus;

private:

    void blink();
    void drawCaret(uint32_t height, uint8_t opacity, int z);

    struct TextLineRect
    {
        uint64_t index;     //index of 1st character of the string in current line
        uint64_t right;     //width of text on current line
        uint64_t bottom;    //height from the top most, not the height of text
        uint64_t top;       //top position of text of the current line
        short left;         //left position of the text of the current line
    };

    std::queue <TextLineRect> textLineRect;

    swcPoint<uint64_t> pos;

    uint64_t current_index; //current index of the whole string the caret is attached


    friend class swcTextField;

};

class swcTextField :
    public swcObject,
    public swcText
{
public:
    swcTextField();

    void setFont(const spFont& font);

    Caret caret;

protected:

    virtual void impOnInputText(const Event& event);
    virtual void impOnInputKey(const Event& event);
    virtual void impOnHover();
    virtual void impOnRestOnce();

private:

    void impUpdate();
    void impPostPaint();
    void impOnClickOnce();
    void impOnUnclick();

    void actionOnDelete();
    void actionOnEnter();

};

}

#endif // SWCTEXTFIELD_H
