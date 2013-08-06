// A GameObject
// Primary building block for the game

#ifndef GAMEOBJECT
#define GAMEOBJECT

#include "sprite.h"

class GameObject : public Object
{
    public:
        GameObject();
        GameObject(const GameObject& C);
        virtual ~GameObject();

        virtual void OnAdd();
        virtual void OnRemove();

        virtual void OnUpdate();
        virtual void OnDraw();

        virtual void OnClick(int mx, int my);

        void DrawHitBox();

        bool coll;
        bool can_collide;

        virtual void OnHit(GameObject& other);

        int GetDepth();

        int x, y;
        int depth;
        bool dead;
        Sprite* current_sprite;
};

#endif

