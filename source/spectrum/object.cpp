// Object implementation

#include <stdio.h>
#include <string>

using namespace std;

#include "spectrum/object.h"

int Object::g_id = 0;

Object::Object()
{
    this->id = Object::g_id++;
    this->name = "Object";
}

Object::~Object()
{

}

int Object::GetID()
{
    return this->id;
}

std::string Object::GetName()
{
    return this->name;
}

void Object::SetName(std::string name)
{
    this->name = name;
}

