// A tileset is basically a uniform texture map similar to the sprite's functionality, but
// with the intention of repeatedly drawing individual units of the image to any location
// on the screen.

#ifndef TILESET_H
#define TILESET_H

#include "../object.h"
#include "../exception.h"
//#include "../stdinc.h"

class Tileset: public Object
{
    public:
        Tileset(std::string assetName);
        ~Tileset();

        void Setup(int tw, int th);

        void Draw(int t, int x, int y);

        static Rect ClipPlane;

        sf::Rect<float> BindTile(int t, bool building = false);
        int GetWidth();
        int GetHeight();
        int GetTileWidth();
        int GetTileHeight();

    protected:
        int width;
        int height;
        sf::Texture* tiledata;
};

#endif
