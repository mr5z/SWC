#include <GL/GLEW/glew.h>

//#include <boost/range/adaptor/reversed.hpp>

#include "../../../ColorCollection/include/colorcollection.h"
#include "../../../Aligner/aligner.h"

#include "../../include/controller/swcwindowhandler.h"
#include "../../include/controller/swcrtree.h"

#include "../../include/view/utils/swchelper.h"
#include "../../include/view/utils/swccolor.h"
#include "../../include/view/swcdocument.h"

namespace swc
{

swcDocument::swcDocument() :
    vScrollbar(new swcVScrollbar),
    hScrollbar(new swcHScrollbar)
{
    //ctor
    using namespace cc;

    setSize(100, 200);
    setColor(ALL_CORNER_SIDE(), getColor(COLORS::WHITE));
    setBorderColor(ALL_BORDER_SIDE(), getColor(COLORS::BLACK));
    setBorderSize(ALL_BORDER_SIDE(), 1);

    vScrollbar->setParentObject(this->contents);
//    vScrollbar->hide();

    hScrollbar->setParentObject(this->contents);
//    hScrollbar->hide();

    addComponent(vScrollbar.get());
    addComponent(hScrollbar.get());

    rtree.setFamily(this, &contents);
}

swcDocument::swcDocument(const swcDocument &other) :
    swcObject(other),
    contents(other.contents),
    vScrollbar(other.vScrollbar),
    hScrollbar(other.hScrollbar),
    contentWidth(other.contentWidth),
    contentHeight(other.contentHeight),
    rtree(other.rtree)
{

}

swcDocument::swcDocument(swcDocument&& other) noexcept :
    swcObject(other),
    contents(std::move(other.getContents())),
    vScrollbar(std::move(other.vScrollbar)),
    hScrollbar(std::move(other.hScrollbar)),
    contentWidth(other.contentWidth),
    contentHeight(other.contentHeight),
    rtree(other.rtree)
{
    other.contents.clear();
    other.vScrollbar.reset();
    other.hScrollbar.reset();
}

swcDocument::~swcDocument()
{

}

swcDocument& swcDocument::operator = (const swcDocument &other)
{
    this->contents = other.getContents();
    return *this;
}

swcDocument& swcDocument::operator = (swcDocument&& other)
{
    contents = std::move(other.getContents());
    vScrollbar = std::move(other.vScrollbar);
    hScrollbar = std::move(other.hScrollbar);

    other.contents.clear();
    other.hScrollbar.reset();
    other.vScrollbar.reset();

    return *this;
}

void swcDocument::setX(int x)
{
    swcObject::setX(x);

    reComputeComponentProperties();
}

void swcDocument::setY(int y)
{
    swcObject::setY(y);

    reComputeComponentProperties();
}

void swcDocument::setPosition(int x, int y)
{
    swcObject::setPosition(x, y);

    reComputeComponentProperties();
}

void swcDocument::setWidth(uint32_t width)
{
    swcObject::setWidth(width);

    hScrollbar->setSize(width - 22);

    reComputeComponentProperties();
}

void swcDocument::setHeight(uint32_t height)
{
    swcObject::setHeight(height);

    vScrollbar->setSize(height - 22);

    reComputeComponentProperties();
}

void swcDocument::setSize(uint32_t width, uint32_t height)
{
    swcObject::setSize(width, height);

    vScrollbar->setSize(height - 22);

    hScrollbar->setSize(width - 22);

    reComputeComponentProperties();
}

void swcDocument::impPostPaint()
{
    int scissorX = getX();
    int scissorY = swcWindowHandler::getWinHeight() - getY() - getHeight();
    int scissorW = getWidth();
    int scissorH = getHeight();

    glPushAttrib(GL_SCISSOR_BIT);

    glScissor(scissorX, scissorY, scissorW, scissorH);

    for (auto &i : contents)
    {
        i->update();
        i->draw();
    }
    glPopAttrib();
}

void swcDocument::impUpdate()
{
    rtree.traverse();

    for (auto &i : contents)
    {
        if (i->getRight() > getRight())
        {
//            contentWidth = getRight() + (i->getRight());
//
//            int arrowWidth = 25;
//            float viewportWidth = getWidth();
//
//            float viewableRatio = viewportWidth / contentWidth;
//
//            float scrollBarArea = viewportWidth - arrowWidth * 2;
//
//            hScrollbar->setThumbSize(scrollBarArea * viewableRatio);
//            hScrollbar->show();
        }// else hScrollbar->hide();

        if (i->getBottom() > getBottom())
        {
//            contentHeight = getBottom() + i->getBottom();
//
//            int arrowHeight = 25;
//            float viewportHeight = getHeight();
//
//            float viewableRatio = viewportHeight / contentHeight;
//
//            float scrollBarArea = viewportHeight - arrowHeight * 2;
//
//            vScrollbar->setThumbSize(scrollBarArea * viewableRatio);
//            vScrollbar->show();
        } //else vScrollbar->hide();
    }
}

void swcDocument::impOnInputText(const Event &event)
{
    for (auto &i : contents)
    {
        if (i->hasFocus())
        {
            i->impOnInputText(event);
            if (i->onInputText != nullptr)
                i->onInputText(event);
        }
    }
}

void swcDocument::impOnInputKey(const Event &event)
{
    for (auto &i : contents)
    {
        if (i->hasFocus())
        {
            i->impOnInputKey(event);
            if (i->onInputKey != nullptr)
                i->onInputKey(event);
        }
    }
}

void swcDocument::addContent(const spObject &content)
{
    this->contents.push_back(content);

    updateContent();
}

void swcDocument::updateContent()
{
    auto &cnt = contents.back();

    cnt->setPosition(getX() + cnt->getX(),
                     getY() + cnt->getY());

    if (cnt->getRight() > getRight())
    {
        if (contentWidth < getRight() + (cnt->getRight()))
        {
            contentWidth = getRight() + (cnt->getRight());

            int arrowWidth = 25;
            float viewportWidth = getWidth();

            float viewableRatio = viewportWidth / contentWidth;

            float scrollBarArea = viewportWidth - arrowWidth * 2;

            hScrollbar->setThumbSize(scrollBarArea * viewableRatio);
            hScrollbar->show();
        }
    }

    if (cnt->getBottom() > getBottom())
    {
        if (contentHeight < (getBottom() + cnt->getBottom()))
        {
            contentHeight = getBottom() + cnt->getBottom();

            int arrowHeight = 25;
            float viewportHeight = getHeight();

            float viewableRatio = viewportHeight / contentHeight;

            float scrollBarArea = viewportHeight - arrowHeight * 2;

            vScrollbar->setThumbSize(scrollBarArea * viewableRatio);
            vScrollbar->show();
        }

    }

}

void swcDocument::reComputeComponentProperties()
{
    computeXposition
    (
     ALIGN::RIGHT | ALIGN::TOP,
     *vScrollbar.get(), getByRef(), -1, 1
    );

    computeXposition
    (
     ALIGN::LEFT | ALIGN::BOTTOM,
     *hScrollbar.get(), getByRef(), 1, -1
    );
}

}
