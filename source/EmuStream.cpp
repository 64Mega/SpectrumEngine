// EmuStream - the better, slightly more improved version of NSFStream.

#include "EmuStream.h"

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <SFML/Audio.hpp>
#include <gme/gme.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>


class WinException
{
    public:
        WinException()
        {
            // Default message
            MessageBox(NULL,"Undefined Exception","GMGME: Exception Thrown!", MB_OK);
        }

        WinException(std::string message)
        {
            MessageBox(NULL,message.c_str(),"GMGME: Exception Thrown!", MB_OK);
        }

        ~WinException(){}
};


emuStream::emuStream()
{
    emu = NULL;
    //gme_err_t err = gme_open_file("Megaman9.nsfe",&emu,44100);
    gme_err_t err = gme_open_file("default.nsf",&emu,buffer_size*2);
    paused = false;
    if(err)
    {
        //printf("ERROR: %s\n",err);
        throw WinException("Couldn't load default.nsf!");
    }

    sample_count = 0;
    last_sample = NULL;

    number_of_tracks = gme_track_count(emu);
    c_track = 0;

    gme_start_track(emu,c_track);

    initialize(2,buffer_size*2);
    //Initialize(2,44100);
}

emuStream::emuStream(std::string fname, int rate)
{
    emu = NULL;
    paused = false;
    //gme_err_t err = gme_open_file(fname.c_str(),&emu,44100);
    gme_err_t err = gme_open_file(fname.c_str(),&emu,48000);
    if(err)
    {
        //printf("ERROR: %s\n",err);
        throw WinException("Couldn't load " + fname + "!");
    }

    sample_count = 0;
    last_sample = NULL;

    number_of_tracks = gme_track_count(emu);
    c_track = 0;

    gme_start_track(emu,c_track);

    //Initialize(2,44100);
    initialize(2,48000);
}

bool emuStream::OnOpen()
{
    return true;
}

int emuStream::GetTracks()
{
    return gme_track_count(emu);
}

void emuStream::SetTrack(int track)
{
    if(track <= number_of_tracks-1)
    {
        gme_start_track(emu,track);
        c_track = track;
    }
}

void emuStream::NextTrack()
{
    if(c_track < number_of_tracks - 1)
    {
        c_track += 1;
    }
    gme_start_track(emu,c_track);
}

short int emuStream::GetLastSample()
{
    // Returns the value of the last sample processed, according to the buffer pointer
    if(!last_sample)return 0;
    short int temp = last_sample[sample_count];
    sample_count += 1;
    if(sample_count >= buffer_size)
    {
        sample_count = 0;
        if(last_sample)
        {
            free(last_sample);
            last_sample = NULL;
        }
    }

    return temp;
}

bool emuStream::SaveBuffer(std::string filename)
{
    // Saves the current contents of the buffer to a file
    FILE* fp = fopen(filename.c_str(),"w");
    if(!fp)
    {
        fprintf(stderr,"Error saving buffer to %s!\n",filename.c_str());
        return false;
    }

    for(int i = 0; i < buffer_size; i++)
    {
        fprintf(fp,"S: %d\n",last_sample[i]);
    }

    fclose(fp);

    return true;
}

bool emuStream::onGetData(Chunk& Data)
{
    short int *sndsample = NULL;
    if(last_sample != NULL)
    {
        free(last_sample);
        last_sample = NULL;
    }
    //sndsample = (short int*)malloc(sizeof(short int*)*buffer_size);
    sndsample = (short int*)malloc(buffer_size*2);
    if(!sndsample)
    {
        //printf("Memory error!\n");
        return false;
    }

    if(gme_track_ended(emu))
    {
        if(c_track < number_of_tracks - 1)
        {
            c_track += 1;
        }
        gme_start_track(emu,c_track);
    }

    gme_err_t err = gme_play(emu,buffer_size,&sndsample[0]);
    if(err)
    {

        //return false;
    } // Ignoring error... I guess...

    Data.samples = sndsample;
    Data.sampleCount = buffer_size;
    last_sample = sndsample;
    return true;
}

void emuStream::onSeek(sf::Time timeOffset)
{

}

emuStream::~emuStream()
{
    gme_delete(emu);
    emu = NULL;
}

/// NEW STATUS INDICATORS


