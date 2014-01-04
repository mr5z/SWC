#ifndef SWCTEXTVIEW_H
#define SWCTEXTVIEW_H

#include "swctextbox.h"

namespace swc
{

class swcTextView :
    public swcTextBox
{
public:
    swcTextView();
    virtual ~swcTextView();

    virtual inline void setFont(const spFont& font) {
        swcText::setFont(font);
    }
protected:

    virtual void impOnEnter();
private:

};

}

#endif // SWCTEXTVIEW_H
