// Tileset implementation

#include "spectrum/spectrum.h"
#include "spectrum/engine/rect.h"
#include "spectrum/engine/tileset.h"

#include <gl/gl.h>

using namespace std;

Tileset::Tileset(std::string assetName):()
{
    SetName("Tileset");
    width = 0;
    height = 0;
    tiledata = Assets::GetImage(assetName);
}

Tileset::~Tileset()
{
    tiledata = NULL;
}

Rect Tileset::ClipPlane = Rect(0,0,640,480);

void Tileset::Setup(int tw, int th)
{
    width = tw;
    height = th;
}

int Tileset::GetTileWidth()
{
    return width;
}
int Tileset::GetTileHeight()
{
    return height;
}

sf::Rect<float> Tileset::BindTile(int t, bool building)
{
    sf::Rect<float> txRect;

    if(building == true)
    {
        //tiledata->Bind();
        sf::Texture::bind(tiledata);
        return txRect;
    }

    int sx = 0;
    int sy = 0;
    static int tgy = tiledata->getSize().y/height;
    static int tgx = tiledata->getSize().x/width;
//    sy = t / (tiledata->GetWidth() / height);
//    sx = t % (tiledata->GetWidth() / width);
    sy = t / tgy;
    sx = t % tgx;


//    float hw = (tiledata->GetWidth());
//    float hh = (tiledata->GetHeight());
//    txRect.Left = (float)((1.0/hw)*(width*sx));
//    txRect.Top =  (float)((1.0/hh)*(height*sy));
//    txRect.Right = (float)((1.0/hw)*(width*sx))+((1.0/hw)*width);
//    txRect.Bottom = (float)((1.0/hh)*(height*sy))+((1.0/hh)*height);
    float hw = 1.0/(tiledata->getSize().x);
    float hh = 1.0/(tiledata->getSize().y);
    txRect.left = (float)((hw)*(width*sx));
    txRect.top =  (float)((hh)*(height*sy));
    txRect.width = (float)((hw)*(width*sx))+((hw)*width);
    txRect.height = (float)((hh)*(height*sy))+((hh)*height);

    sf::Texture::bind(tiledata);

    return txRect;
}

void Tileset::Draw(int t, int x, int y)
{
    int sx = 0;
    int sy = 0;

    Rect R(x,y,width,height);
    if(!Tileset::ClipPlane.ColRect(R))return;

    int tx = x+(-Tileset::ClipPlane.x);
    int ty = y+(-Tileset::ClipPlane.y);

    sy = t / (tiledata->getSize().y / height);
    sx = t % (tiledata->getSize().y / width);

    // Calculate the correct texture coordinates
    sf::Rect<float> txRect;
    float hw = (tiledata->getSize().x);
    float hh = (tiledata->getSize().y);
    txRect.left = (float)((1.0/hw)*(width*sx));
    txRect.top =  (float)((1.0/hh)*(height*sy));
    txRect.width = (float)((1.0/hw)*(width*sx))+((1.0/hw)*width);
    txRect.height = (float)((1.0/hh)*(height*sy))+((1.0/hh)*height);

    sf::Texture::bind(tiledata);

    glPushMatrix();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_QUADS);
    glTexCoord2f(txRect.left,txRect.top);
    glVertex2f(tx,ty);
    glTexCoord2f(txRect.width,txRect.top);
    glVertex2f(tx+width,ty);
    glTexCoord2f(txRect.width,txRect.height);
    glVertex2f(tx+width,ty+height);
    glTexCoord2f(txRect.left,txRect.height);
    glVertex2f(tx,ty+height);
    glEnd();
    glPopMatrix();
}

int Tileset::GetWidth()
{
    return width;
}
int Tileset::GetHeight()
{
    return height;
}
