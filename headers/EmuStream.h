// An NSF stream

//#define buffer_size 8192
//#define buffer_size 16384

#ifndef EMUSTREAM_H
#define EMUSTREAM_H

#include <gme/Music_Emu.h>
#include <SFML/Audio.hpp>

//#include "common.h"

const int fill_rate = 45;
const int buffer_size = 16384;

class emuStream:public sf::SoundStream
{
    public:

    Music_Emu *emu;
    int number_of_tracks;
    int c_track;
    short int *last_sample;
    int sample_count;

    bool paused;

    emuStream();

    emuStream(std::string fname, int rate = 44100);

    virtual bool OnOpen();

    virtual int GetTracks();

    virtual void SetTrack(int track);

    virtual void NextTrack();

    short int GetLastSample();

    virtual bool SaveBuffer(std::string filename);

    virtual bool onGetData(Chunk& Data);
    virtual void onSeek(sf::Time timeOffset);

    ~emuStream();
};

#endif
