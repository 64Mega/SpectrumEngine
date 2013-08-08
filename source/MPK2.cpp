// Implementation file

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

#include "mpk2.h"
#include "mio.h"

MPK2::MPK2()
{
    data = NULL;
}

MPK2::~MPK2()
{
    if(data)
    {
        delete[] data;
        data = NULL;
    }
}

char* MPK2::GetDataPtr(string name)
{
    for(int i = 0; i < nt_names.size(); i++)
    {
        if(name == nt_names[i])
        {
            return &data[nt_offset[i]];
        }
    }

    printf("Couldn't find %s!\n",name.c_str());

    return data;
}

void MPK2::LoadMPK(string fname)
{
    // Loads the MPK file.

    FILE *fin = fopen(fname.c_str(),"rb");
    if(!fin)
    {
        printf("Error opening %s for reading!\n",fname.c_str());
        return;
    }

    // Get total file length
    _finddata_t fdd;
    _findfirst(fname.c_str(),&fdd);
    if(fdd.size < 4)
    {
        printf("File too small.\n");
        fclose(fin);
        return;
    }

    char* stg = new char[5];
    stg[4] = '\0';
    fread(stg,sizeof(char),4,fin);
    printf("Tag read: %s\n",stg);
    delete[] stg;

    // Read all pairs in
    unsigned int tlength = 0;
    do
    {
        fread(&tlength,sizeof(unsigned int),1,fin);
        if(tlength > 0)
        {
            char* tb = new char[tlength+1];
            tb[tlength] = '\0';
            fread(tb,sizeof(char),tlength,fin);

            nt_names.push_back(string(tb));
            delete[] tb;

            unsigned int fl = 0;
            fread(&fl,sizeof(unsigned int),1,fin);
            nt_size.push_back(fl);
        }
    }while(tlength != 0);

    // Calculate total data size
    unsigned int paksize = 0;
    for(int i = 0; i < nt_size.size(); i++)
    {
        nt_offset.push_back(paksize);
        paksize += nt_size[i];
    }

    // Allocate and read in the rest of the data.
    data = new char[paksize];
    fread(data,sizeof(char),paksize,fin);

    // Done.
    printf("Read all data... %d entries found...\n",nt_size.size());
    for(int i = 0; i < nt_names.size(); i++)
    {
        printf("Found: %s\n",nt_names[i].c_str());
    }

    printf("Total of %d bytes of data...\n",paksize);

    fclose(fin);
}

unsigned int MPK2::GetLength(string name)
{
    for(int i = 0; i < nt_names.size(); i++)
    {
        if(name == nt_names[i])
        {
            return nt_size[i];
        }
    }

    return 0;
}

void MPK2::Write(string name)
{
    FILE* fout;

    // Writes the given file to an actual file. Used to write the 'unlockables'.
    bool found;
    int index;
    for(int i = 0; i < nt_names.size(); i++)
    {
        if(nt_names[i] == name)
        {
            found = true;
            index = i;
            break;
        }
    }

    if(!found)
    {
        printf("%s not found in PAK!\n",name.c_str());
        return;
    }

    fout = fopen(nt_names[index].c_str(),"wb");
    if(!fout)
    {
        printf("Couldn't write %s!\n",nt_names[index].c_str());
        return;
    }
    fwrite(GetDataPtr(nt_names[index]),sizeof(char),GetLength(nt_names[index]),fout);

    fclose(fout);
}
