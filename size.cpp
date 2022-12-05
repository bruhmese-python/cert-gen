#include "size.h"

size::size(float w,float h): width(w),height(h)
{
}

void size::constrainProportions(size &other)
{
    auto ratio = width/height;

    //constrains width first
    width = other.width;
    height = width/ratio;
}
