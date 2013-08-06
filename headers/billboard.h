// Tests billboard sprites, in 1 of 6 directions depending on player orientation.

#ifndef BILLBOARD_H
#define BILLBOARD_H

#include "common.h"
#include "mxmath.h"

#include "spectrum/engine/sprite.h"

#include "player.h"

class Billboard
{
    public:
        MXVector pos;
        float rotation;
        Sprite* spr;
        int frame;

        Billboard();

        Billboard(float x, float y, float z);

        ~Billboard();

        void Update(tPlayer& player);

        void Render();
};

#endif
