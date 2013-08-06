// Engine implementation

#include <stdio.h>
#include <list>
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;

#include "spectrum/object.h"
#include "spectrum/exception.h"
#include "spectrum/engine/rect.h"
#include "spectrum/engine/gameobject.h"
#include "spectrum/engine/room.h"

Room::Room():()
{
    obj_list.clear();
    map = NULL;
    f_width = 0;
    f_height = 0;
    static_bg = 0;
}

static int Room::f_width = 0;
static int Room::f_height = 0;

//class obj_Player;

Room::~Room()
{
    for(std::list<GameObject*>::iterator i = obj_list.begin(); i != obj_list.end(); i++)
    {
        GameObject* dptr = (*i);
        i = obj_list.erase(i);
        delete dptr;
        dptr = 0;
    }
    obj_list.clear();

    if(static_bg)delete static_bg;
    static_bg = NULL;

    map = NULL;
}

void Room::Add(GameObject* obj)
{
    if(!obj)return;
    if(obj_list.empty())
    {
        obj_list.push_back(obj);
        cout <<"Added instance of '" << obj->GetName() << "' to Room." << endl;
        obj = NULL;
        return;
    }
    for(std::list<GameObject*>::iterator i = obj_list.begin(); i != obj_list.end(); i++)
    {
        if(obj->GetDepth() <= (*i)->GetDepth())
        {
            obj_list.insert(i,obj);
            cout <<"Added instance of '" << obj->GetName() << "' to Room." << endl;
            obj = NULL;
            return;
        }
    }
    obj_list.push_back(obj);
    cout <<"Added instance of '" << obj->GetName() << "' to Room." << endl;
    obj = NULL;
    return;
}

void Room::RemoveID(int id)
{
    for(std::list<GameObject*>::iterator i = obj_list.begin(); i != obj_list.end(); i++)
    {
        if((*i)->GetID() == id)
        {
            GameObject* dptr = (*i);
            i = obj_list.erase(i);
            delete dptr;
            dptr = NULL;
            return;
        }
    }
}

void Room::RemoveType(string name)
{
    for(std::list<GameObject*>::iterator i = obj_list.begin(); i != obj_list.end(); i++)
    {
        if((*i)->GetName() == name)
        {
            GameObject* dptr = (*i);
            i = obj_list.erase(i);
            delete dptr;
            dptr = NULL;
        }
    }
}

//sf::Input* Room::room_input = NULL;
int Room::view_x = 0;
int Room::view_y = 0;
float Room::view_scale = 1.0F;

void Room::OnUpdate()
{
    // Click check, before the rest of the frame is processed
    //if(room_input == NULL)return; // Refuse to process updates until input is connected
    int mx = sf::Mouse::getPosition(Engine::GetEngine().GetRenderContext()).x;
    int my = sf::Mouse::getPosition(Engine::GetEngine().GetRenderContext()).y;
    bool mb = sf::Mouse::isButtonPressed(sf::Mouse::Left);

    for(std::list<GameObject*>::iterator i = obj_list.begin(); i != obj_list.end(); i++)
    {
        if((*i)->dead)
        {
            GameObject* dptr = (*i);
            i = obj_list.erase(i);
            delete dptr;
            dptr = NULL;
            if(i == obj_list.end())return;
        }
        if(mb)
        {
            (*i)->OnClick(mx,my);
        }

        (*i)->OnUpdate();

        // Collision detection check
        for(std::list<GameObject*>::iterator j = obj_list.begin(); j != obj_list.end(); j++)
        {
            // Do square distance determination in order to determine whether or not to test
            // for a narrow phase collision

            // No collision if this is the same object
            if((*j) == (*i))continue;

            // No collision if either object is non-collidable
            if((*j)->can_collide == false || (*i)->can_collide == false)continue;

            // No collision if no sprite
            if((*j)->current_sprite == NULL || (*i)->current_sprite == NULL)continue;

            float dx = fabs((*j)->x - (*i)->x);
            float dy = fabs((*j)->y - (*i)->y);
            float dist = (dx*dx) + (dy*dy);
            float sw = (*j)->current_sprite->GetWidth() > (*i)->current_sprite->GetWidth() ? (*j)->current_sprite->GetWidth()
                                                                                           : (*i)->current_sprite->GetWidth();

            sw = sw*sw;
            if(dist <= sw)
            {
                // Narrow phase collision
                Rect B1, B2;
                B1 = (*i)->current_sprite->GetBBox();
                B2 = (*j)->current_sprite->GetBBox();

                bool col = B1.ColRect(B2);
                if(col)
                {
                    (*i)->OnHit(*(*j));
                    (*j)->OnHit(*(*i));
                }
            }
        }
    }
}

void Room::OnDraw()
{
    if(static_bg)static_bg->Draw(view_x,view_y);
    if(map)map->Draw();
    for(std::list<GameObject*>::iterator i = obj_list.begin(); i != obj_list.end(); i++)
    {
        (*i)->OnDraw();
    }
}

void Room::OnUnset()
{
    // Call all sub-object's OnRemove() function
    for(std::list<GameObject*>::iterator i = obj_list.begin(); i != obj_list.end(); i++)
    {
        (*i)->OnRemove();
    }
    // Remove all objects that were created with mapcodes (All of 'em, probably)
    for(std::list<GameObject*>::iterator i = obj_list.begin(); i != obj_list.end(); i++)
    {
        GameObject* dptr = (*i);
        i = obj_list.erase(i);
        delete dptr;
        dptr = 0;
    }
    obj_list.clear();
}

void Room::OnSet()
{
    cout <<"Setting room..." << endl;
    // Scan through each and every mapcode and create instances if possible
    if(this->map)
    {
        cout <<"Scanning map" << endl;
        for(int iy = 0; iy < map->GetHeight(); iy++)
        for(int ix = 0; ix < map->GetWidth(); ix++)
        {
            if(map->GetCode(ix,iy,0) > 0)
            {
                cout <<"Instance ID: " << map->GetCode(ix,iy,0) << endl;
                int ind = map->GetCode(ix,iy,0)-1;
                switch(ind)
                {
                    // Case based instantialization
                    default:
                    break;
                }
            }
        }
    }

    for(std::list<GameObject*>::iterator i = obj_list.begin(); i != obj_list.end(); i++)
    {
        (*i)->OnAdd();
    }
}

bool Room::check_place_free(int x, int y)
{
    bool r = false;
    // Do MXLtest first, return early if possible (Early out)
    if(this->map)
    {
        r = map->place_free(x,y,0);
        if(!r)return r;
    }
    for(std::list<GameObject*>::iterator i = obj_list.begin(); i != obj_list.end(); i++)
    {
        if(!(*i)->current_sprite)continue;
        r = (*i)->current_sprite->GetBBox().ColPoint(x,y);
        if(r == true)return !r;
    }
    return true;
}

bool Room::check_line(int x1, int y1, int x2, int y2)
{
    bool r = false;
    for(std::list<GameObject*>::iterator i = obj_list.begin(); i != obj_list.end(); i++)
    {
        if(!(*i)->current_sprite)continue;
        r = (*i)->current_sprite->GetBBox().ColLine(x1,y1,x2,y2);
        if(r == true)return r;
    }
    return false;
}

void Room::SetMap(MXLMap* m)
{
    this->map = m;
    if(m == NULL)return;
    this->f_width = m->GetWidth()*16;
    this->f_height = m->GetHeight()*16;
}

