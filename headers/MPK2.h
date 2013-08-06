// The MPK2 class
// Needs to be able to read in and store objects, their lengths, and also
// be able to write them to individual files if necessary.

#ifndef MPK2_H
#define MPK2_H

#include "common.h"

class MPK2
{
    public:
    void LoadMPK(string fname);
    char* GetDataPtr(string name);
    unsigned int GetLength(string name);
    unsigned int GetOffset(string name);
    void Write(string name);

    MPK2();
    ~MPK2();

    private:
    vector<string> nt_names;
    vector<unsigned int> nt_size;
    vector<unsigned int> nt_offset;

    char* data;
};

#endif
