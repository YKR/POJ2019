
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
    icoord() : x(0), y(0) {}
    icoord(int _x, int _y) : x(_x), y(_y) {}
    bool inbox(int xmin, int ymin, int xmax, int ymax) const { return x >= xmin && y >= ymin && x <= xmax && y <= ymax; }
    icoord operator+ (const icoord & other) const { return icoord(x + other.x, y + other.y); }
    bool operator== (const icoord & other) const { return x == other.x && y == other.y; }
};
#define REV_DIR(DIR) ((DIR) ^ 2)
const icoord dir[4] = { icoord(0, 1), icoord(1, 0), icoord(0, -1), icoord(-1, 0) };
int ch2dir[256];

int T;
int w, h, m;
string path;
int walls[110][110];
icoord endp;

int mindis[110][110][2];
bool inpath[110][110];

bool inque[110][110];
void bfs()
{
    deque<icoord> que;
    memset(mindis, 0x7F, sizeof(mindis));
    memset(inque, 0, sizeof(inque));
    mindis[0][0][0] = 0;
    mindis[0][0][1] = 0;
    que.push_back(icoord(0, 0));
    inque[0][0] = true;
    while (!que.empty())
    {
        icoord cur = que.front(); que.pop_front();
        inque[cur.x][cur.y] = false;
        for (int k = 0; k < 4; ++k)
        {
            icoord to = cur + dir[k];
            if (!to.inbox(0, 0, w - 1, h - 1)) continue;
            if (!(walls[cur.x][cur.y] & (1 << k)))
            {
                if (mindis[to.x][to.y][0] > mindis[cur.x][cur.y][0] + 1)
                {
                    mindis[to.x][to.y][0] = mindis[cur.x][cur.y][0] + 1;
                    mindis[to.x][to.y][1] = 1 << REV_DIR(k);
                    if (!inque[to.x][to.y]) que.push_back(to), inque[to.x][to.y] = true;
                }
                else if (mindis[to.x][to.y][0] == mindis[cur.x][cur.y][0] + 1)
                {
                    mindis[to.x][to.y][1] |= 1 << REV_DIR(k);
                    if (!inque[to.x][to.y]) que.push_back(to), inque[to.x][to.y] = true;
                }
            }
        }
    }
}

bool checkMindis()
{
    bool ret = false;
    if (mindis[endp.x][endp.y][0] == path.size())
    {
        memset(inpath, 0, sizeof(inpath));
        icoord cur(endp.x, endp.y);
        ret = true;
        for (int i = path.size() - 1; i >= 0; --i)
            if (mindis[cur.x][cur.y][1] != (1 << REV_DIR(ch2dir[path[i]])))
            {
                ret = false;
                break;
            }
            else
            {
                inpath[cur.x][cur.y] = true;
                cur = cur + dir[REV_DIR(ch2dir[path[i]])];
            }
        inpath[0][0] = true;
    }
    return ret;
}

int main()
{
    ios::sync_with_stdio(false);

    ch2dir['R'] = 1; ch2dir['U'] = 0; ch2dir['L'] = 3; ch2dir['D'] = 2;
    
    cin >> T;
    while (T--)
    {
        cin >> w >> h >> path >> m;
        endp = icoord();
        for (int i = 0; i < path.size(); ++i)
            endp = endp + dir[ch2dir[path[i]]];
        memset(walls, 0, sizeof(walls));
        for (int i = 0; i < m; ++i)
        {
            icoord p0, p1;
            cin >> p0.x >> p0.y >> p1.x >> p1.y;
            for (int k = 0; k < 4; ++k)
                if (p1 == p0 + dir[k])
                {
                    walls[p0.x][p0.y] |= 1 << k;
                    walls[p1.x][p1.y] |= 1 << REV_DIR(k);
                    break;
                }
        }
        bfs();
        bool correct = checkMindis();
        if (correct)
        {
            for (int x = 0; x < w && correct; ++x)
                for (int y = 0; y < h && correct; ++y)
                {
                    icoord cur(x, y);
                    if (mindis[x][y][0] == INF)
                    {
                        correct = false;
                        break;
                    }
                    for (int k = 0; k < 2; ++k)
                    {
                        icoord to = cur + dir[k];
                        if (!to.inbox(0, 0, w - 1, h - 1)) continue;
                        if (walls[x][y] & (1 << k))
                        {
                            walls[x][y] ^= (1 << k);
                            walls[to.x][to.y] ^= (1 << REV_DIR(k));
                            bfs();
                            if (checkMindis())
                            {
                                correct = false;
                                break;
                            }
                            walls[x][y] ^= (1 << k);
                            walls[to.x][to.y] ^= (1 << REV_DIR(k));
                        }
                    }
                }
        }
        if (correct)
            cout << "CORRECT" << endl;
        else
            cout << "INCORRECT" << endl;
    }

    return 0;
}