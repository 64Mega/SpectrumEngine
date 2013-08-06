
#include "screens/gamemenu.h"
#include "screens/mainmenu.h"

#include <string>
#include <list>
#include <iostream>

#include <spectrum/spectrum.h>

extern Sprite* font_game;

using namespace std;

ScreenGameMenu::ScreenGameMenu()
{
    done = false;
    screenName = "Game Menu";
    cout << "[SCREEN] Entered In-Game Menu." << endl;

    menuSelect = 0;
    kbDown = false;
    kbUp = false;
    kbLeft = false;
    kbRight = false;

    kbEnter = true;
    kbSpace = false;
    kbEscape = true;
}

ScreenGameMenu::~ScreenGameMenu()
{
    // Nothing yet.
}

void ScreenGameMenu::update()
{
    Engine *engine = &Engine::GetEngine();
    engine->go2DX();
    engine->SetFont(font_game);
    Engine::Print(0,0,"PAUSED",1.0F,0.0F,0.0F);

    Engine::Print(10,20,"RESUME");
    Engine::Print(10,30,"LOAD GAME");
    Engine::Print(10,40,"SAVE GAME");
    Engine::Print(10,50,"QUIT");

    // Allow the player to move the cursor
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Joystick::getAxisPosition(0,sf::Joystick::Y) > 20 || sf::Joystick::getAxisPosition(0,sf::Joystick::PovX) < -20)
    {
        if(!kbDown)
        {
            menuSelect++;
            Assets::PlaySound(10);
            if(menuSelect > 3)menuSelect = 0;
            kbDown = true;
        }
    }
    else kbDown = false;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Joystick::getAxisPosition(0,sf::Joystick::Y) < -20 || sf::Joystick::getAxisPosition(0,sf::Joystick::PovX) > 20)
    {
        if(!kbUp)
        {
            menuSelect--;
            Assets::PlaySound(10);
            if(menuSelect < 0)menuSelect = 3;
            kbUp = true;
        }
    }
    else kbUp = false;

    // Draw the two selector sprites
    int ixof;
    switch(menuSelect)
    {
    case 0:
        ixof = 20;
        break;
    case 1:
        ixof = 30;
        break;
    case 2:
        ixof = 40;
        break;
    case 3:
        ixof = 50;
        break;
    };

    //selector->Draw(ixof,80 + menuSelect*20);
    //selector->Draw(320-ixof-28,80 + menuSelect*20);
    Engine::Print(0,ixof,">");

    engine->Render();

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return) || sf::Joystick::isButtonPressed(0,0))
    {
        if(!kbEnter)
        {
            switch(menuSelect)
            {
            case 0:
                // Resume the game
                done = true;
                engine->SetSTMessage("",0,true);
                break;
            case 1:
                // Load game
                engine->SetSTMessage("GAME LOADED",200,true);
                // -- Load game here
                done = true;
                break;
            case 2:
                // Save game
                engine->SetSTMessage("GAME SAVED",200,true,true);
                // -- Save game here
                break;
            case 3:
                // Return to main menu
                done = true;
                menuSelect = 0;
                screen_list.push_back(new ScreenMainMenu());
                break;
            };
        }
        kbEnter = true;
    }
    else kbEnter = false;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || sf::Joystick::isButtonPressed(0,1))
    {
        if(!kbEscape)
        {
            // Resume the game
            done = true;
            engine->SetSTMessage("",0,true);
        }
        kbEscape = true;
    }
    else kbEscape = false;
}
