// Rect implementation

#include <math.h>
#include <stdio.h>
#include <string>

using namespace std;

#include "spectrum/engine/rect.h"

Rect::Rect()
{
    x = y = w = h = 0;
}

Rect::Rect(int x, int y, int w, int h)
{
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
}

Rect::Rect(const Rect& C)
{
    this->x = C.x;
    this->y = C.y;
    this->w = C.w;
    this->h = C.h;
}

Rect::~Rect()
{

}

Rect Rect::operator=(const Rect& C)
{
    Rect R(this->x, this->y, this->w, this->h);
    return R;
}

bool Rect::ColPoint(int x, int y)
{
    return !(x < this->x || x > this->x+this->w || y < this->y || y > this->y+this->h);
}

bool Rect::ColRect(Rect& R)
{
    return !(this->x+this->w < R.x || this->x > R.x+R.w ||
             this->y+this->h < R.y || this->y > R.y+R.h);
}

bool Rect::ColLine(int x1, int y1, int x2, int y2)
{
    int hl = fabs(x2-x1);
    int vl = fabs(y2-y1);
    int length = (hl > vl) ? hl : vl;

    float deltax = (float)(x2-x1)/(float)length;
    float deltay = (float)(y2-y1)/(float)length;

    for(int i = 0; i < length; i++)
    {
        int ix = (int)(x1 += deltax);
        int iy = (int)(y1 += deltay);

        if(ColPoint(ix,iy))return true;
    }
    return false;
}
