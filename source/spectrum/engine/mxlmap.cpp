// MXLMap implementation
// Partly ported from my former C# version of the loader

#include "spectrum/spectrum.h"

using namespace std;

namespace MIO
{
vector<string> split(string, char);
};

#include "spectrum/engine/mxlmap.h"
//#include "MIO.h"

Rect MXLMap::ClipPlane = Rect(0,0,640,480);

MXLMap::MXLMap():()
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

    light_r = NULL;
    light_g = NULL;
    light_b = NULL;
}

MXLMap::~MXLMap()
{
    for(int i = 0; i < layers; i++)
    {
        if(mapdata[i])delete[] mapdata[i];
    }
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
    if(mapdata)delete[] mapdata;
    if(light_r)delete[] light_r;
    if(light_g)delete[] light_g;
    if(light_b)delete[] light_b;

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

    light_r = NULL;
    light_g = NULL;
    light_b = NULL;

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
    //int m_numtiles = ReadWord(fp);

    numtiles = m_width*m_height*m_layers; //(m_numtiles/7);
    width = m_width;
    height = m_height;
    layers = m_layers;
    gap = m_gap;

    cout << "Map Dimensions: " << m_width << "x" << m_height << endl;
    cout << "Map layer info: " << m_layers << " @ " << m_gap << endl;

    // Read trigger zones first
    int trigcount = 0;
    //int numtrigs = ReadByte(fp);
    //cout << "TRIGGERS: " << numtrigs << endl;
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

        //cout << "TRIGGER " << i << ": ID = " << triggers[i].id << endl;

        if(li > 0)trigcount++;
    }

    cout << "TRIGGERS READ: " << trigcount << endl;

    for(int ili = 0; ili < layers; ili++)
    {
        // Initialize arrays
        mapdata[ili] = new STile[numtiles];
        STile* buffer = new STile[numtiles];

        light_r = new float[numtiles];
        light_g = new float[numtiles];
        light_b = new float[numtiles];

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

            mapdata[ili][i].t = t;
            mapdata[ili][i].l = l;

            mapdata[ili][i].e = e;
            mapdata[ili][i].o = o;
            mapdata[ili][i].hi = hi;
            mapdata[ili][i].lo = lo;
            mapdata[ili][i].col = col;
            mapdata[ili][i].d1 = d1;
            mapdata[ili][i].di = di;
            mapdata[ili][i].dt = dt;

            light_r[i] = light_g[i] = light_b[i] = (mapdata[ili][i].l/10.0F);


            //if(b > 0)
            //{
            //    //printf("DD: x(%d) y(%d) tm(%d) b(%d) c(%d)\n",x,y,tm,b,c);
            //}
        }
    }
    // Flip the entire array on it's Z axis, so the map is correctly aligned.

    //STile* buffer = new STile[numtiles];
    float tlight_r[numtiles], tlight_b[numtiles], tlight_g[numtiles];

    int ky = 0;
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
    //isLoaded = true;

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
        cout <<"Can't find " << fname << " for this level. Skipping." << endl;
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
            cout <<"Player Start effector" << endl;
            i += 5;
            break;
        case 2: // Trigger Sector, (x1, y1, x2, y2, tag, target, act)
            cout <<"Trigger Sector" << endl;
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

bool MXLMap::place_free(int x, int y, int z)
{
    int tx = floor(x/myTileset->GetWidth());
    int ty = floor(y/myTileset->GetHeight());
    int tt = (ty * width) + tx;
    if(tt > numtiles-1 || tt < 0)return true;
    return !(mapdata[z][tt].col == 1 || mapdata[z][tt].col == 2 || mapdata[z][tt].col == 6 || mapdata[z][tt].col == 17);
}

bool MXLMap::place_free_ni(int x, int y, int z)
{
    int tx = round((float)x/myTileset->GetWidth());
    int ty = round((float)y/myTileset->GetHeight());
    int tt = (ty * width) + tx;
    if(tt > numtiles-1 || tt < 0)return true;
    return !(mapdata[z][tt].col == 1 || mapdata[z][tt].col == 6 || (mapdata[z][tt].col == 2 && (mapdata[z][tt].hi == 5 || mapdata[z][tt].hi == 6)));
}

void MXLMap::Draw()
{
//    int w = 16;
//    int h = 16;
//
//    Rect rr = MXLMap::ClipPlane;
//
//    int tvx = (rr.x / w)-1;
//    int tvy = (rr.y / h)-1;
//    int tvw = (rr.w / w)+2;
//    int tvh = (rr.h / h)+2;
//
//    for(int iy = tvy; iy < tvy + tvh; iy++)
//        for(int ix = tvx; ix < tvx + tvw; ix++)
//        {
//            if(ix > width || ix < 0 || iy > height || iy < 0)continue;
//            int i = iy * width + ix;
//            if (i > numtiles - 1)
//            {
//                i = (i % numtiles);
//                if (i > numtiles - 1) i = numtiles - 1;
//            }
//            if (i < 0)
//            {
//                i = -i;
//                i = numtiles - (i % numtiles);
//            }
//
//            int x = ix * w;
//            int y = iy * h;
//            //x = x+(-rr.x);
//            //y = y+(-rr.y);
//
//            if(i > numtiles - 1) i = numtiles - 1;
//            //if(layer_back[i].t >= 0)myTileset->Draw(layer_back[i].t, x, y);
//            //if(layer_mid[i].t >= 0)myTileset->Draw(layer_mid[i].t, x, y);
//            //if(layer_front[i].t >= 0)myTileset->Draw(layer_front[i].t, x, y);
//
//        }
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
