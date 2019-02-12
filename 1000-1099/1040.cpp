
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

int c, n, m;
int from[30], to[30], pcnt[30];
int oorder[30], sback[30];

bool OrderGreater(int a, int b)
{
    return (to[a] - from[a]) * pcnt[a] > (to[b] - from[b]) * pcnt[b];
}

int ans;
int train[10];
void dfs(int onum, int accum)
{
    if (onum == m)
    {
        if (ans < accum) ans = accum;
        return;
    }
    if (ans > accum + sback[onum]) return;
    int curo = oorder[onum];
    bool canAdd = true;
    for (int i = from[curo]; i < to[curo]; ++i)
        if (train[i] + pcnt[curo] > c)
        {
            canAdd = false;
            break;
        }
    if (canAdd)
    {
        for (int i = from[curo]; i < to[curo]; ++i)
            train[i] += pcnt[curo];
        dfs(onum + 1, accum + (to[curo] - from[curo]) * pcnt[curo]);
        for (int i = from[curo]; i < to[curo]; ++i)
            train[i] -= pcnt[curo];
    }
    dfs(onum + 1, accum);
}

int main()
{
    ios::sync_with_stdio(false);

    while (cin >> c >> n >> m, !(c == 0 && n == 0 && m == 0))
    {
        for (int i = 0; i < m; ++i)
            cin >> from[i] >> to[i] >> pcnt[i], oorder[i] = i;
        sort(oorder, oorder + m, OrderGreater);
        sback[m] = 0;
        for (int i = m - 1; i >= 0; --i)
            sback[i] = sback[i + 1] + (to[oorder[i]] - from[oorder[i]]) * pcnt[oorder[i]];
        memset(train, 0, sizeof(train));
        ans = 0;
        dfs(0, 0);
        cout << ans << endl;
    }

    return 0;
}