// Assets implementation

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

#include <mio.h>
#include <mxmath.h>
#include "spectrum/engine/assets.h"
#include "spectrum/object.h"
#include "spectrum/exception.h"
#include "spectrum/engine/gameobject.h"

/**
    A static member function that you use to load all the assets required by
    a game.
*/

void Assets::LoadAssets()
{
    // You shouldn't load your assets here, instead create your own function that calls Assets::Load*() at startup.
}
// End Load Assets

/**
    Default constructor. Because Assets is supposed to strictly remain a static class, it
    will not allow itself to be created as an instance.
*/

Assets::Assets()
{
    throw (Exception(new Object, "Class 'Assets' is not supposed to be instanced!"));
}

/**
    Default destructor. Does nothing.
*/

Assets::~Assets(){}


/**
    Special initialization code. Currently empty.
*/
void Assets::Initialize(){}

/**
    Destruction code. Frees all memory from load calls in LoadAssets()
*/
void Assets::Destroy()
{
    typedef vector<sf::Texture*>::iterator iter;
    int j = 0;
    for(iter i = ls_images.begin(); i != ls_images.end(); i++)
    {
        //cout <<"Removing Image '" << nt_images[j] << "' from Asset Manager";
        //GameLog::Write("ASSETS","Removed Image '" + nt_images[j] + "'");
        delete (*i);
        (*i) = NULL;
        //cout <<"      ...Done" << endl;
        j++;
    }

    char isc[4] = {' ',' ',' ','\0'};
    sprintf(isc,"%d",j);
    GameLog::Write("ASSETS","Removed " + string(isc) + " images from memory...");

    typedef vector<GameObject*>::iterator iter2;
    j = 0;
    for(iter2 i = ls_objects.begin(); i != ls_objects.end(); i++)
    {
        //cout <<"Removing Object '" << ls_objects[j]->GetName() << "' from Asset Manager";
        //GameLog::Write("ASSETS","Removed Object '" + ls_objects[j]->GetName() + "'");
        delete (*i);
        (*i) = NULL;
        //cout <<"      ... Done" << endl;
        j++;
    }

    char iso[4] = {' ',' ',' ','\0'};
    sprintf(iso,"%d",j);
    GameLog::Write("ASSETS","Removed " + string(iso) + " objects from memory...");

    typedef vector<sf::Sound*>::iterator iter3;
    j = 0;
    for(iter3 i = ls_sounds.begin(); i != ls_sounds.end(); i++)
    {
        //cout <<"Removing Sound from memory..." << endl;
        delete (*i);
        (*i) = NULL;
        j++;
    }

    char iss[4] = {' ',' ',' ','\0'};
    sprintf(iss,"%d",j);
    GameLog::Write("ASSETS","Removed " + string(iss) + " sounds from memory...");

    typedef vector<sf::SoundBuffer*>::iterator iter4;
    for(iter4 i = ls_sbuffers.begin(); i != ls_sbuffers.end(); i++)
    {
        delete (*i);
        (*i) = NULL;
    }

    ls_images.clear();
    ls_objects.clear();
    ls_sounds.clear();
    ls_sbuffers.clear();
    nt_music.clear();
    nt_images.clear();
    nt_levels.clear();

    if(myEmuStream)
    {
        myEmuStream->stop();
        delete myEmuStream;
    }
}

/**
    Loads and adds a new sf::Image to the table.
    @param filename The absolute or relative path and name of the image file to load.
    @param ident A descriptive name or tag for the image (E.G. "player jump")
*/

std::vector<std::string> Assets::nt_levels;

std::string Assets::GetLevel(unsigned int idex)
{
    if(idex < nt_levels.size())return nt_levels[idex];
    else return nt_levels[0];
}

void Assets::AddLevel(std::string lname)
{
    nt_levels.push_back(lname);
}

void Assets::AddImage(std::string filename, std::string ident)
{
    for(unsigned int i = 0; i < nt_images.size(); i++)
    {
        if(nt_images[i] == ident)
        {
            throw Exception(new Object, "AssetManager: ident '" + ident + "' already defined!");
        }
    }
    sf::Texture* img = new sf::Texture;
    img->loadFromFile(filename);
    img->setSmooth(false);

    ls_images.push_back(img);
    nt_images.push_back(ident);

    //cout <<"Added Image '" << filename << "' as '" << ident << "' to Assets" << endl;
    GameLog::Write("ASSETS","Added image '" + filename + "' identified by '" + ident + "' to assets...");
}

void Assets::AddImageX(std::string filename, std::string ident)
{
    for(unsigned int i = 0; i < nt_images.size(); i++)
    {
        if(nt_images[i] == ident)
        {
            throw Exception(new Object, "AssetManager: ident '" + ident + "' already defined!");
        }
    }
    sf::Texture* img = new sf::Texture;
    img->loadFromMemory(packfile.GetDataPtr(filename),packfile.GetLength(filename));
    img->setSmooth(false);

    ls_images.push_back(img);
    nt_images.push_back(ident);

    //cout <<"Added Image '" << filename << "' as '" << ident << "' to Assets" << endl;
    GameLog::Write("ASSETS","Added image '" + filename + "' identified by '" + ident + "' to assets...");
}

void Assets::AddDecal(std::string filename, std::string ident)
{
    for(unsigned int i = 0; i < nt_decals.size(); i++)
    {
        if(nt_decals[i] == ident)
        {
            throw Exception(new Object, "AssetManager: ident '" + ident + "' already defined!");
        }
    }
    sf::Texture* img = new sf::Texture;
    img->loadFromFile(filename);
    img->setSmooth(false);

    ls_decals.push_back(img);
    nt_decals.push_back(ident);

    //cout <<"Added Image '" << filename << "' as '" << ident << "' to Assets" << endl;
    GameLog::Write("ASSETS","Added decal '" + filename + "' identified by '" + ident + "' to assets...");
}

void Assets::AddDecalX(std::string filename, std::string ident)
{
    for(unsigned int i = 0; i < nt_decals.size(); i++)
    {
        if(nt_decals[i] == ident)
        {
            throw Exception(new Object, "AssetManager: ident '" + ident + "' already defined!");
        }
    }
    sf::Texture* img = new sf::Texture;
    img->loadFromMemory(packfile.GetDataPtr(filename),packfile.GetLength(filename));
    img->setSmooth(false);

    ls_decals.push_back(img);
    nt_decals.push_back(ident);

    //cout <<"Added Image '" << filename << "' as '" << ident << "' to Assets" << endl;
    GameLog::Write("ASSETS","Added decal '" + filename + "' identified by '" + ident + "' to assets...");
}

/**
    @param obj Instance of object to be used as template
    @return Index (ID) assigned to instance.
    This function adds an instance of a given GameObject to the Assets list.
    It returns an index value that can be used with the Instantiate() function.
*/

int Assets::AddObject(GameObject* obj)
{
    ls_objects.push_back(obj);
    //cout <<"Added object type '" << obj->GetName() << "' to Assets" << endl;
    GameLog::Write("ASSETS","Added Object '" + obj->GetName() + "' to Assets");
    return ls_objects.size()-1;
}

/**
    @param index Index of required object (ITEM ID)
    @param x X position (Absolute) to instantiate the new object at.
    @param y Y position (Absolute) to instantiate the new object at.
    @return Pointer to GameObject.
    Creates a copy instance of the object at the given index.
*/

GameObject* Assets::Instantiate(unsigned int index, int x, int y)
{
    try
    {
        if(index > ls_objects.size()-1 || index < 0)
        {
            throw Exception(new Object, "Instance ID out of range (Assets::Instantiate)");
        }
    }
    catch(Exception e)
    {
        //cout <<"[NON-CRITICAL]" << endl;
        GameLog::Write("ASSETS - NON CRITICAL ERROR","Instance ID out of range in Assets::Instantiate");
        return NULL;
    }

    GameObject* tmp = new GameObject(*ls_objects[index]); // Copy construction
    tmp->x = x;
    tmp->y = y;
    return tmp;
}

/**
    Returns a pointer to an sf::Image.
    @param name The name/tag of the image as supplied in AddImage().
    @return Pointer to sf::Image.
    Throws an instance of Exception on failure with a message.
*/

sf::Texture* Assets::GetImage(std::string name)
{
    for(unsigned int i = 0; i < nt_images.size(); i++)
    {
        if(nt_images[i] == name)return ls_images[i];
    }

    throw Exception(new Object, "No Image with name '" + name + "' found!");
}

sf::Texture* Assets::GetImageByIndex(unsigned int index)
{
    if(index < nt_images.size() && index >= 0)
    {
        return ls_images[index];
    }
    else return ls_images[0];
}

sf::Texture* Assets::GetDecal(std::string name)
{
    for(unsigned int i = 0; i < nt_decals.size(); i++)
    {
        if(nt_decals[i] == name)return ls_decals[i];
    }

    throw Exception(new Object, "No Image with name '" + name + "' found!");
}

sf::Texture* Assets::GetDecalByIndex(unsigned int index)
{
    if(index < nt_decals.size() && index >= 0)
    {
        return ls_decals[index];
    }
    else return ls_decals[0];
}

void Assets::LoadPack(std::string fname)
{
    packfile.LoadMPK(fname);
}

void Assets::LoadList(std::string fname)
{
    // Load a list of assets from a file
    string ff = fname;
    char *mcfn = new char[fname.size()-1];

    nt_levels.clear();

    for(unsigned int i = 0; i < fname.size()-1; i++)
    {
        mcfn[i] = fname[i];
    }
    mcfn[fname.size()-1] = '\0';

    //FILE* fin = fopen(ff.c_str(),"r");
    ifstream fin(ff.c_str());
    if(!fin)
    {
        printf("%s\n",mcfn);
        throw Exception(new Object, "[FATAL]Couldn't open asset-file '" + fname + "'!");
    }
    vector<std::string> lines;
    string temp = "";
    while(!fin.eof())
    {
        temp = MIO::fgetline(fin);
        if(temp != "EOF")lines.push_back(temp);
    }

    // Process lines
    for(unsigned int i = 0; i < lines.size(); i++)
    {
        if(lines[i].size() > 0)
        {
            if(lines[i][0] == '#')continue; // Comment line

            // Split and check
            vector<string> spl = MIO::split(lines[i],' ');
            if(spl.size() > 1)
            {
                if(MIO::same(MIO::lower(spl[0]),"tilewidth"))
                {
                    // Set tile-width
                    TileWidth = atoi(spl[1].c_str());
                }
                else
                if(MIO::same(MIO::lower(spl[0]),"tileheight"))
                {
                    // Set tile-height
                    TileHeight = atoi(spl[1].c_str());
                }
                else
                {
                    string assetType = "";
                    string assetName = "";
                    string assetFile = "";

                    assetType = MIO::lower(spl[0]);

                    if(assetType == "texture" && spl.size() >= 3)
                    {
                        assetName = MIO::strip(spl[1],'\t');
                        assetFile = MIO::strip(spl[2],'\t');
                        AddImage(assetFile,assetName);
                    }
                    if(assetType == "paktexture" && spl.size() >= 3)
                    {
                        assetName = MIO::strip(spl[1],'\t');
                        assetFile = MIO::strip(spl[2],'\t');
                        AddImageX(assetFile,assetName);
                    }
                    if(assetType == "decal" && spl.size() >= 3)
                    {
                        assetName = MIO::strip(spl[1],'\t');
                        assetFile = MIO::strip(spl[2],'\t');
                        AddDecal(assetFile,assetName);
                    }
                    if(assetType == "pakdecal" && spl.size() >= 3)
                    {
                        assetName = MIO::strip(spl[1],'\t');
                        assetFile = MIO::strip(spl[2],'\t');
                        AddDecalX(assetFile,assetName);
                    }
                    if(assetType == "map" && spl.size() >= 2)
                    {
                        string ss = MIO::strip(spl[1],'\t');
                        printf("Loaded map %s\n",ss.c_str());
                        AddLevel(ss);
                    }
                    if(assetType == "sound" && spl.size() >= 2)
                    {
                        string ss = MIO::strip(spl[1],'\t');
                        printf("Loaded sound effect %s\n",ss.c_str());
                        AddSound(ss);
                    }
                    if(assetType == "music" && spl.size() >= 2)
                    {
                        string ss = MIO::strip(spl[1],'\t');
                        printf("Loaded music effect %s\n",ss.c_str());
                        AddMusic(ss);
                    }
                }
            }
        }
    }
    //fclose(fin);
    fin.close();
}

// Static definitions
vector<sf::Texture*> Assets::ls_images;
vector<std::string> Assets::nt_images;

vector<sf::Texture*> Assets::ls_decals;
vector<std::string> Assets::nt_decals;

vector<GameObject*> Assets::ls_objects;

int Assets::TileWidth = 16;
int Assets::TileHeight = 16;

int Assets::svolume = 200;
int Assets::mvolume = 200;

vector<sf::SoundBuffer*> Assets::ls_sbuffers;
vector<sf::Sound*> Assets::ls_sounds;
vector <std::string> Assets::nt_music;

void Assets::AddMusic(std::string mname)
{
    nt_music.push_back(mname);
}

void Assets::AddSound(std::string sname)
{
    sf::SoundBuffer* sb = new sf::SoundBuffer();
    sb->loadFromFile(sname);
    sf::Sound* s = new sf::Sound(*sb);

    ls_sbuffers.push_back(sb);
    ls_sounds.push_back(s);
}

bool Assets::music = false;
bool Assets::sound = false;

void Assets::PlaySound(unsigned int index)
{
    if(!Assets::sound)return;

    if(index < ls_sounds.size())
    {
        ls_sounds[index]->setRelativeToListener(true);
        ls_sounds[index]->setPosition(0,0,0);
        ls_sounds[index]->setVolume(svolume);
        ls_sounds[index]->play();
    }
}

void Assets::PlaySound3D(unsigned int index, MXVector pos)
{
    if(!Assets::sound)return;

    // Work out a normal that can be used to modify the volume based on position.
    ls_sounds[index]->setPosition(pos.x,pos.y,pos.z);
    ls_sounds[index]->setAttenuation(50.0F);
    ls_sounds[index]->setMinDistance(0.5F);


    // Only start playing the sound again if it's stopped.

    if(index < ls_sounds.size() && ls_sounds[index]->getStatus() == sf::Sound::Stopped)
    {
        ls_sounds[index]->setVolume(svolume);
        ls_sounds[index]->play();
    }

    //ls_sounds[index]->SetPosition(0,0,0);
}

void Assets::PlayMusic(unsigned int index)
{
    if(index < 0)
    {
        if(myEmuStream)
        {
            myEmuStream->setVolume(0);
            myEmuStream->stop();
        }
        delete myEmuStream;
        myEmuStream = NULL;
        //printf("O_O\n");
        return;
    }
    if(index >= nt_music.size())return;
    if(!Assets::music)return;

    if(!Assets::myEmuStream)
    {
        if(index < 0)return;
        Assets::myEmuStream = new emuStream(nt_music[index],44100);
        Assets::myEmuStream->setVolume(mvolume);
        Assets::myEmuStream->play();
    }
    else
    {
        Assets::myEmuStream->stop();
        delete Assets::myEmuStream;
        if(index < 0)return;
        Assets::myEmuStream = new emuStream(nt_music[index],44100);
        Assets::myEmuStream->setVolume(mvolume);
        Assets::myEmuStream->play();
    }
}

void Assets::SetMusicVolume(int vol)
{
    mvolume = vol;
}
void Assets::SetSoundVolume(int vol)
{
    svolume = vol;
}

void Assets::UnlockBonusFiles()
{
    packfile.Write("Developers Notes.txt");
    packfile.Write("Design.txt");
    packfile.Write("Level Editing Readme.txt");

    system("@move \"./Developers Notes.txt\" \"./stuff/\"");
    system("@move \"./Design.txt\" \"./stuff/\"");
    system("@move \"./Level Editing Readme.txt\" \"./stuff/\"");
}

emuStream *Assets::myEmuStream = NULL;

MPK2 Assets::packfile;
