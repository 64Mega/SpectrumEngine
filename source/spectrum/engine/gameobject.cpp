// GameObject implementation

#include <stdio.h>
#include <list>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <SFML/Graphics.hpp>

using namespace std;

#include "spectrum/object.h"
#include "spectrum/exception.h"
#include "spectrum/engine/rect.h"
#include "spectrum/engine/sprite.h"
#include "spectrum/engine/gameobject.h"
#include "spectrum/engine/room.h"
#include "spectrum/engine/engine.h"

#include <gl/gl.h>

GameObject::GameObject():Object()
{
    SetName("GameObject");
    current_sprite = NULL;
    x = 0;
    y = 0;
    depth = 0;
    dead = false;
    coll = false;
    this->can_collide = false;
}

GameObject::GameObject(const GameObject& C):Object()
{
    SetName("GameObject");
    this->current_sprite = C.current_sprite;
    this->x = C.x;
    this->y = C.y;
    this->depth = C.depth;
    this->dead = C.dead;
    coll = false;
    this->can_collide = C.can_collide;
}

GameObject::~GameObject()
{

}

int GameObject::GetDepth()
{
    return this->depth;
}

void GameObject::OnAdd(){}
void GameObject::OnRemove(){}
void GameObject::OnUpdate()
{
    coll = false;
}
void GameObject::OnDraw()
{
    if(this->current_sprite)
    {
        current_sprite->Draw(x,y);
    }
}
void GameObject::OnHit(GameObject& other)
{
    cout <<"Collision between (" << GetID() << ") and (" << other.GetID() << ")" << endl;
    dead = true;
    coll = true;
}

void GameObject::OnClick(int mx, int my)
{
    // Handle click checks here
    Rect R;
    R.x = x;
    R.y = y;
    R.w = 16;
    R.h = 16;

    if(R.ColPoint(mx,my))
    {
        dead = true;
    }
}

void GameObject::DrawHitBox()
{
    // Draws this object's hitbox (takes origin information into account)
    if(!current_sprite)return;

    glPushMatrix();
        Rect Box = current_sprite->GetBBox();
        int sx = Box.x + x-current_sprite->GetOffsetX();
        int sy = Box.y + y-current_sprite->GetOffsetY();

        glColor4f(1.0,0.0,0.0,1.0);
        glBegin(GL_LINE_STRIP);
        glVertex2f(sx,sy);
        glVertex2f(sx+Box.w,sy);
        glVertex2f(sx+Box.w,sy+Box.h);
        glVertex2f(sx,sy+Box.h);
        glVertex2f(sx,sy);
        glEnd();
    glPopMatrix();
}
