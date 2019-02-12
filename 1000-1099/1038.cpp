
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

const int power3[12] = {1, 3, 9, 27, 81, 243, 729, 2187, 6561, 19683, 59049};
inline int encode3(int bit, int pos)
{
    return bit * power3[pos];
}
inline int decode3(int code, int pos)
{
    return code / power3[pos] % 3;
}

int D;
int n, m;
short plate[160];

int f[2][60010];
int curf;

void searchNext(int s, int next, int pos, int num)
{
    if (pos >= m)
    {
        f[1 ^ curf][next] = max(f[1 ^ curf][next], f[curf][s] + num);
        return;
    }
    searchNext(s, next, pos + 1, num);
    if (pos + 2 <= m && decode3(s, pos) == 0 && decode3(s, pos + 1) == 0
        && decode3(next, pos) == 0 && decode3(next, pos + 1) == 0)
    {
        int addedNext = next + encode3(2, pos) + encode3(2, pos + 1);
        searchNext(s, addedNext, pos + 2, num + 1);
    }
    if (pos + 3 <= m && decode3(next, pos) == 0
        && decode3(next, pos + 1) == 0 && decode3(next, pos + 2) == 0)
    {
        int addedNext = next + encode3(2, pos) + encode3(2, pos + 1) + encode3(2, pos + 2);
        searchNext(s, addedNext, pos + 3, num + 1);
    }
}

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
        plate[n] = -1;
        curf = 0;
        memset(f[curf], -1, sizeof(f[curf]));
        f[curf][power3[m] - 1] = 0;
        for (int i = 0; i <= n; ++i)
        {
            memset(f[1 ^ curf], -1, sizeof(f[1 ^ curf]));
            for (int s = 0; s < power3[10]; ++s)
            {
                if (f[curf][s] == -1) continue;
                int basicNext = 0;
                for (int j = 0; j < m; ++j)
                    basicNext += encode3(((plate[i] >> j) & 1) ? 2 : max(decode3(s, j) - 1, 0), j);
                searchNext(s, basicNext, 0, 0);
            }
            curf = 1 ^ curf;
        }
        int ans = -1;
        for (int s = 0; s < power3[10]; ++s)
        {
            if (ans < f[curf][s])
                ans = f[curf][s];
        }
        cout << ans << endl;
    }

    return 0;
}