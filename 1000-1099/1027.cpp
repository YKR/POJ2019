
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

const int dir[4][2] = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} };

int T;
int visited[20][20];
char grids[20][20];

void OUTPUT_GRIDS()
{
    for (int i = 0; i < 10; ++i)
    {
        for (int j = 0; j < 15; ++j)
        {
            cout << grids[i][j];
        }
        cout << endl;
    }
}

int MarkCluster(int x, int y, int mark)
{
    int ret = 1;
    visited[x][y] = mark;
    for (int k = 0; k < 4; ++k)
    {
        int tox = x + dir[k][0];
        int toy = y + dir[k][1];
        if (tox >= 0 && tox < 10 && toy >= 0 && toy < 15
            && !visited[tox][toy] && grids[tox][toy] == grids[x][y])
            ret += MarkCluster(tox, toy, mark);
    }
    return ret;
}

int main()
{
    ios::sync_with_stdio(false);

    int gameCount = 0;
    cin >> T;
    while (T--)
    {
        ++gameCount;
        for (int i = 0; i < 10; ++i)
            cin >> grids[9 - i];
        cout << "Game " << gameCount
            << ":" << endl << endl;
        int points = 0;
        int moveCount = 0;
        while (1)
        {
            memset(visited, 0, sizeof(visited));
            int visitedMark = 0;
            int maxClusterSize = 0;
            int maxClusterMark = 0;
            int mcX, mcY;
            for (int j = 0; j < 15; ++j)
                for (int i = 0; i < 10; ++i)
                {
                    int clusterSize = 0;
                    if (!visited[i][j] && grids[i][j] != ' '
                        && maxClusterSize < (clusterSize = MarkCluster(i, j, ++visitedMark)))
                    {
                        maxClusterSize = clusterSize;
                        maxClusterMark = visitedMark;
                        mcX = i, mcY = j;
                    }
                }
            if (maxClusterSize < 2) break;
            // remove cluster
            ++moveCount;
            cout << "Move " << moveCount
                << " at (" << mcX + 1 << "," << mcY + 1
                << "): removed " << maxClusterSize
                << " balls of color " << grids[mcX][mcY]
                << ", got " << (maxClusterSize - 2) * (maxClusterSize - 2)
                << " points." << endl;
            points += (maxClusterSize - 2) * (maxClusterSize - 2);
            for (int j = 0; j < 15; ++j)
                for (int i = 0; i < 10; ++i)
                {
                    if (visited[i][j] == maxClusterMark)
                        grids[i][j] = ' ';
                }
            // fall down
            for (int j = 0; j < 15; ++j)
            {
                int offset = 0;
                for (int i = 0; i + offset < 10; ++i)
                {
                    while (i + offset < 10 && grids[i + offset][j] == ' ') ++offset;
                    if (i + offset < 10) grids[i][j] = grids[i + offset][j];
                }
                for (int i = 10 - offset; i < 10; ++i)
                    grids[i][j] = ' ';
            }
            // shrink left
            int offset = 0;
            for (int j = 0; j + offset < 15; ++j)
            {
                while (j + offset < 15 && grids[0][j + offset] == ' ') ++offset;
                if (j + offset < 15)
                    for (int i = 0; i < 10; ++i)
                        grids[i][j] = grids[i][j + offset];
            }
            for (int j = 15 - offset; j < 15; ++j)
                for (int i = 0; i < 10; ++i)
                    grids[i][j] = ' ';
        }
        int remainingBalls = 0;
        for (int j = 0; j < 15; ++j)
            for (int i = 0; i < 10; ++i)
            {
                if (grids[i][j] != ' ')
                    ++remainingBalls;
            }
        if (remainingBalls == 0) points += 1000;
        cout << "Final score: " << points
            << ", with " << remainingBalls
            << " balls remaining. " << endl << endl;
    }

    return 0;
}