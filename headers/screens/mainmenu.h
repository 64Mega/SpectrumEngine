// The Main Menu

#ifndef MAINMENU_SCREEN_H
#define MAINMENU_SCREEN_H

#include <screen.h>

class ScreenMainMenu : public Screen
{
protected:
    bool kbDown, kbLeft, kbRight, kbUp;
    bool kbEnter, kbEscape, kbSpace;
    int menuSelect;
public:
    ScreenMainMenu();
    virtual ~ScreenMainMenu();

    void update();
};

#endif // OPTIONS_SCREEN_H
