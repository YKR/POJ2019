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

#define STATE_GET_BIT(STATE, POS) (((STATE) >> (POS) * 2) & 0x3)

int D;
int n, m;
short plate[160];

int f[2][1048576];
int curf;

int main()
{
    ios::sync_with_stdio(false);

    cin >> D;
    while (D--)
    {
        memset(plate, 0, sizeof(plate));
        int K;
        cin >> n >> m >> K;
        while (K--)
        {
            int x, y;
            cin >> x >> y;
            plate[x - 1] |= 1 << (y - 1);
        }
        curf = 0;
        memset(f[curf], -1, sizeof(f[curf]));
        f[curf][(1 << m * 2) - 1] = 0;
        for (int i = 0; i <= n; ++i)
        {
            memset(f[1 ^ curf], -1, sizeof(f[1 ^ curf]));
            for (int s = 0; s < 1048576; ++s)
            {
                if (f[curf][s] == -1) continue;
                int nexts = 0;
                for (int j = 0; j < m; ++j)
                    nexts |= (((plate[i] >> j) & 1) ? 0x3 : max(((s >> j * 2) & 0x3) - 1, 0)) << j * 2;
                f[1 ^ curf][nexts] = max(f[1 ^ curf][nexts], f[curf][s]);
                for (int j = 0; j + 3 <= m; ++j)
                    if (!(s & (0x2A << j * 2)))
                    {
                        int filled = s | (0x3f << j * 2);
                        f[curf][filled] = max(f[curf][filled], f[curf][s] + 1);
                    }
                for (int j = 0; j + 2 <= m; ++j)
                    if (!(s & (0xf << j * 2)))
                    {
                        int filled = s | (0xf << j * 2);
                        f[curf][filled] = max(f[curf][filled], f[curf][s] + 1);
                    }
            }
            curf = 1 ^ curf;
        }
        int ans = -1;
        for (int s = 0; s < 1048576; ++s)
        {
            if (ans < f[curf][s])
                ans = f[curf][s];
        }
        cout << ans << endl;
    }

    return 0;
}