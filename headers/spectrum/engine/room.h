// A Room class manages a list of GameObjects.

#ifndef ROOM_H
#define ROOM_H

#include "gameobject.h"
#include "mxlmap.h"

class Room : public Object
{
    public:
        Room();
        virtual ~Room();

        // These methods add and remove objects from the list
        void Add(GameObject* obj);
        void RemoveType(string name);
        void RemoveID(int id);

        void OnSet();
        void OnUnset();
        void OnUpdate();
        void OnDraw();

        static int GetRoomWidth(){ return Room::f_width; }
        static int GetRoomHeight(){ return Room::f_height; }

        bool check_place_free(int x, int y);
        bool check_line(int x1, int y1, int x2, int y2);

        //static sf::Input *room_input;
        static int view_x;
        static int view_y;
        static float view_scale;

        void SetMap(MXLMap* m);
        void SetSBG(std::string asname)
        {
            if(static_bg)delete static_bg;
            static_bg = new Sprite(asname,1);
        }
    protected:
        list<GameObject*> obj_list;
        typedef list<GameObject*> o_iter;
        MXLMap* map;
        Sprite* static_bg;

        static int f_width, f_height;
};

#endif
