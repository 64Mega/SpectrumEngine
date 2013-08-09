// MXLMap implementation
// Partly ported from my former C# version of the loader

// WARNING: This class is officially FUBAR and Deprecated.
// --

#include "spectrum/spectrum.h"

using namespace std;

namespace MIO
{
vector<string> split(string, char);
};

#include "spectrum/engine/mxlmap.h"
//#include "MIO.h"

Rect MXLMap::ClipPlane = Rect(0,0,640,480);

MXLMap::MXLMap():Object()
{
    width = 0;
    height = 0;
    layers = 0;
    gap = 0;

    isLoaded = false;
    for(int i = 0; i < 16; i++)mapdata[i] = NULL;

    myTileset = NULL;

    mapLabel = "Unknown";
    tsetLabel = "Unknown";

    numtiles = 0;
}

MXLMap::~MXLMap()
{
    for(int i = 0; i < layers; i++)
    {
        if(mapdata[i])delete mapdata[i];
    }

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
    if(mapdata)delete[] mapdata;
    if(myTileset)delete myTileset;

    width = 0;
    height = 0;

    isLoaded = false;

    bool grv4 = false;

    for(int i = 0; i < 16; i++)mapdata[i] = NULL;

    myTileset = NULL;

    mapLabel = "Unknown";
    tsetLabel = "Unknown";

    numtiles = 0;

    FILE* fp = fopen(filename.c_str(),"rb");
    if(!fp)
    {
        throw Exception(this, "Unable to load '" + filename + "'...");
    }

    mapName = filename;

    string id = ReadString(fp,3);
    if(id != "GR2" && id != "GR4")
    {
        throw Exception(this, "Valid GRX header missing from '" + filename + "'!");
    }

    if(id == "GR4")
    {
        grv4 = true;
    }

    myTileset = new Tileset("terrain");
    myTileset->Setup(16,16);

    // Load other information
    int idlength = ReadWord(fp);
    string label = ReadString(fp,idlength);

    int m_width = ReadWord(fp);
    int m_height = ReadWord(fp);
    int m_layers = ReadWord(fp);
    int m_gap = ReadWord(fp);

    numtiles = m_width*m_height*m_layers;
    width = m_width;
    height = m_height;
    layers = m_layers;
    gap = m_gap;

    cout << "Map Dimensions: " << m_width << "x" << m_height << endl;
    cout << "Map layer info: " << m_layers << " @ " << m_gap << endl;

    // Read trigger zones first
    int trigcount = 0;
    for(int i = 0; i < 256; i++)
    {
        int li, lx, ly, ll, lw, lh;
        li = ReadWord(fp);
        lx = ReadWord(fp);
        ly = ReadWord(fp);
        ll = ReadWord(fp);
        lw = ReadWord(fp);
        lh = ReadWord(fp);

        triggers[i].id = li;
        triggers[i].x = lx;
        triggers[i].y = ly;
        triggers[i].layer = ll;
        triggers[i].width = lw;
        triggers[i].height = lh;

        if(li > 0)trigcount++;
    }

    cout << "TRIGGERS READ: " << trigcount << endl;

    for(int ili = 0; ili < layers; ili++)
    {
        // Initialize arrays
        mapdata[ili] = new vector<STile>(numtiles);

        // Read in the remaining data
        for(int i = 0; i < (numtiles/layers); i++)
        {
            int t = ReadWord(fp);
            int l = ReadWord(fp);
            int e = ReadWord(fp);
            int o = ReadWord(fp);
            int hi = ReadWord(fp);
            int lo = ReadWord(fp);
            int col = ReadWord(fp);

            if(col > 0)
            {
                cout << "COLLISION TYPE: " << col << endl;
            }

            int d1 = ReadWord(fp);
            int di = ReadWord(fp);
            int dt = ReadWord(fp);

            col = d1; // Stupid patch for strange byte ordering?

            // There. Now store it.

            (*mapdata[ili])[i].t = t;
            (*mapdata[ili])[i].l = l;

            (*mapdata[ili])[i].e = e;
            (*mapdata[ili])[i].o = o;
            (*mapdata[ili])[i].hi = hi;
            (*mapdata[ili])[i].lo = lo;
            (*mapdata[ili])[i].col = col;
            (*mapdata[ili])[i].d1 = d1;
            (*mapdata[ili])[i].di = di;
            (*mapdata[ili])[i].dt = dt;
        }
    }
    // Flip the entire array on it's Z axis, so the map is correctly aligned.

    //STile* buffer = new STile[numtiles];
    //float tlight_r[numtiles], tlight_b[numtiles], tlight_g[numtiles];

    // int ky = 0;
    /*
    for(int iy = height-1; iy > 0; iy--)
    {

    for(int ix = 0; ix < width; ix++)
    {
        buffer[iy * width + ix].t = mapdata[ky*width+ix].t;
        buffer[iy * width + ix].l = mapdata[ky*width+ix].l;
        buffer[iy * width + ix].e = mapdata[ky*width+ix].e;
        buffer[iy * width + ix].o = mapdata[ky*width+ix].o;
        buffer[iy * width + ix].hi = mapdata[ky*width+ix].hi;
        buffer[iy * width + ix].lo = mapdata[ky*width+ix].lo;
        buffer[iy * width + ix].col = mapdata[ky*width+ix].col;

        tlight_r[iy * width + ix] = tlight_g[iy * width + ix] = tlight_b[iy * width + ix] = light_g[ky * width + ix];

    }
    ky++;
    }

    // Recopy.
    for(int i = 0; i < numtiles; i++)
    {
        mapdata[i] = buffer[i];
        light_r[i] = light_g[i] = light_b[i] = tlight_r[i];
    }*/

    fclose(fp);
}

void MXLMap::LoadEffectors()
{
    // REmove
}

void MXLMap::WriteEffectors()
{
    // Remove
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

bool MXLMap::place_free(int x, int y, int z)
{
    int tx = floor(x/myTileset->GetWidth());
    int ty = floor(y/myTileset->GetHeight());
    int tt = (ty * width) + tx;
    if(tt > numtiles-1 || tt < 0)return true;
    return !((*mapdata[z])[tt].col == 1 || (*mapdata[z])[tt].col == 2 || (*mapdata[z])[tt].col == 6 || (*mapdata[z])[tt].col == 17);
}

bool MXLMap::place_free_ni(int x, int y, int z)
{
    int tx = round((float)x/myTileset->GetWidth());
    int ty = round((float)y/myTileset->GetHeight());
    int tt = (ty * width) + tx;
    if(tt > numtiles-1 || tt < 0)return true;
    return !((*mapdata[z])[tt].col == 1 || (*mapdata[z])[tt].col == 6 || ((*mapdata[z])[tt].col == 2 && ((*mapdata[z])[tt].hi == 5 || (*mapdata[z])[tt].hi == 6)));
}

void MXLMap::Draw()
{
    // Currently unused
}

void MXLMap::UpdateEffectors(int px, int py, bool use)
{
    // Remove
}
