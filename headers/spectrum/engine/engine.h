/*
 *  The 'Engine' header. Note that calling it an engine is overrated.
 *
 */

 #ifndef ENGINE_H
 #define ENGINE_H

 #include "gameobject.h"

// This is a very simple construct. All it does is handle an instance of the RenderWindow object
// and will eventually handle the various 'Room' frames (States, actually), along with the basic
// management of the Update cycles.

class Room;

// Basic game font
extern Sprite* font_game;

class Engine : public Object
{
    public:
        // Status messages, the ones displayed at the top-center of the screen

        std::string st_message;
        float st_alpha;
        int st_lifetime;
        bool st_swapout;
        bool st_dmode;

        float ff_alpha;
        int ff_phase;

        int Fade();

        void SetSTMessage(std::string amsg, int lifetime, bool swap = true, bool dscale_mode = false);
        void STMessageStep();

        // -- end status message variables

        Engine();
        Engine(int width, int height, bool fullscreen);
        ~Engine();

        bool IsRunning();

        void Update();
        void Render();

        void go3D();
        void go2D();
        void go2DX();

        sf::RenderWindow& GetRenderContext();

        void SetRoom(Room* room);

        static Room& GetRoom()
        {
            if(g_room)return (*g_room);
        }

        static Engine& GetEngine(); // Returns a reference to the global engine

        static bool PlaceFree(int x, int y);
        static bool LineCheck(int x1, int y1, int x2, int y2);
        static void MoveDown(GameObject& self, int max);
        static void MoveLeft(GameObject& self, int max);
        static void MoveRight(GameObject& self, int max);
        static void MoveUp(GameObject& self, int max);

        static void Print(int x, int y, std::string txt, float r = 1.0, float g = 1.0, float b = 1.0);
        static void SetFont(Sprite* font);
        static void SetFontDefault();

        void reinit();
    private:
        sf::RenderWindow* window;
        static Engine* g_engine;
        static Room* g_room;
        static Sprite *fnt_sprite; // Font for the engine to use
        static Sprite *c_font;

        int iwidth, iheight;

        // Struct used for print queue
        typedef struct tr_message
        {
            tr_message()
            {
                // Done like this so that all text is drawn over all scenery (After Room draw code is called)
                x = y = 0;
                r = g = b = 0.0F;
                message = "";
                fnt = NULL;
            }

            tr_message(int ax, int ay, std::string amessage, float ar = 1.0F, float ag = 1.0F, float ab = 1.0F, Sprite* fo = NULL)
            {
                x = ax;
                y = ay;
                message = amessage;
                r = ar;
                g = ag;
                b = ab;
                fnt = fo;
            }

            int x, y;
            float r, g, b;
            std::string message;
            Sprite* fnt;
        }r_message;

        // Print queue
        static vector<r_message> message_queue;
};

 #endif
