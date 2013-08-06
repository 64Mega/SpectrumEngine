// In-game menu

#ifndef GAMEMENU_SCREEN_H
#define GAMEMENU_SCREEN_H

#include <screen.h>

class ScreenGameMenu : public Screen
{
protected:
    bool kbDown, kbLeft, kbRight, kbUp;
    bool kbEnter, kbEscape, kbSpace;
    int menuSelect;
public:
    ScreenGameMenu();
    virtual ~ScreenGameMenu();

    void update();
};

#endif // OPTIONS_SCREEN_H



