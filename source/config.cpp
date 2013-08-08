// Implementation of the config class

#include "config.h"

#include <list>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

t_Config::t_Config()
{
    width = 640;
    height = 480;
    fullscreen = false;
    list_assets = "./assets.dat";
    music = false;
    sound = false;
    music_volume = 255;
    sound_volume = 255;
}

t_Config::~t_Config()
{

}

void t_Config::Load()
{
    // Load from config.dat
    //FILE* fin = fopen("./config.dat","r");
    ifstream fin("./config.dat");
    if(!fin.is_open())
    {
        printf("[WARNING] No config.dat. Generating default settings...\n");
        return;
    }

    vector<string> lines;
    string ss = "";
    while(!fin.eof())
    {
        ss = MIO::fgetline(fin);
        if(ss != "EOF")lines.push_back(ss);
    }

    // Parse config

    for(unsigned int i = 0; i < lines.size(); i++)
    {
        if(lines[i].empty())continue;
        vector<string> spl = MIO::split(lines[i],' ');
        if(spl[0][0] == '#')continue; // Comment line

        string token = MIO::lower(spl[0]);

        if(MIO::same(token,"fullscreen"))
        {
            fullscreen = true;
            cout <<"[CONFIG] Fullscreen mode" << endl;
        }

        if(MIO::same(token,"gamepad"))
        {
            gamepad = true;
            cout << "[CONFIG] Gamepad enabled" << endl;
        }

        if(MIO::same(token,"music"))
        {
            music = true;
            cout <<"[CONFIG] Music enabled" << endl;
        }
        if(MIO::same(token,"sound"))
        {
            sound = true;
            cout <<"[CONFIG] Sound enabled" << endl;
        }

        if(MIO::same(token,"musicvolume") && spl.size() > 1)
        {
            music_volume = atoi(spl[1].c_str());
            cout <<"[CONFIG] Music volume set to " << music_volume << endl;
        }
        if(MIO::same(token,"soundvolume") && spl.size() > 1)
        {
            sound_volume = atoi(spl[1].c_str());
            cout <<"[CONFIG] Sound volume set to " << sound_volume << endl;
        }
        if(MIO::same(token,"gammalevel") && spl.size() > 1)
        {
            gamma = atoi(spl[1].c_str());
            cout <<"[CONFIG] Brightness/Gamma set to " << gamma << endl;
        }

        if(MIO::same(token,"assetlist") && spl.size() > 1)
        {
            // Asset list
            string lst_a = MIO::strip(spl[1],'\n');
            lst_a = MIO::strip(lst_a,'\r');
            lst_a = MIO::strip(lst_a,' ');
            lst_a = MIO::strip(lst_a,'\0');

            list_assets = string(lst_a);
            cout <<"[CONFIG] Asset list '" << list_assets << "' selected" << endl;
        }

        if(MIO::same(token,"assetpack") && spl.size() > 1)
        {
            // Asset list
            string lst_a = MIO::strip(spl[1],'\n');
            lst_a = MIO::strip(lst_a,'\r');
            lst_a = MIO::strip(lst_a,' ');
            lst_a = MIO::strip(lst_a,'\0');

            pack_assets = string(lst_a);
            cout <<"[CONFIG] Asset pack '" << pack_assets << "' selected" << endl;
        }
    }

    fin.close();
}

void t_Config::Write()
{
    FILE* fout = fopen("./config.dat","w");
    if(!fout)
    {
        printf("[ERROR] Config not saved!\n");
        printf("        Could not open 'config.dat' for writing!\n");
        return;
    }

    // Write standard commentary
    fprintf(fout,"# Exile Config File\n");
    fprintf(fout,"# -----------------\n\n");
    fprintf(fout,"# Uncomment the next line for fullscreen\n");

    string s_fullscreen = fullscreen ? "Fullscreen" : "#Fullscreen";
    fprintf(fout,"%s\n\n",s_fullscreen.c_str());

    fprintf(fout,"# Comment out the next two lines to turn Music and Sound off\n");
    string s_music = music ? "Music" : "#Music";
    string s_sound = sound ? "Sound" : "#Sound";
    fprintf(fout,"%s\n",s_music.c_str());
    fprintf(fout,"%s\n\n",s_sound.c_str());

    fprintf(fout, "# Comment out the next line to enable gamepad support\n");
    string s_gpad = gamepad ? "Gamepad" : "#Gamepad";
    fprintf(fout, "%s\n\n",s_gpad.c_str());

    fprintf(fout,"# Set the music and sound volume here\n");
    fprintf(fout,"MusicVolume %d\n",music_volume);
    fprintf(fout,"SoundVolume %d\n\n",sound_volume);

    fprintf(fout,"# Gamma level AKA the brightness. 100 is full-bright, 80 is the default. Adjust at will.\n");
    fprintf(fout,"GammaLevel %d\n\n", gamma);

    fprintf(fout,"# Select the asset-list to use.\n");
    fprintf(fout,"AssetList %s\n\n",list_assets.c_str());

    fprintf(fout,"# PAK file to use (or use core.pak)\n");
    fprintf(fout,"AssetPack %s\n\n",pack_assets.c_str());
    fprintf(fout,"# End of config ---\n");

    fclose(fout);
}

t_Config global_config;
