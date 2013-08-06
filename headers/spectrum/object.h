// Generic Object, which is very useful for exception handling and general management

#ifndef OBJECT_H
#define OBJECT_H

#include <string>

class Object
{
    public:
        Object();
        ~Object();

        int         GetID();
        std::string GetName();
    protected:
        void SetName(std::string name);
    private:
        std::string name;
        int id;

        static int g_id;
};

#endif
