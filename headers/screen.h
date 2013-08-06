// Represents the concept of a "Screen".
// Examples of a 'screen' would be: Title screen, Intro screen, Options screen, Menu screen, etc.

#ifndef SCREEN_H
#define SCREEN_H

#include <string>
#include <list>

class Screen
{
protected:
    std::string screenName;
    bool done;

public:
    Screen();
    ~Screen();

    virtual void update()=0;

    bool isDone();
    void enterScreen();
};

extern std::list<Screen*> screen_list;

#endif // SCREEN_H
