// Sprite class - Can load still images or sprite-strips, handles animation and collision detection
// data

#ifndef SPRITE_H
#define SPRITE_H

#include "rect.h"
#include "../object.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
class Sprite : public Object
{
    public:
        Sprite(std::string assetName, int sub_images = 1);  // Loads specifically from the Asset manager
        Sprite(const Sprite& C);
        ~Sprite();

        void  SetBBox(int x, int y, int w, int h);
        Rect& GetBBox();

        int GetWidth();
        int GetHeight();

        int GetOffsetX();
        int GetOffsetY();

        Sprite& SetOrigin(int x, int y);
        Sprite& SetSpeed(float speed);
        Sprite& SetFrame(int frame);

        void SetBPos(int x, int y);

        void Draw(int x, int y, float r = 1.0F, float g = 1.0F, float b = 1.0F);
        void Step();

        float alpha;
        static Rect ClipPlane;
        bool flip_x, flip_y;

        bool IsLooped(){return loop;}
        bool SetLoop(bool l){loop = l; return l;}

        int CurrentIndex();
        int Images();

        bool IsLastFrame(){ return current_image >= number_images-1; }
        sf::Rect<float> Pick(); // Picks texture coordinates for current frame
        void Bind()
        {
            if(image)image->bind(image,sf::Texture::Normalized);
        }

        int w, h;
        int current_image;
        int number_images;

        sf::Texture* image;
    protected:
        int ox, oy;

        float framecounter;
        float framespeed;

        Rect BBox;

        Sprite();

    private:
        bool p_set;
        bool loop; // Does the sprite loop it's animation?
};

#endif
