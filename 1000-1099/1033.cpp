
#include <ctime>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <iomanip>
#include <sstream>
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

int n, k;
int blocks[10010];

int lastEmptyBlock;
bool visited[10010];
int switchBack;
void SwapBlock(int index)
{
    visited[index] = true;
    int targetBlock = blocks[index];
    if (blocks[targetBlock] != -1)
    {
        if (visited[targetBlock])
        {
            blocks[index] = -1;
            blocks[lastEmptyBlock] = targetBlock;
            cout << index << ' ' << lastEmptyBlock << endl;
            switchBack = lastEmptyBlock;
            lastEmptyBlock = index;
            visited[index] = false;
            return;
        }
        else
            SwapBlock(targetBlock);
    }
    blocks[index] = -1;
    blocks[targetBlock] = targetBlock;
    cout << index << ' ' << targetBlock << endl;
    lastEmptyBlock = index;
    visited[index] = false;
}

int main()
{
    ios::sync_with_stdio(false);

    memset(blocks, -1, sizeof(blocks));
    memset(visited, 0, sizeof(visited));
    cin >> n >> k;
    int targetLocation = 0;
    for (int i = 0; i < k; ++i)
    {
        int numBlocks;
        cin >> numBlocks;
        for (int j = 0; j < numBlocks; ++j)
        {
            ++targetLocation;
            int blockIndex;
            cin >> blockIndex;
            blocks[blockIndex] = targetLocation;
        }
    }
    lastEmptyBlock = 0;
    for (int i = 1; i <= n; ++i)
    {
        if (blocks[i] == -1)
        {
            lastEmptyBlock = i;
            break;
        }
    }
    bool opti = false;
    for (int i = 1; i <= n; ++i)
    {
        if (blocks[i] != -1 && blocks[i] != i)
        {
            opti = true;
            switchBack = -1;
            SwapBlock(i);
            if (switchBack != -1)
            {
                int targetBlock = blocks[switchBack];
                blocks[switchBack] = -1;
                blocks[targetBlock] = targetBlock;
                cout << switchBack << ' ' << targetBlock << endl;
                lastEmptyBlock = switchBack;
            }
        }
    }
    if (!opti)
    {
        cout << "No optimization needed" << endl;
    }

    return 0;
}