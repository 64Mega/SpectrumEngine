// A general exception that takes advantage of the Object class

#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <iostream>
#include <fstream>
#include <time.h>

#include "spectrum.h"

using namespace std;

class Exception
{
    public:
        Exception();
        Exception(Object* object, std::string message = "No Message");
        ~Exception();
    private:
        void Log(Object* object, std::string message); // Logs to Exception.log
};

// Stowing the log class in here too.
// The GameLog class is static. On game start GameLog::Initialize() should be called to
// clear the file game.log

class GameLog
{
    public:
        GameLog()
        {
            throw Exception(new Object,"[GAMELOG] You aren't supposed to instance the GameLog class...");
        }

        ~GameLog(){}

        static void Write(string type, string line)
        {
            // Make sure to timestamp everything in a sensible way
            tm *mx_time;
            time_t xtime = time(NULL);
            mx_time = localtime(&xtime);

            ofstream fout;
            fout.open("game.log",ios::app);
            fout << "[" << type << "] (" << mx_time->tm_hour << ":" << mx_time->tm_min << ":" << mx_time->tm_sec << ") - " << line << endl;
            fout.flush();
            fout.close();
        }

        static void Initialize()
        {
            ofstream fout;
            fout.open("game.log",ios::trunc);
            fout << "EXILE LOG:" << endl;
            fout.flush();
            fout.close();
        }
};

#endif
