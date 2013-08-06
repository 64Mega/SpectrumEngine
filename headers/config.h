// Provides configuration options.
// If you're adding a new option, put it here.

#ifndef CONFIG_H
#define CONFIG_H

#include <MIO.h>
#include <string>

class t_Config
{
public:

    int width;
    int height;
    bool fullscreen;

    bool music;
    bool sound;

    int music_volume;
    int sound_volume;

    int gamma;

    bool gamepad;

    std::string list_assets;
    std::string pack_assets;

    t_Config();

    ~t_Config();

    void Load();
    void Write();
};

extern t_Config global_config;

#endif // CONFIG_H
