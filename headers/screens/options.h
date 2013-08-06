// The Options screen

#ifndef OPTIONS_SCREEN_H
#define OPTIONS_SCREEN_H

#include <screen.h>

class ScreenOptions : public Screen
{
protected:
    bool kbDown, kbLeft, kbRight, kbUp;
    bool kbEnter, kbEscape, kbSpace;
    int menuSelect;
public:
    ScreenOptions();
    virtual ~ScreenOptions();

    void update();
};

#endif // OPTIONS_SCREEN_H
