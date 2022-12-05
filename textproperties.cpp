#include "textproperties.h"

textProperties::textProperties()
{

}

textProperties getTextProperties(QGraphicsTextItem *text)
{
    textProperties p;
    p.X=text->pos().x();
    p.Y=text->pos().y();
    p.font=text->font();
    p.scale=text->scale();
    return p;
}
