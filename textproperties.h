#ifndef TEXTPROPERTIES_H
#define TEXTPROPERTIES_H

#include<QGraphicsTextItem>
#include<QFont>

struct textProperties
{
    float scale,X,Y,offset;
    QFont font;
    QColor color;
public:
    textProperties();
};

textProperties getTextProperties(QGraphicsTextItem*);

#endif // TEXTPROPERTIES_H
