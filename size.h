#ifndef SIZE_H
#define SIZE_H


struct size
{
    size(float = 0,float = 0);
    float width,height;

    void constrainProportions(size &);
};

#endif // SIZE_H
