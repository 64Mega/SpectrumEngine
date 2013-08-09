// Engine.h implementation

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include <string>
#include <list>

using namespace std;

#include "spectrum/object.h"
#include "spectrum/exception.h"
#include "spectrum/engine/assets.h"
#include "spectrum/engine/gameobject.h"
#include "spectrum/engine/room.h"
#include "spectrum/engine/engine.h"

#include <gl/gl.h>
#include <gl/glu.h>

Engine::Engine():Object()
{
    this->SetName("Engine");
    iwidth = 640;
    iheight = 480;

    ff_phase = 0;
    ff_alpha = 0.0F;

    // Build the version string
    string title = "RPG";
    window = new sf::RenderWindow(sf::VideoMode(640,480,32), title);

    // Game Font

    Assets::AddImage("./res/spr_gamefont_strip97.png","font game");
    fnt_sprite = new Sprite("font game",97);
    fnt_sprite->SetSpeed(0);
    c_font = fnt_sprite;

    if(window == NULL)
    {
        throw (Exception(this, "Error initializing display mode!"));
    }

    window->setFramerateLimit(30);

    // Initialize OpenGL
    //window->PreserveOpenGLStates(true);

    glClearDepth(1);
    glClearColor(0,0,0,0);

    glDepthMask(GL_TRUE);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0,0,640,480);

    glOrtho(0,320,240,0,0,100);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_COLOR_MATERIAL);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();

    // Set the global engine pointer
    Engine::g_engine = this;


    st_dmode = false;
}

void Engine::reinit()
{
    window->setFramerateLimit(30);

    // Initialize OpenGL
    //window->PreserveOpenGLStates(true);

    glClearDepth(1);
    glClearColor(0,0,0,0);

    glDepthMask(GL_TRUE);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0,0,iwidth,iheight);

    glOrtho(0,iwidth/2,iheight/2,0,0,100);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_COLOR_MATERIAL);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
}

Engine::Engine(int width, int height, bool fullscreen):Object()
{
    this->SetName("Engine");

    iwidth = width;
    iheight = height;

    ff_alpha = 0.0F;
    ff_phase = 0;

    // Build the version string
    string title = "Exile";
    if(fullscreen)window = new sf::RenderWindow(sf::VideoMode(iwidth,iheight,32), title, sf::Style::Fullscreen);
    else window = new sf::RenderWindow(sf::VideoMode(iwidth,iheight,32), title);

    // Game Font

    Assets::AddImage("./res/spr_gamefont_strip97.png","font game");
    fnt_sprite = new Sprite("font game",97);
    fnt_sprite->SetSpeed(0);
    c_font = fnt_sprite;

    if(window == NULL)
    {
        throw (Exception(this, "Error initializing display mode!"));
    }

    window->setFramerateLimit(30);

    // Initialize OpenGL
    //window->PreserveOpenGLStates(true);

    glClearDepth(1);
    glClearColor(0,0,0,0);

    glDepthMask(GL_TRUE);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0,0,iwidth,iheight);

    glOrtho(0,iwidth/2,iheight/2,0,0,100);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_COLOR_MATERIAL);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();

    // Set the global engine pointer
    Engine::g_engine = this;

    // Set the Room input pointer
    //Room::room_input = &this->window->GetInput();

    st_message = "";
    st_lifetime = 0;
    st_alpha = 0.0F;
    st_swapout = true;
    st_dmode = false;
}

void Engine::go2D()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0,0,iwidth,iheight);

    glOrtho(0,iwidth,iheight,0,0,100);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_COLOR_MATERIAL);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
}

int Engine::Fade()
{
    // Do fade operation
    if(ff_phase == 0)
    {
        if(ff_alpha < 1.0F)ff_alpha += 0.05F;
        else
        {
            ff_phase = 1;
        }
    }
    if(ff_phase == 1)
    {
        if(ff_alpha > 0.0F)ff_alpha -= 0.05F;
        else
        {
            ff_phase = 2;
            return ff_phase;
        }
    }
    if(ff_phase == 2)ff_phase = 0;
    return ff_phase;
}

void Engine::go2DX()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0,0,iwidth,iheight);

    glOrtho(0,iwidth/2,iheight/2,0,0,100);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_COLOR_MATERIAL);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
}

void Engine::go3D()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0,0,iwidth,iheight);

    gluPerspective(60.0F, 1.33F,0.1F,1000.0F);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_COLOR_MATERIAL);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
}

Engine::~Engine()
{
    Engine::g_engine = NULL;
    if(window)
    {
        if(window->isOpen())window->close();
        delete window;
        window = 0;
    }
}

// Normal Methods

bool Engine::IsRunning()
{
    return window->isOpen(); // Can assume a valid pointer.
}

void Engine::Update()
{
    sf::Event event;
    while(window->pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
        {
            window->close();
        }
    }
    // Will update here

    //window->Clear();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    // Now call room-update code

    if(Engine::g_room)
    {
        Engine::g_room->OnUpdate();
    }
}

void Engine::Render()
{
    // Render code goes here
    // -- Engine Render Code Start
    if(Engine::g_room)
    {
        Engine::g_room->OnDraw();
    }
    // Draw all messages in the message queue
    if(message_queue.size() > 0)
    {
        for(unsigned int i = 0; i < message_queue.size(); i++)
        {
            string ts = message_queue[i].message;
            int l = ts.length();
            for(int k = 0; k < l; k++)
            {
                //fnt_sprite->SetFrame(ts[k]-32);
                int tt = ts[k];
                int to = 0;
                if(tt == 'p' || tt == 'q' || tt == 'y' || tt == 'g') to = 2;
                int width = message_queue[i].fnt->GetWidth();
                message_queue[i].fnt->SetFrame(ts[k]-32);

                message_queue[i].fnt->Draw(message_queue[i].x+(width*k)+Room::view_x,message_queue[i].y + to + Room::view_y,message_queue[i].r,message_queue[i].g,message_queue[i].b);
            }
        }
        message_queue.clear();
        STMessageStep();
    }

    // Draw fade effect
    go2DX();
    glColor4f(0.0F,0.0F,0.0F,ff_alpha);
    glBindTexture(GL_TEXTURE_2D,0);
    glBegin(GL_QUADS);
        glVertex2f(0,0);
        glVertex2f(320,0);
        glVertex2f(320,240);
        glVertex2f(0,240);
    glEnd();

    // --
    glFlush();
    window->display();
}

vector<Engine::r_message> Engine::message_queue;

Sprite* Engine::c_font = NULL;
Sprite* Engine::fnt_sprite = NULL;

void Engine::Print(int x, int y, std::string txt, float r, float g, float b)
{
    message_queue.push_back(r_message(x,y,txt,r,g,b,Engine::c_font));
}

void Engine::SetFont(Sprite* font)
{
    Engine::c_font = font;
    Engine::c_font->SetSpeed(0);
}
void Engine::SetFontDefault()
{
    Engine::c_font = fnt_sprite;
}

sf::RenderWindow& Engine::GetRenderContext()
{
    return (*this->window);
}

Engine& Engine::GetEngine()
{
    if(Engine::g_engine != NULL)return (*Engine::g_engine);
    else throw Exception(new Object, "Global Engine pointer not assigned!");
}

void Engine::SetRoom(Room* room)
{
    if(room == NULL)
    {
        if(Engine::g_room)Engine::g_room->OnUnset();
        Engine::g_room = NULL;
    }
    else
    {
        if(Engine::g_room)Engine::g_room->OnUnset();
        Engine::g_room = room;
        Engine::g_room->OnSet();
    }
}

bool Engine::PlaceFree(int x, int y)
{
    if(Engine::g_room)
    {
        return Engine::g_room->check_place_free(x,y);
    }

    return false;
}

bool Engine::LineCheck(int x1, int y1, int x2, int y2)
{
    if(Engine::g_room)
    {
        return Engine::g_room->check_line(x1,y1,x2,y2);
    }

    return false;
}

void Engine::MoveDown(GameObject& self, int max)
{
    if(Engine::g_room == NULL)return;
    if(self.current_sprite == NULL)return;

    for(int i = 0; i < max; i++)
    {
        Rect BB = self.current_sprite->GetBBox();
        bool r = Engine::g_room->check_place_free(self.x,self.y+BB.y+i);
        //bool r = Engine::g_room->check_place_free(self.x,self.y+self.current_sprite->GetOffsetY() + self.current_sprite->GetBBox().y + i + 1);

        if(r)
        {
            self.y += i + 1;
            return;
        }
    }
}
void Engine::MoveUp(GameObject& self, int max)
{
    if(Engine::g_room == NULL)return;
    if(self.current_sprite == NULL)return;
    for(int i = 0; i < max; i++)
    {
        bool r = Engine::g_room->check_place_free(self.x,self.y+self.current_sprite->GetBBox().y-i);
        if(r)
        {
            self.y -= i;
            return;
        }
    }
}

void Engine::MoveLeft(GameObject& self, int max)
{
    if(Engine::g_room == NULL)return;
    if(self.current_sprite == NULL)return;
    for(int i = 0; i < max; i++)
    {
        bool r = Engine::g_room->check_place_free(self.x + self.current_sprite->GetBBox().x - i,self.y);
        if(r)
        {
            self.x -= i;
            return;
        }
    }
}
void Engine::MoveRight(GameObject& self, int max)
{
    if(Engine::g_room == NULL)return;
    if(self.current_sprite == NULL)return;
    for(int i = 0; i < max; i++)
    {
        bool r = Engine::g_room->check_place_free(self.x + self.current_sprite->GetBBox().w + i,self.y);
        if(r)
        {
            self.x += i;
            return;
        }
    }
}

void Engine::SetSTMessage(std::string amsg, int lifetime, bool swap, bool dscale_mode)
{
    if(!st_swapout)return;
    st_message = amsg;
    st_lifetime = lifetime;
    st_swapout = swap;
    st_alpha = 1.0F;
    st_dmode = dscale_mode;
}

void Engine::STMessageStep()
{
    // Update and render the message
    if(st_lifetime > 0)
    {
        st_lifetime--;

        int l = st_message.length();
        int ofx = (l/2)*8;
        Sprite* fnt_disp = new Sprite("font_game",96);
            for(int k = 0; k < l; k++)
            {
                int width = 8;
                fnt_disp->SetFrame(st_message[k]-32);


                if(st_dmode)fnt_disp->Draw((160-ofx)+(width*k)+Room::view_x,230 + Room::view_y);
                else fnt_disp->Draw((320-ofx)+(width*k)+Room::view_x,470 + Room::view_y);
            }
        delete fnt_disp;
    }
    else
    {
        st_alpha = 0.0F;
        st_swapout = true;
    }
}

Engine* Engine::g_engine = NULL;
Room* Engine::g_room = NULL;
Sprite* font_game = NULL;
