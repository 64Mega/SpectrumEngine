// Exception implementation

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#include "spectrum/object.h"
#include "spectrum/exception.h"

void Exception::Log(Object* object, std::string message)
{
    // Writes the output to a file named "Exception.log"
    ofstream out;
    out.open("Exception.log",ios::app);
    out << "Exception thrown by instance of " << object->GetName() << " (ID: " << object->GetID() << ")" << endl;
    out << "Message: " << message << endl << "----" << endl;
    out.close();
}

Exception::Exception()
{
    // Undefined Exception
    cerr << "Undefined exception thrown!" << endl;
    Log(new Object, "Undefined Exception thrown!");
}

Exception::Exception(Object* object, std::string message)
{
    cerr << "Exception thrown by instance of " << object->GetName() << " (ID: " << object->GetID() << ")" << endl;
    cerr << "Message: " << message << endl;

    Log(object, message);
}

Exception::~Exception()
{

}

