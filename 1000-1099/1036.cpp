
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

struct Gangster
{
    int t, p, s;
};

int n, k, t;
Gangster g[110];
bool gangsterTimeLess(const Gangster & a, const Gangster & b)
{
    return a.t < b.t;
}
int f[110];

bool cando(int a, int b)
{
    return abs(g[b].s - g[a].s) <= g[b].t - g[a].t;
}

int main()
{
    ios::sync_with_stdio(false);

    cin >> n >> k >> t;
    g[0].t = g[0].p = g[0].s = 0;
    for (int i = 0; i < n; ++i)
        cin >> g[i + 1].t;
    for (int i = 0; i < n; ++i)
        cin >> g[i + 1].p;
    for (int i = 0; i < n; ++i)
        cin >> g[i + 1].s;
    sort(g + 1, g + n + 1, gangsterTimeLess);
    f[0] = 0;
    int ans = 0;
    for (int i = 1; i <= n; ++i)
    {
        f[i] = -1;
        for (int j = 0; j < i; ++j)
        {
            if (f[j] >= 0 && cando(j, i))
                f[i] = max(f[i], f[j] + g[i].p);
        }
        ans = max(ans, f[i]);
    }
    cout << ans << endl;

    return 0;
}