
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

#define REV_DIR(TARGET) ((TARGET) ^ 1)

struct Coord4D
{
    int ax[4];
    Coord4D() { memset(this, 0, sizeof(Coord4D)); }
    Coord4D(const Coord4D & other) { memcpy(ax, other.ax, 4 * sizeof(int)); }
    Coord4D(int _x, int _y, int _z, int _w) { ax[0] = _x; ax[1] = _y; ax[2] = _z; ax[3] = _w;}
    int & operator[] (int index) { return ax[index]; }
    const int & operator[] (int index) const { return ax[index]; }
};

int T;
int n;
int id[110];
int links[110][8];
map<int, int> idmap;
int boundingbox[8];
bool visited[110];
bool inconsistent;

void flood(int cur, const Coord4D & coord)
{
    visited[cur] = true;
    for (int i = 0; i < 4; ++i)
    {
        boundingbox[i << 1] = max(boundingbox[i << 1], coord[i]);
        boundingbox[REV_DIR(i << 1)] = min(boundingbox[REV_DIR(i << 1)], coord[i]);
    }
    for (int i = 0; i < 8; ++i)
    {
        if (links[cur][i] == 0) continue;
        int target = idmap[links[cur][i]];
        if (visited[target]) continue;
        if (links[target][REV_DIR(i)] != id[cur])
        {
            inconsistent = true;
            return;
        }
        Coord4D nextCoord = coord;
        nextCoord[i >> 1] += (i & 1) ? -1 : 1;
        flood(target, nextCoord);
        if (inconsistent) return;
    }
}

int main()
{
    ios::sync_with_stdio(false);

    cin >> T;
    while (T--)
    {
        idmap.clear();
        cin >> n;
        for (int i = 0; i < n; ++i)
        {
            cin >> id[i];
            idmap[id[i]] = i;
            for (int j = 0; j < 8; ++j)
                cin >> links[i][j];
        }
        inconsistent = false;
        memset(visited, 0, sizeof(visited));
        memset(boundingbox, 0, sizeof(boundingbox));
        bool flooded = false;
        for (int i = 0; i < n && !inconsistent; ++i)
        {
            if (!visited[i])
            {
                if (flooded)
                    inconsistent = true;
                else
                    flood(i, Coord4D()), flooded = true;
            }
        }
        if (inconsistent)
            cout << "Inconsistent" << endl;
        else
        {
            int ans = 1;
            for (int i = 0; i < 4; ++i)
                ans *= std::abs(boundingbox[i << 1] - boundingbox[REV_DIR(i << 1)]) + 1;
            cout << ans << endl;
        }
    }

    return 0;
}