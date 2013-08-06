// Loads a MXL format map (Compiled MXT)

#ifndef MXLMAP_H
#define MXLMAP_H

#include "tileset.h"
#include "../../sectoreffect.h"

typedef struct _TSTile
{
    int x, y, t;
    _TSTile()
    {
        x = y = 0;
        t = -1;
    }
}STile;

class MXLMap: public Object
{
    public:
        MXLMap();
        ~MXLMap();

        int GetWidth(){return width;}
        int GetHeight(){return height;};

        int GetCode(int x, int y){ return codemap[y * width + x]; }

        void Load(string filename);
        void Draw();

        static Rect ClipPlane;
        bool place_free(int x, int y); // Go figure

        STile* layer_back;
        STile* layer_mid;
        STile* layer_front;

        unsigned char *boundmap;
        unsigned char *codemap;
        float *light_r, *light_g, *light_b;

        vector<Object*> effectors;

        void UpdateEffectors(int px, int py, bool use);

        void WriteEffectors(); // Writes to <levelname>.WCH
        void LoadEffectors();

    protected:
        int width;
        int height;

        string mapName;

        int numtiles;

        string mapLabel;
        string tsetLabel;

        Tileset *myTileset;

        bool isLoaded;


    private:
        int ReadWord(FILE* fp);
        string ReadString(FILE* fp, int len);
        unsigned char ReadByte(FILE* fp);
        void clamp(int& i);
};

#endif
