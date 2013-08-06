// Not much to implement

#include <screen.h>

Screen::Screen()
{
    screenName = "Screen";
    done = false;
}

Screen::~Screen(){}

bool Screen::isDone(){ return done; }

void Screen::enterScreen() { done = false; }

std::list<Screen*> screen_list;
