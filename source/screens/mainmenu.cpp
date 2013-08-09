
#include "screens/mainmenu.h"
#include "screens/options.h"

#include <string>
#include <list>
#include <iostream>

#include <spectrum/spectrum.h>
extern Sprite* font_game;

using namespace std;

ScreenMainMenu::ScreenMainMenu()
{
    done = false;
    screenName = "Main Menu";
    cout << "[SCREEN] Entered Main Menu." << endl;

    menuSelect = 0;
    kbDown = false;
    kbUp = false;
    kbLeft = false;
    kbRight = false;

    kbEnter = true;
    kbSpace = false;
    kbEscape = false;
}

ScreenMainMenu::~ScreenMainMenu()
{
    // Nothing yet.
}

void ScreenMainMenu::update()
{
    Engine* engine = &Engine::GetEngine();
    engine->SetFont(font_game);
    Engine::Print(0,10,"DARKHOLD DEMO",1.0F,0.0F,0.0F);
    Engine::Print(0,20,"PLACEHOLDER TITLE SCREEN",1.0F,0.0F,0.0F);

    Engine::Print(10,40,"NEW GAME");
    Engine::Print(10,50,"LOAD GAME");
    Engine::Print(10,60,"OPTIONS");

    Engine::Print(10,80,"QUIT");

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
        ixof = 40;
        break;
    case 1:
        ixof = 50;
        break;
    case 2:
        ixof = 60;
        break;
    case 3:
        ixof = 80;
        break;
    case 4:
        ixof = 104;
        break;
    default:
        ixof = 0;
        break;
    };

    Engine::Print(0,ixof,">");

    engine->Render();

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return) || sf::Joystick::isButtonPressed(0,0))
    {
        if(!kbEnter)
        {
            switch(menuSelect)
            {
            case 0:
                // Begin play
                engine->SetSTMessage("",0,true);
                done = true;
                break;
            case 1:
                engine->SetSTMessage("GAME LOADED",200,true);
                // -- Load game here
                done = true;
                break;
            case 2:
                // Options menu. Now fully implemented. For now.
                menuSelect = 0;
                screen_list.push_back(new ScreenOptions());
                break;
            case 99:
                break;
            case 3:
                engine->GetRenderContext().close();
                break;
            };
            kbEnter = true;
        }
    }
    else kbEnter = false;
}
