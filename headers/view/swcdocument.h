#ifndef SWCDOCUMENT_H
#define SWCDOCUMENT_H

#include "../controller/swcrtree.h"
#include "utils/swcdefs.h"
#include "swcscrollbar.h"

namespace swc
{

class swcDocument :
    public swcObject
{
public:
    swcDocument();
    swcDocument(const swcDocument &other);
    swcDocument(swcDocument&& other) noexcept;

    virtual ~swcDocument();

    swcDocument& operator = (const swcDocument &other);
    swcDocument& operator = (swcDocument&& other);

    virtual void setX(int x);
    virtual void setY(int y);
    virtual void setPosition(int x, int y);

    virtual void setWidth(uint32_t width);
    virtual void setHeight(uint32_t height);
    virtual void setSize(uint32_t width, uint32_t height);

    void addContent(const spObject &content);

    vspObject getContents() const {
        return contents;
    }

protected:

    vspObject contents;

    spScrollbar vScrollbar;
    spScrollbar hScrollbar;

    int contentWidth;
    int contentHeight;

    swcRTree rtree;

private:

    void impPostPaint();
    void impUpdate();

    void impOnInputText(const Event &event);
    void impOnInputKey(const Event &event);

    void reComputeComponentProperties();

    void updateContent();

};

}

#endif // SWCDOCUMENT_H
