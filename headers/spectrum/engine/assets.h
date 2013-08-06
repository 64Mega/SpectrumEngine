// Defines the Assets class, which is designed to be used in a static context
// It prevents the common mistake of re-loading resources each time an
// object is created

#ifndef ASSETS_H
#define ASSETS_H

#include "./../../emustream.h"
#include <mxmath.h>

#include "./../../mpk2.h"

/**
    Loads and manages various assets used by the game.
    Currently handles:
        sf::Image
        GameObject* (Instantiation templates, for Mapcode use)
    @author Mega
*/

class Object;
class GameObject;
class Assets
{
    public:
        Assets();
        ~Assets();

        static void Initialize();
        static void Destroy();

        static void LoadAssets();

        static sf::Texture* GetImage(std::string name);
        static sf::Texture* GetImageByIndex(int index);

        static sf::Texture* GetDecal(std::string name);
        static sf::Texture* GetDecalByIndex(int index);

        static void AddImage(std::string filename, std::string ident);
        static void AddImageX(std::string filename, std::string ident);

        static void AddDecal(std::string filename, std::string ident);
        static void AddDecalX(std::string filename, std::string ident);

        static int AddObject(GameObject* obj);
        static GameObject* Instantiate(int index, int x, int y);
        static std::string GetLevel(int idex);
        static void AddLevel(std::string lname);

        static void AddMusic(std::string mname);
        static void AddSound(std::string sname);

        static void PlaySound(int index);
        static void PlaySound3D(int index, MXVector pos); // Plays a faked positional sound.
        static void PlayMusic(int index);

        static void SetMusicVolume(int vol);
        static void SetSoundVolume(int vol);

        static void LoadList(std::string fname);
        static void LoadPack(std::string fname);

        static void UnlockBonusFiles();

        static int TileWidth;
        static int TileHeight;

        static int svolume, mvolume;
        static bool music, sound;

    private:
        static vector<sf::Texture*> ls_images;
        static vector<sf::Texture*> ls_decals;
        static vector<GameObject*> ls_objects;

        static vector<sf::SoundBuffer*> ls_sbuffers;
        static vector<sf::Sound*> ls_sounds;

        // Name tables
        static vector<std::string> nt_images;
        static vector<std::string> nt_decals;
        static vector<std::string> nt_levels; // Level paths. Used for in-game switching.
        static vector<std::string> nt_music;

        static emuStream *myEmuStream;

        static MPK2 packfile;
        // No nametable for objects. They use indices instead
};

#endif
