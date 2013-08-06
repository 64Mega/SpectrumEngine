// Billboard implementation
// --

#include "billboard.h"

Billboard::Billboard()
{
    rotation = 0.0F;
    spr = new Sprite("ENM_GRUNT",1);
    spr->SetSpeed(0);
    spr->SetFrame(0);

    frame = 0;
    pos = MXVector(0,0,0);
}

Billboard::Billboard(float x, float y, float z)
{
    rotation = 0.0F;
    spr = new Sprite("ENM_GRUNT",1);
    spr->SetSpeed(0);
    spr->SetFrame(0);

    frame = 0;
    pos = MXVector(x,y,z);
}

Billboard::~Billboard()
{
    delete spr;
}

void Billboard::Update(tPlayer& player)
{
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::H))
    {
        pos.x = player.x;
        pos.y = player.y;
        pos.z = -player.z;

        printf("New position: %.2f, %.2f, %.2f\n",pos.x,pos.y,pos.z);
    }
    float rry = player.ry+10;
    while(rry > 360.0F)rry -= 360.0F;
    while(rry < 0.0F)rry += 360.0F;
    float dd = abs(rotation - rry);
    frame = round(dd/60.0F);
    if(frame > 5)frame = 5;
    if(frame < 0)frame = 0;

}

void Billboard::Render()
{
    // Draw the billboard
    spr->SetFrame(frame);
    spr->Bind();
    sf::Rect<float> txc = spr->Pick();

    //glLoadIdentity();

    GLfloat viewMatrix[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, viewMatrix);

    MXVector up(viewMatrix[1],viewMatrix[5],viewMatrix[9]);
    MXVector right(viewMatrix[0],viewMatrix[4],viewMatrix[8]);
    MXVector point(pos.x,pos.y+1.5,-pos.z);

    //glTranslatef(pos.x,pos.y+1.5,-pos.z);
    glRotatef(rotation,0,1,0);
    glDisable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glColor4f(1.0F,1.0F,1.0F,1.0F);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_QUADS);
    glTexCoord2f(txc.left,txc.height);
    glVertex3dv((point + (right + up) * -1.5F).v());
    glTexCoord2f(txc.width,txc.height);
    glVertex3dv((point + (right - up) * 1.5F).v());
    glTexCoord2f(txc.width,txc.top);
    glVertex3dv((point + (right + up) * 1.5F).v());
    glTexCoord2f(txc.left,txc.top);
    glVertex3dv((point + (up - right) * 1.5F).v());
    glEnd();

    //glDisable(GL_BLEND);
    //glEnable(GL_LIGHTING);
}


