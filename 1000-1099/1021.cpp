
#include <ctime>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <iomanip>
#include <iostream>
#include <algorithm>

#include <string>
#include <vector>
#include <queue>
#include <list>
#include <map>

using namespace std;

#define OUTPUT_DEBUG1(TARGET0) \
cerr << #TARGET0 << " == " << TARGET0 << endl;
#define OUTPUT_DEBUG2(TARGET0, TARGET1) \
cerr << #TARGET0 << " == " << TARGET0 << "," << #TARGET1 << " == " << TARGET1 << endl;
#define OUTPUT_DEBUG3(TARGET0, TARGET1, TARGET2) \
cerr << #TARGET0 << " == " << TARGET0 << "," << #TARGET1 << " == " << TARGET1 << "," << #TARGET2 << " == " << TARGET2 << endl;

#define INF 0x7f7f7f7f

struct icoord
{
    int x, y;
    icoord(int _x = 0, int _y = 0) : x(_x), y(_y) {}
    bool operator< (const icoord & other) const { return x < other.x || x == other.x && y < other.y; }
    bool operator== (const icoord & other) const { return x == other.x && y == other.y; }
    icoord operator+ (const icoord & other) const { return icoord(x + other.x, y + other.y); }
    icoord operator- (const icoord & other) const { return icoord(x - other.x, y - other.y); }
    bool inbox(int xmin, int ymin, int xmax, int ymax) const { return x >= xmin && x <= xmax && y >= ymin && y <= ymax; }
};
const icoord dir[4] = { icoord(0, 1), icoord(1, 0), icoord(0, -1), icoord(-1, 0) };

struct EncodePtr
{
    bool valid;
    icoord startpos;
    icoord xdir;
    icoord ydir;
    char * targetImage;
    int xmin, ymin, xmax, ymax;
    int height, width;
    icoord curxpos;
    icoord curpos;
    vector<char> seq;
    EncodePtr() { memset(this, 0, sizeof(EncodePtr)); }
    void setHW(int _h, int _w) { height = _h; width = _w; xmin = 0; ymin = 0; xmax = height - 1; ymax = width - 1; }
    void start() { curpos = curxpos = startpos; valid = true; seq.clear(); seq.push_back(targetImage[curpos.x * width + curpos.y]); }
    bool step()
    {
        if (!valid) return false;
        icoord nx = curpos + ydir;
        if (!nx.inbox(xmin, ymin, xmax, ymax))
        {
            curxpos = curxpos + xdir, nx = curxpos;
            if (!curxpos.inbox(xmin, ymin, xmax, ymax))
                return valid = false;
        }
        curpos = nx;
        seq.push_back(targetImage[curpos.x * width + curpos.y]);
        return true;
    }
};

struct Image
{
    vector<icoord> points;
    vector<unsigned int> code;
    int ww, hh;
    bool valid;
    Image() : points(), code(), ww(0), hh(0), valid(false) {}
    void encode()
    {
        int xmin = INF, xmax = -INF, ymin = INF, ymax = -INF;
        for (int i = 0; i < points.size(); ++i)
        {
            xmin = min(xmin, points[i].x);
            xmax = max(xmax, points[i].x);
            ymin = min(ymin, points[i].y);
            ymax = max(ymax, points[i].y);
        }
        icoord minpoint(xmin, ymin);
        for (int i = 0; i < points.size(); ++i)
            points[i] = points[i] - minpoint;
        xmax -= xmin; xmin = 0;
        ymax -= ymin; ymin = 0;
        int height = xmax + 1;
        int width = ymax + 1;
        char * bitImage = new char[height * width];
        memset(bitImage, 0, height * width * sizeof(char));
        for (int i = 0; i < points.size(); ++i)
            bitImage[points[i].x * width + points[i].y] = 1;
        EncodePtr ptrs[8];
        for (int i = 0; i < 8; ++i)
        {
            ptrs[i].setHW(height, width);
            ptrs[i].targetImage = bitImage;
        }
        ptrs[0].startpos = ptrs[1].startpos = icoord(0, 0);
        ptrs[2].startpos = ptrs[3].startpos = icoord(xmax, 0);
        ptrs[4].startpos = ptrs[5].startpos = icoord(xmax, ymax);
        ptrs[6].startpos = ptrs[7].startpos = icoord(0, ymax);
        ptrs[0].xdir = ptrs[6].xdir = dir[1];
        ptrs[1].xdir = ptrs[3].xdir = dir[0];
        ptrs[2].xdir = ptrs[4].xdir = dir[3];
        ptrs[5].xdir = ptrs[7].xdir = dir[2];
        ptrs[0].ydir = ptrs[2].ydir = dir[0];
        ptrs[1].ydir = ptrs[7].ydir = dir[1];
        ptrs[4].ydir = ptrs[6].ydir = dir[2];
        ptrs[3].ydir = ptrs[5].ydir = dir[3];
        if (width < height)
            ptrs[0].valid = ptrs[2].valid = ptrs[4].valid = ptrs[6].valid = false;
        else if (width > height)
            ptrs[1].valid = ptrs[3].valid = ptrs[5].valid = ptrs[7].valid = false;
        for (int i = 0; i < 8; ++i)
            ptrs[i].start();
        int winner = -1;
        while (1)
        {
            int validnum = 0;
            for (int i = 0; i < 8; ++i)
                if (ptrs[i].valid)
                    ++validnum, winner = i;
            if (validnum == 0) break;
            char cmax = 0;
            for (int i = 0; i < 8; ++i)
                if (ptrs[i].valid && cmax < ptrs[i].seq.back())
                    cmax = ptrs[i].seq.back();
            for (int i = 0; i < 8; ++i)
                if (ptrs[i].valid && ptrs[i].seq.back() < cmax)
                    ptrs[i].valid = false;
            for (int i = 0; i < 8; ++i)
                ptrs[i].step();
        }
        ww = min(width, height), hh = max(width, height);
        //code = ptrs[winner].seq;
        int seqsize = ptrs[winner].seq.size();
        code.resize(seqsize / 32 + (seqsize % 32 > 0));
        for (int i = 0; i < code.size(); ++i) code[i] = 0;
        for (int i = 0; i < seqsize; ++i)
        {
            unsigned int idx = i / 32;
            unsigned int lsh = i % 32;
            unsigned int bit = ptrs[winner].seq[i] > 0 ? 1 : 0;
            code[idx] = code[idx] | (bit << lsh);
        }
        delete[] bitImage;
        valid = true;
    }
};

int T;
int W, H, n;
int grids[110][110];
vector<Image> images[2];
int sorted[2][10010];
template<int which>
bool imageSortCompare(int a, int b)
{
    return images[which][a].code < images[which][b].code;
}

icoord uf[110][110];
icoord unionFind(const icoord & pos)
{
    if (uf[pos.x][pos.y] == pos) return pos;
    return uf[pos.x][pos.y] = unionFind(uf[pos.x][pos.y]);
}
int uf2image[110][110];

int main()
{
    ios::sync_with_stdio(false);

    cin >> T;
    while (T--)
    {
        cin >> W >> H >> n;
        images[0].clear();
        images[1].clear();
        for (int t = 0; t < 2; ++t)
        {
            memset(grids, 0, sizeof(grids));
            for (int i = 0; i < W; ++i)
                for (int j = 0; j < H; ++j)
                    uf[i][j] = icoord(i, j);
            memset(uf2image, -1, sizeof(uf2image));
            for (int i = 0; i < n; ++i)
            {
                int x, y;
                cin >> x >> y;
                icoord cur(x, y);
                grids[x][y] = 1;
                for (int k = 0; k < 4; ++k)
                {
                    icoord to = cur + dir[k];
                    if (to.inbox(0, 0, W - 1, H - 1) && grids[to.x][to.y])
                    {
                        icoord found = unionFind(cur);
                        uf[found.x][found.y] = unionFind(to);
                    }
                }
            }
            for (int i = 0; i < W; ++i)
                for (int j = 0; j < H; ++j)
                    if (grids[i][j])
                    {
                        icoord cur(i, j);
                        icoord found = unionFind(cur);
                        if (uf2image[found.x][found.y] == -1)
                        {
                            images[t].push_back(Image());
                            uf2image[found.x][found.y] = images[t].size() - 1;
                        }
                        int imageIndex = uf2image[found.x][found.y];
                        images[t][imageIndex].points.push_back(cur);
                    }
            for (int i = 0; i < W; ++i)
                for (int j = 0; j < H; ++j)
                    if (grids[i][j])
                    {
                        icoord cur(i, j);
                        icoord found = unionFind(cur);
                        int imageIndex = uf2image[found.x][found.y];
                        if (!images[t][imageIndex].valid) images[t][imageIndex].encode();
                    }
        }
        bool noooooo = true;
        if (images[0].size() == images[1].size())
        {
            for (int i = 0; i < images[0].size(); ++i)
                sorted[0][i] = sorted[1][i] = i;
            sort(sorted[0], sorted[0] + images[0].size(), imageSortCompare<0>);
            sort(sorted[1], sorted[1] + images[1].size(), imageSortCompare<1>);
            noooooo = false;
            for (int i = 0; i < images[0].size(); ++i)
                if (images[0][sorted[0][i]].ww != images[1][sorted[1][i]].ww
                    || images[0][sorted[0][i]].hh != images[1][sorted[1][i]].hh
                    || images[0][sorted[0][i]].code != images[1][sorted[1][i]].code)
                {
                    noooooo = true;
                    break;
                }
        }
        if (noooooo)
            cout << "NO" << endl;
        else
            cout << "YES" << endl;
    }

    return 0;
}