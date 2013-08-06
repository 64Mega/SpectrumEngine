// Sprite Implementation

#include <stdio.h>
#include <string>
#include <SFML/Graphics.hpp>

using namespace std;

#include "spectrum/object.h"
#include "spectrum/exception.h"
#include "spectrum/engine/rect.h"
#include "spectrum/engine/assets.h"
#include "spectrum/engine/sprite.h"

#include <gl/gl.h>

Sprite::Sprite():()
{
    // Protected method. Not the desired interface to Sprite.
    SetName("Sprite");
    image = NULL;
    BBox = Rect(0,0,0,0);
    p_set = false;

    w = 0;
    h = 0;
    ox = 0;
    oy = 0;

    number_images = 0;
    current_image = 0;
    framecounter = 0.0F;
    framespeed = 1.0F;

    alpha = 0.0F;
    loop = true;

    Sprite::ClipPlane = Rect(0,0,640,480);

    flip_x = false;
    flip_y = false;
}

Sprite::Sprite(std::string assetName, int sub_images):()
{
    // Public method. The intended one.
    SetName("Sprite");
    image = Assets::GetImage(assetName);

    number_images = sub_images;
    w = image->getSize().x/number_images;
    h = image->getSize().y;

    BBox = Rect(0,0,w,h);
    p_set = true;

    ox = 0;
    oy = 0;

    current_image = 0;
    framecounter = 0.0F;
    framespeed = 1.0F;

    alpha = 1.0F;
    loop = true;
    Sprite::ClipPlane = Rect(0,0,640,480);

    flip_x = false;
    flip_y = false;
}

Sprite::Sprite(const Sprite& C):()
{
    this->alpha = C.alpha;
    this->BBox = C.BBox;
    this->w = C.w;
    this->h = C.h;
    this->ox = C.ox;
    this->oy = C.oy;
    this->p_set = C.p_set;
    this->image = C.image;
    this->number_images = C.number_images;
    this->framecounter = C.framecounter;
    this->framespeed = C.framespeed;
    this->current_image = C.current_image;
    this->loop = C.loop;

    this->flip_x = C.flip_x;
    this->flip_y = C.flip_y;

    SetName(C.GetName());
    Sprite::ClipPlane = Rect(0,0,640,480);
}

Sprite::~Sprite()
{
    image = NULL; // Pointer is handled by Asset Manager
}

sf::Rect<float> Sprite::Pick()
{
    // Given a frame number, this function returns (in GL Float coordinates)
    // The texture coordinates needed by the sprite.

    float hw = (this->w * this->number_images);
    float top = 0.0F;
    float bottom = 1.0F;
    float left = (float)((1.0/hw)*(this->w*this->current_image));
    float right = ((1.0/hw)*(this->w*this->current_image))+((1.0/hw)*w);

    sf::Rect<float> R;
    R.left = left;
    R.top = top;
    R.height = bottom;
    R.width = right;
    return R;
}

Rect Sprite::ClipPlane = Rect(0,0,640,480);

void Sprite::Step()
{
    if(loop == false && current_image >= number_images-1)
    {
        current_image = number_images-1;
        return;
    }
    // Advance frames if necessary
    framecounter += framespeed;
    if(framecounter >= 1.0F)
    {
        current_image++;
        framecounter = 0.0F;
        if(current_image >= number_images)
        {
            current_image = 0;
        }
    }
}

void Sprite::Draw(int x, int y, float r, float g, float b)
{
    // Draw the sprite

    sf::Rect<float> texco = Pick();

    if(flip_x)
    {
        float ol = texco.left;
        texco.left = texco.width;
        texco.width = ol;
    }
    if(flip_y)
    {
        float ol = texco.top;
        texco.top = texco.height;
        texco.height = ol;
    }

    if(image)
    {
        sf::Texture::bind(image);
    }

    x -= ox;
    y -= oy;

    if(x > Sprite::ClipPlane.x+Sprite::ClipPlane.w || x+w < Sprite::ClipPlane.x ||
       y > Sprite::ClipPlane.y+Sprite::ClipPlane.h || y+h < Sprite::ClipPlane.y )
       {
           return;
       }

    x = x + (-Sprite::ClipPlane.x);
    y = y + (-Sprite::ClipPlane.y);

    glPushMatrix();
    glLoadIdentity();
        glColor4f(r,g,b,alpha);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBegin(GL_QUADS);
        glTexCoord2f(texco.left,texco.top); glVertex2f(x,y);
        glTexCoord2f(texco.width,texco.top); glVertex2f(x+w,y);
        glTexCoord2f(texco.width,texco.height); glVertex2f(x+w,y+h);
        glTexCoord2f(texco.left,texco.height); glVertex2f(x,y+h);
        glDisable(GL_BLEND);
        glEnd();
    glPopMatrix();


    if(loop == false && current_image >= number_images-1)
    {
        current_image = number_images-1;
        return;
    }
    // Advance frames if necessary
    framecounter += framespeed;
    if(framecounter >= 1.0F)
    {
        current_image++;
        framecounter = 0.0F;
        if(current_image >= number_images)
        {
            current_image = 0;
        }
    }
}

Sprite& Sprite::SetOrigin(int x, int y)
{
    this->ox = x;
    this->oy = y;
    return (*this);
}

Sprite& Sprite::SetSpeed(float speed)
{
    this->framespeed = speed;
    return (*this);
}

void Sprite::SetBPos(int x, int y)
{
    BBox.x = x;
    BBox.y = y;
}

Sprite& Sprite::SetFrame(int frame)
{
    this->current_image = frame;
    return (*this);
}

int Sprite::GetWidth(){ return w; }
int Sprite::GetHeight(){ return h; }

void  Sprite::SetBBox(int x, int y, int w, int h)
{
    BBox.x = x;
    BBox.y = y;
    BBox.w = w;
    BBox.h = h;
}

Rect& Sprite::GetBBox()
{
    return BBox;
}

int Sprite::GetOffsetX(){ return ox; }
int Sprite::GetOffsetY(){ return oy; }

int Sprite::CurrentIndex(){ return current_image; }
int Sprite::Images(){ return number_images; }
