// Loads a MXL format map (Compiled MXT)

#ifndef MXLMAP_H
#define MXLMAP_H

#include "tileset.h"
#include "../../sectoreffect.h"

typedef struct _TSTile
{
    int t, l, e, o, hi, lo, col;

    _TSTile()
    {
        t = -1;
        l = 5;
        e = 32;
        o = -1;
        hi = 0;
        lo = 0;
        col = 0;
    }
}STile;


class MXLMap: public Object
{
    public:
        MXLMap();
        ~MXLMap();

        int GetWidth(){return width;}
        int GetHeight(){return height;};

        int GetCode(int x, int y){ return mapdata[y * width + x].lo; }

        void Load(string filename);
        void Draw();

        static Rect ClipPlane;
        bool place_free(int x, int y); // Go figure. Takes closed doors into account.
        bool place_free_ni(int x, int y); // Doesn't take Inviswalls into account.

        STile* mapdata;

        float *light_r, *light_g, *light_b;

        vector<Object*> effectors;

        void UpdateEffectors(int px, int py, bool use);

        void WriteEffectors(); // Writes to <levelname>.WCH
        void LoadEffectors();

        bool isLoaded;

    protected:
        int width;
        int height;

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
