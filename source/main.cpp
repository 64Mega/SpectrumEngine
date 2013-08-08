// DARKHOLD - Exile 2
// --
// (c) 2013, Rewired Games
// --

#include <mio.h>
#include <math.h>
#include <mxmath.h>
#include <iostream>
#include <fstream>

std::ofstream lout; // Log

#include "EmuStream.h"

#include "spectrum/spectrum.h"
#include "config.h"

// Include various screens
#include "screens/options.h"
#include "screens/mainmenu.h"
#include "screens/gamemenu.h"

#include <gl/gl.h>
#include <gl/glu.h>

bool stateMainMenu;
bool stateGameMenu;

#define SQR(a) (a*a)

extern Sprite* font_game;

int main(int argc, char** argv)
{
    GameLog::Initialize();
    lout.open("log.txt",ios::out);

    stateMainMenu = true;
    stateGameMenu = false;

    bool kbEscape = false;

    global_config.Load();

    Assets::SetMusicVolume(global_config.music_volume);
    Assets::SetSoundVolume(global_config.sound_volume);
    if(global_config.music == false)Assets::SetMusicVolume(0);
    if(global_config.sound == false)Assets::SetSoundVolume(0);

    Engine* engine;
    engine = new Engine(global_config.width,global_config.height,global_config.fullscreen);

    sf::Image ico_darkhold;
    ico_darkhold.loadFromFile("./icon.png");
    engine->GetRenderContext().setIcon(128,128,ico_darkhold.getPixelsPtr());

    engine->GetRenderContext().setMouseCursorVisible(false);
    engine->GetRenderContext().setFramerateLimit(60);

    Assets::LoadPack(global_config.pack_assets);
    Assets::LoadList(global_config.list_assets);

    font_game = new Sprite("font_game",96);

    bool gamestarted = false;

    Assets::svolume = global_config.sound_volume;
    Assets::mvolume = global_config.music_volume;
    Assets::music = global_config.music;
    Assets::sound = global_config.sound;

    cout <<"[SYSTEM] Game started." << endl;

    engine->ff_phase = 1;
    engine->ff_alpha = 1.0F;

    screen_list.push_back(new ScreenMainMenu());

    while(engine->IsRunning())
    {
        glClearColor(0.0F,0.0F,0.0F,1.0F);
        engine->Update();
        glDisable(GL_DEPTH_TEST);

        Assets::svolume = global_config.sound_volume;
        Assets::mvolume = global_config.music_volume;
        Assets::music = global_config.music;
        Assets::sound = global_config.sound;

        if(gamestarted == false)
        {
            // Draw the title screen

            if(engine->ff_phase != 0)engine->Fade();

            Sprite* font_display = new Sprite("font_display",97);

            engine->SetFont(font_game);
            Engine::Print(0,0,"DARKHOLD DEMO",1.0F,0.0F,0.0F);
            font_display->w = 7;
            engine->SetFont(font_game);
            Engine::Print(0,40,"THIS IS A WORKING AND FULLY PLAYABLE");
            Engine::Print(0,50,"'EPISODE' OF MY GAME.");
            Engine::Print(0,60,"IT CONTAINS FOUR LEVELS AND A BOSS");
            Engine::Print(0,70,"ARENA, SEVERAL RUNES, MULTIPLE ENEMIES AND");
            Engine::Print(0,80,"IN ALL LIKELIHOOD, A FAIR SHARE OF BUGS.");
            Engine::Print(0,90,"ENJOY THE GAME; I MAY KEEP WORKING ON");
            Engine::Print(0,100,"IT IF PEOPLE LIKE THE CONCEPT.");

            Engine::Print(0,120,"PRESS SPACE OR (X) TO CONTINUE...",0.3F,0.3F,0.9F);
            engine->Render();

            delete font_display;

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Joystick::isButtonPressed(0,0))
            {
                gamestarted = true;
            }

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                engine->GetRenderContext().close();
            }

            continue;
        }

        if(screen_list.empty() != true)
        {
            screen_list.back()->update();
            if(screen_list.back()->isDone())
            {
                screen_list.pop_back();
            }
            continue;
        }

        // Check if Escape is pressed

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || sf::Joystick::isButtonPressed(0,7))
        {
            if(!kbEscape)
            {
                screen_list.push_back(new ScreenGameMenu());
            }

            kbEscape = true;
        }
        else kbEscape = false;

        engine->go3D();
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER,0);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);

        // Render stuff in 3D here

        engine->go2D();
        glDisable(GL_LIGHTING);

        // Render 2D stuff here

        engine->Render();
    }

    delete engine;



    delete font_game;\
    Assets::Destroy();

    global_config.Write();

    lout.flush();
    lout.close();

    return 0;
}
