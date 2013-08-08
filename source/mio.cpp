// Template generated by HeLex
// Remember to include the necessary files!

#include "mio.h"

#include <iostream>
#include <fstream>

using namespace std;

namespace MIO
{

std::string getline()
{
    string ret;
    getline(cin, ret);

    return ret;
}

bool same(std::string str1, std::string str2)
{
    return str1.compare(str2) == 0;
}

std::string fgetline(ifstream& in)
{
    string ret;
    getline(in, ret);

    return ret;
}

void printvs(std::vector<std::string> dat)
{
    // Print a vector out
    for(vector<string>::iterator i = dat.begin(); i != dat.end(); i++)
    {
        cout << (*i) << endl;
    }
}

std::vector<std::string> split(std::string str, char delim)
{
    // Splits the given string up by delim
    vector<string> buffer;
    string temp = "";

    for(unsigned int i = 0; i < str.size(); i++)
    {
        if(str[i] == delim)
        {
            // Cut off
            if(temp != "")
            {
                buffer.push_back(temp);
                temp = "";
            }
            continue;
        }

        temp += str[i];
    }

    if(temp != "")
    {
        buffer.push_back(temp);
        temp = "";
    }

    return buffer;
}

std::string strip(std::string str, char delim)
{
    // Remove all offending characters from a string :P
    string buffer = "";

    for(unsigned int i = 0; i < str.size(); i++)
    {
        if(str[i] != delim)buffer += str[i];
    }

    return buffer;
}

std::string upper(std::string str)
{
    string tmp = str;
    // Converts all chars to uppercase
    for(unsigned int i = 0; i < tmp.size(); i++)
    {
        tmp[i] = toupper(tmp[i]);
    }

    return tmp;
}

std::string lower(std::string str)
{
    string tmp = str;
    // Converts all chars to lowercase
    for(unsigned int i = 0; i < tmp.size(); i++)
    {
        tmp[i] = tolower(tmp[i]);
    }

    return tmp;
}

bool haschar(std::string str, char d)
{
    // Returns true if a string has a given char
    for(unsigned int i = 0; i < str.size(); i++)
    {
        if(str[i] == d)return true;
    }

    return false;
}

}