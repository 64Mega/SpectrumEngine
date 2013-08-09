
#include "screens/options.h"

#include <iostream>
#include <string>

#include <spectrum/spectrum.h>
extern Sprite* font_game;

#include <config.h>

using namespace std;

ScreenOptions::ScreenOptions()
{
    screenName = "ScreenOptions";
    cout << "[SCREEN] Entered Options screen." << endl;
    done = false;

    menuSelect = 0;
    kbDown = false;
    kbUp = false;
    kbLeft = false;
    kbRight = false;

    kbEnter = true;
    kbSpace = false;
    kbEscape = false;
}

ScreenOptions::~ScreenOptions()
{

}

void ScreenOptions::update()
{
    // I hate these messy sorts of state-based affairs, but I'm brute-forcing the last remaining problems;
    // Elegance can wait.
    // -- One year later --
    // Elegance is still waiting...
    Engine *engine = &Engine::GetEngine();
    engine->SetFont(font_game);
    Engine::Print(0,0,"OPTIONS",1.0,0.2,0.2);
    engine->SetFont(font_game);
    string s_music_status = global_config.music ? "MUSIC: ON " : "MUSIC: OFF";
    string s_sound_status = global_config.sound ? "SOUND: ON " : "SOUND: OFF";
    string s_fullscreen_status = global_config.fullscreen ? "FULLSCREEN: YES" : "FULLSCREEN:  NO";
    string s_gamepad_status = global_config.gamepad ? "GAMEPAD:  ON" : "GAMEPAD: OFF";
    Engine::Print(10,20,s_music_status);
    Engine::Print(10,30,s_sound_status);
    Engine::Print(10,40,s_fullscreen_status);
    char smv[4] = {' ',' ',' ','\0'};
    char ssv[4] = {' ',' ',' ','\0'};
    sprintf(smv,"%d",global_config.music_volume);
    sprintf(ssv,"%d",global_config.sound_volume);
    string s_music_volume = "MUSIC VOLUME: " + string(smv);
    string s_sound_volume = "SOUND VOLUME: " + string(ssv);

    Engine::Print(10,60,s_music_volume);
    Engine::Print(10,70,s_sound_volume);


    char sbv[4] = {' ',' ',' ','\0'};
    sprintf(sbv,"%d",global_config.gamma);
    string s_gamma_level = "BRIGHTNESS: " + string(sbv);
    Engine::Print(10,90,s_gamma_level);

    Engine::Print(10,110,s_gamepad_status);

    Engine::Print(10,130,"BACK");


    // Allow the player to move the cursor

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Joystick::getAxisPosition(0,sf::Joystick::Y) > 20 || sf::Joystick::getAxisPosition(0,sf::Joystick::PovX) < -20)
    {
        if(!kbDown)
        {
            menuSelect++;
            Assets::PlaySound(10);
            if(menuSelect > 7)menuSelect = 0;
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
            if(menuSelect < 0)menuSelect = 7;
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
        ixof = 60;
        break;
    case 4:
        ixof = 70;
        break;
    case 5:
        ixof = 90;
        break;
    case 6:
        ixof = 110;
        break;
    case 7:
        ixof = 130;
        break;
    default:
        ixof = 0;
        break;
    };

    Engine::Print(0,ixof,">");

    engine->Render();

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || sf::Joystick::isButtonPressed(0,1))
    {
        if(!kbEscape)
        {
            done = true;
            menuSelect = 0;
        }
        kbEscape = true;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return) || sf::Joystick::isButtonPressed(0,0))
    {
        if(!kbEnter)
        {
            if(menuSelect == 0)global_config.music = !global_config.music;
            if(menuSelect == 1)global_config.sound = !global_config.sound;
            if(menuSelect == 2)
            {

                global_config.fullscreen = !global_config.fullscreen;
                global_config.fullscreen ? engine->SetSTMessage("RESTART GAME TO ENABLE FULLSCREEN",200,true,true) : engine->SetSTMessage("RESTART GAME TO DISABLE FULLSCREEN",200,true,true);


                // Recreate window with fullscreen mode enabled
                //int iw = Engine::GetEngine().GetRenderContext().getSize().x;
                //int ih = Engine::GetEngine().GetRenderContext().getSize().y;
                //if(global_config.fullscreen)Engine::GetEngine().GetRenderContext().create(sf::VideoMode(iw,ih),"Darkhold",sf::Style::Fullscreen);
                //else Engine::GetEngine().GetRenderContext().create(sf::VideoMode(iw,ih),"Darkhold");

                Engine::GetEngine().reinit();
            }
            if(menuSelect == 5)
            {
                // Brightness level set
            }
            if(menuSelect == 6)
            {
                global_config.gamepad = !global_config.gamepad;
            }
            if(menuSelect == 7)
            {
                done = true;
                menuSelect = 0;
            }
            Assets::svolume = global_config.sound_volume;
            Assets::mvolume = global_config.music_volume;
            Assets::music = global_config.music;
            Assets::sound = global_config.sound;
            Assets::PlaySound(11);
        }
        kbEnter = true;
    }
    else kbEnter = false;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Joystick::getAxisPosition(0,sf::Joystick::X) < -20 || sf::Joystick::getAxisPosition(0,sf::Joystick::PovY) < -20)
    {
        if(menuSelect == 0)
        {
            if(!kbLeft)
            {
                global_config.music = false;
            }
            kbLeft = true;
        }

        if(menuSelect == 1)
        {
            if(!kbLeft)
            {
                global_config.sound = false;
            }
            kbLeft = true;
        }

        if(menuSelect == 2)
        {
            if(!kbLeft)
            {
                global_config.fullscreen = false;
                global_config.fullscreen ? engine->SetSTMessage("RESTART GAME TO ENABLE FULLSCREEN",200,true,true) : engine->SetSTMessage("RESTART GAME TO DISABLE FULLSCREEN",200,true,true);
                // Recreate window with fullscreen mode enabled
                //int iw = Engine::GetEngine().GetRenderContext().getSize().x;
                //int ih = Engine::GetEngine().GetRenderContext().getSize().y;
                //if(global_config.fullscreen)Engine::GetEngine().GetRenderContext().create(sf::VideoMode(iw,ih),"Darkhold",sf::Style::Fullscreen);
                //else Engine::GetEngine().GetRenderContext().create(sf::VideoMode(iw,ih),"Darkhold");
            }
            kbLeft = true;
        }

        if(menuSelect == 3)
        {
            if(!kbLeft)global_config.music_volume -= 1;
            if(global_config.music_volume < 0)global_config.music_volume = 0;
            kbLeft = !kbLeft;
        }

        if(menuSelect == 4)
        {
            if(!kbLeft)global_config.sound_volume -= 1;
            if(global_config.sound_volume < 0)global_config.sound_volume = 0;
            kbLeft = !kbLeft;
        }
        if(menuSelect == 5)
        {
            if(!kbLeft)global_config.gamma -= 1;
            if(global_config.gamma < 0)global_config.gamma = 0;
            kbLeft = !kbLeft;
        }
        if(menuSelect == 6)
        {
            if(!kbLeft)
            {
                global_config.gamepad = false;
            }
            kbLeft = true;
        }
    }
    else kbLeft = false;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Joystick::getAxisPosition(0,sf::Joystick::X) > 20 || sf::Joystick::getAxisPosition(0,sf::Joystick::PovY) > 20)
    {
        if(menuSelect == 0)
        {
            if(!kbRight)
            {
                global_config.music = true;
            }
            kbRight = true;
        }

        if(menuSelect == 1)
        {
            if(!kbRight)
            {
                global_config.sound = true;
            }
            kbRight = true;
        }

        if(menuSelect == 2)
        {
            if(!kbRight)
            {
                global_config.fullscreen = true;
                global_config.fullscreen ? engine->SetSTMessage("RESTART GAME TO ENABLE FULLSCREEN",200,true,true) : engine->SetSTMessage("RESTART GAME TO DISABLE FULLSCREEN",200,true,true);
                // Recreate window with fullscreen mode enabled
                //int iw = Engine::GetEngine().GetRenderContext().getSize().x;
                //int ih = Engine::GetEngine().GetRenderContext().getSize().y;
                //if(global_config.fullscreen)Engine::GetEngine().GetRenderContext().create(sf::VideoMode(iw,ih),"Darkhold",sf::Style::Fullscreen);
                //else Engine::GetEngine().GetRenderContext().create(sf::VideoMode(iw,ih),"Darkhold");
            }
            kbRight = true;
        }

        if(menuSelect == 3)
        {
            if(!kbRight)global_config.music_volume += 1;
            if(global_config.music_volume > 255)global_config.music_volume = 255;
            kbRight = !kbRight;
        }

        if(menuSelect == 4)
        {
            if(!kbRight)global_config.sound_volume += 1;
            if(global_config.sound_volume > 255)global_config.sound_volume = 255;
            kbRight = !kbRight;
        }

        if(menuSelect == 5)
        {
            if(!kbRight)global_config.gamma += 1;
            if(global_config.gamma > 100)global_config.gamma = 100;
            kbRight = !kbRight;
        }
        if(menuSelect == 6)
        {
            if(!kbRight)
            {
                global_config.gamepad = true;
            }
            kbRight = true;
        }
    }
    else kbRight = false;
}
