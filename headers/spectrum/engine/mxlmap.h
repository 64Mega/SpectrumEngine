// Loads a MXL format map (Compiled MXT)

// WARNING: This class is officially FUBAR and Deprecated.
// --

#ifndef MXLMAP_H
#define MXLMAP_H

#include "tileset.h"

#define MAX_LAYERS 16

typedef struct _TSTriggerZone
{
    int id;
    int x;
    int y;
    int layer;
    int width;
    int height;

    _TSTriggerZone()
    {
        id = 0;
        x = 0;
        y = 0;
        layer = -1;
        width = 0;
        height = 0;
    }
}STrigZone;

typedef struct _TSTile
{
    int t, l, e, o, hi, lo, col;
    int d1; // Unknown
    int di; // Decal index
    int dt; // Decal type

    _TSTile()
    {
        t = -1;
        l = 5;
        e = 32;
        o = -1;
        hi = 0;
        lo = 0;
        col = 0;

        d1 = 0;
        di = 0;
        dt = -1;
    }
}STile;


class MXLMap: public Object
{
    public:
        MXLMap();
        ~MXLMap();

        int GetWidth(){return width;}
        int GetHeight(){return height;}
        int GetLayers(){return layers;}
        int GetGap(){return gap;}

        int GetCode(int x, int y, int z){ return (*mapdata[z])[y * width + x].lo; }

        void Load(string filename);
        void Draw();

        static Rect ClipPlane;
        bool place_free(int x, int y, int z); // Go figure. Takes closed doors into account.
        bool place_free_ni(int x, int y, int z); // Doesn't take Inviswalls into account.

        vector<STile>* mapdata[16];
        STrigZone triggers[256];

        vector<Object*> effectors;

        void UpdateEffectors(int px, int py, bool use);

        void WriteEffectors(); // Writes to <levelname>.WCH
        void LoadEffectors();

        bool isLoaded;

    protected:
        int width;
        int height;
        int layers;
        int gap;

        string mapName;

        int numtiles;

        string mapLabel;
        string tsetLabel;

        Tileset *myTileset;




    private:
        int ReadWord(FILE* fp);
        string ReadString(FILE* fp, int len);
        unsigned char ReadByte(FILE* fp);
        void clamp(int& i);
};

#endif
