// Implementation of ModelObj loader

#include <modelobj.h>
#include <sfml/graphics.hpp>
#include <string>
#include <gl/gl.h>
#include <gl/glu.h>
#include <MIO.h>
#include <vector>

using namespace std;

ModelObj::ModelObj()
{
    texture = "";
    verts = NULL;
}

ModelObj::~ModelObj()
{
    if(verts)delete[] verts;
}

void ModelObj::loadFromFile(std::string filename)
{
    FILE* fin = fopen(filename.c_str(),"r");
    if(!fin)
    {
        throw string("Couldn't open file '" + filename + "'...");
    }

    vector<string> lines;


    fclose(fin);
}

void ModelObj::render()
{

}
