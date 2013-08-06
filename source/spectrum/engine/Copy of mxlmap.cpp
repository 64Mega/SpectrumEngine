// MXLMap implementation
// Partly ported from my former C# version of the loader

#include "../spectrum.h"

using namespace std;

namespace MIO
{
    vector<string> split(string, char);
};

#include "mxlmap.h"
//#include "MIO.h"

Rect MXLMap::ClipPlane = Rect(0,0,640,480);

MXLMap::MXLMap():()
{
    width = 0;
    height = 0;

    boundmap = NULL;
    codemap = NULL;

    isLoaded = false;
    layer_back = NULL;
    layer_mid = NULL;
    layer_front = NULL;

    myTileset = NULL;

    mapLabel = "Unknown";
    tsetLabel = "Unknown";

    numtiles = 0;

    light_r = NULL;
    light_g = NULL;
    light_b = NULL;
}

MXLMap::~MXLMap()
{
    if(boundmap)delete[] boundmap;
    if(codemap)delete[] codemap;
    if(layer_back)delete[] layer_back;
    if(layer_mid)delete[] layer_mid;
    if(layer_front)delete[] layer_front;
    if(light_r)delete[] light_r;
    if(light_g)delete[] light_g;
    if(light_b)delete[] light_b;

    if(myTileset)delete myTileset;
}


string MXLMap::ReadString(FILE* fp, int len)
{
    char *bt = (char*)malloc(sizeof(char)*(len+1));
    bt[len] = '\0';
    string tt = "";
    fread(bt, sizeof(char), len, fp);
    tt = string(bt);
    free(bt);
    return tt;
}

int MXLMap::ReadWord(FILE* fp)
{
    int c;
    fread(&c,sizeof(int),1,fp);
    return c;
}

unsigned char MXLMap::ReadByte(FILE* fp)
{
    unsigned char c;
    fread(&c, sizeof(unsigned char), 1, fp);
    return c;
}

typedef unsigned char byte;

void MXLMap::Load(string filename)
{
    FILE* fp = fopen(filename.c_str(),"rb");
    if(!fp)
    {
        throw Exception(this, "Unable to load '" + filename + "'...");
    }

    mapName = filename;

    string id = ReadString(fp,3);
    if(id != "MXL")
    {
        throw Exception(this, "MXL header missing from '" + filename + "'!");
    }

    int idlength = ReadWord(fp);
    string label = ReadString(fp,idlength);
    int tslength = ReadWord(fp);
    string ts    = ReadString(fp,tslength);

    // Attempt to load tileset
    cout << "Map '" << label << "' using tileset '" << ts << "'." << endl;

    myTileset = new Tileset(ts);
    myTileset->Setup(16,16);
    //myTileset->Setup(16,16);

    // Load other information

    int m_width = ReadWord(fp);
    int m_height = ReadWord(fp);
    int m_numtiles = ReadWord(fp);

    numtiles = m_numtiles/7;
    width = m_width;
    height = m_height;

    cout << "Map Dimensions: " << m_width << "x" << m_height << endl;

    // Initialize arrays
    layer_back = new STile[numtiles];
    layer_mid = new STile[numtiles];
    layer_front = new STile[numtiles];
    boundmap = new unsigned char[numtiles];
    codemap = new unsigned char[numtiles];

    light_r = new float[numtiles];
    light_g = new float[numtiles];
    light_b = new float[numtiles];

    // Read in the remaining data
    for(int i = 0; i < numtiles; i++)
    {
        int x = ReadWord(fp);
        int y = ReadWord(fp);
        int tb = ReadWord(fp);
        int tm = ReadWord(fp);
        int tf = ReadWord(fp);

        unsigned char b = ReadByte(fp);
        unsigned char c = ReadByte(fp);

        // There. Now store it.

        layer_back[i].x = layer_mid[i].x = layer_front[i].x = x;
        layer_back[i].y = layer_mid[i].y = layer_front[i].y = y;
        layer_back[i].t = tb;
        layer_mid[i].t = tm;
        layer_front[i].t = tf;
        boundmap[i] = b;
        codemap[i] = c;

        light_r[i] = light_g[i] = light_b[i] = layer_back[i].t == 11 || layer_front[i].t == 11 ? 1.0F : 0.5F;

        if(b > 0)
        {
            //printf("DD: x(%d) y(%d) tm(%d) b(%d) c(%d)\n",x,y,tm,b,c);
        }
    }

    fclose(fp);

    // Load and process sector effectors
    LoadEffectors();
}

void MXLMap::LoadEffectors()
{
    return;
    // Load Sector Effectors and process them.
    effectors.clear();

    // Try to open file <levelname>.wch
    vector<string> sname = MIO::split(mapName,'.');
    string fname = string("./") + sname[0] + ".WCH";

    FILE* fin = fopen(fname.c_str(),"rb");
    if(!fin)
    {
        cout << "Can't find " << fname << " for this level. Skipping." << endl;
        return;
    }

    _finddata_t fdd;
    _findfirst(fname.c_str(),&fdd);

    long size = fdd.size;
    char* buffer = new char[size];

    fread(buffer, sizeof(char), size, fin);

    // Transfer to a string
    string sBuffer = string(buffer);

    for(int i = 0; i < sBuffer.size(); i++)
    {
        switch(sBuffer[i])
        {
            case 1: // Player Start, 4 params (x, y, rotation, tag)
                cout << "Player Start effector" << endl;
                i += 5;
            break;
            case 2: // Trigger Sector, (x1, y1, x2, y2, tag, target, act)
                cout << "Trigger Sector" << endl;
                i += 8;
            break;
        }
    }

    fclose(fin);
    delete[] buffer;
}

void MXLMap::WriteEffectors()
{
    return;
    // Writes to <levelname>.WCH
    string fn = mapName;
    fn += ".WCH";
    FILE* fout = fopen(fn.c_str(),"wb");
    if(!fout)
    {
        throw Exception(new Object, "[FATAL]: Couldn't open '" + fn + "' for writing!");
    }
    for(int i = 0; i < effectors.size(); i++)
    {
        SectorEffect* SE = (SectorEffect*)(effectors[i]);
        string ss = SE->ToString();
        fwrite(ss.c_str(),sizeof(char),ss.size(),fout);
    }

    fclose(fout);
}

void MXLMap::clamp(int& i)
{
    if(i > numtiles-1)
    {
        i = (i % numtiles);
        if(i > numtiles - 1) i = numtiles - 1;
    }
    if(i < 0)
    {
        i = -i;
        i = numtiles - (i % numtiles);
    }
    if(i > numtiles - 1)i = numtiles - 1;
}

bool MXLMap::place_free(int x, int y)
{
    int tx = floor(x/myTileset->GetWidth());
    int ty = floor(y/myTileset->GetHeight());
    int tt = (ty * width) + tx;
    if(tt > numtiles-1 || tt < 0)return true;
    return !(boundmap[tt] > 0);
}

void MXLMap::Draw()
{
    int w = 16;
    int h = 16;

    Rect rr = MXLMap::ClipPlane;

    int tvx = (rr.x / w)-1;
    int tvy = (rr.y / h)-1;
    int tvw = (rr.w / w)+2;
    int tvh = (rr.h / h)+2;

    for(int iy = tvy; iy < tvy + tvh; iy++)
        for(int ix = tvx; ix < tvx + tvw; ix++)
        {
            if(ix > width || ix < 0 || iy > height || iy < 0)continue;
            int i = iy * width + ix;
            if (i > numtiles - 1)
            {
                i = (i % numtiles);
                if (i > numtiles - 1) i = numtiles - 1;
            }
            if (i < 0)
            {
                i = -i;
                i = numtiles - (i % numtiles);
            }

            int x = ix * w;
            int y = iy * h;
            //x = x+(-rr.x);
            //y = y+(-rr.y);

            if(i > numtiles - 1) i = numtiles - 1;
            if(layer_back[i].t >= 0)myTileset->Draw(layer_back[i].t, x, y);
            if(layer_mid[i].t >= 0)myTileset->Draw(layer_mid[i].t, x, y);
            if(layer_front[i].t >= 0)myTileset->Draw(layer_front[i].t, x, y);

        }
}

void MXLMap::UpdateEffectors(int px, int py, bool use)
{
    for(int i = 0; i < effectors.size(); i++)
    {
        SectorEffect* SE = (SectorEffect*)(effectors[i]);
            SE->Update(px,py,use,this);
        SE = NULL;
    }
}
