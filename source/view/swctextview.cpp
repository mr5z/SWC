#include "../../../BMfont/Text2D.h"
#include "../../../Aligner/aligner.h"
#include "../../../ColorCollection/include/colorcollection.h"

#include "../../include/view/utils/swcdefs.h"
#include "../../include/view/utils/swchelper.h"
#include "../../include/view/utils/swcgfx.h"
#include "../../include/view/utils/swccolor.h"
#include "../../include/view/swctextview.h"

namespace swc
{

swcTextView::swcTextView()
{
    //ctor
    setSize(256, 256);

    text_align = ALIGN::LEFT | ALIGN::TOP;
}

swcTextView::~swcTextView()
{
    //dtor
}

void swcTextView::impOnEnter()
{
    text += '\n';
    moveCaretCol(0);
    moveCaretRow(1);
}

}
