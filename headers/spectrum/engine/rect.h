// Integer Rectangle class with basic collision checking

#ifndef RECT_H
#define RECT_H

class Rect
{
    public:
        Rect();
        Rect(int x, int y, int w, int h);
        Rect(const Rect& C);
        ~Rect();

        void Set(int x, int y, int w, int h);

        int x, y, w, h;

        bool ColPoint(int x, int y);
        bool ColRect(Rect& R);
        bool ColLine(int x1, int y1, int x2, int y2); // May not implement yet until I check the slope equation

        Rect& operator=(const Rect& C);
};

#endif
