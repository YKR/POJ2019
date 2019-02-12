
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

#define XDOWN 0
#define XUP 1

long long f[30][30][2];

int K, n;
long long c;

int main()
{
    ios::sync_with_stdio(false);

    f[1][0][XUP] = 1;
    f[1][0][XDOWN] = 1;
    for (int i = 2; i <= 20; ++i)
    {
        for (int j = 0; j < i; ++j)
        {
            f[i][j][XUP] = 0;
            f[i][j][XDOWN] = 0;
            for (int k = 0; k < i - 1; ++k)
            {
                if (k < j) f[i][j][XDOWN] += f[i - 1][k][XUP];
                else f[i][j][XUP] += f[i - 1][k][XDOWN];
            }
        }
    }
    cin >> K;
    while (K--)
    {
        vector<int> ans;
        cin >> n >> c;
        int last = -1;
        int updown = -1;
        for (int i = n; i >= 1; --i)
        {
            long long accum = 0;
            int targetj = -1;
            for (int j = updown == XUP ? last : 0; j < i; ++j)
            {
                bool hitRange = false;
                for (int ud = 0; ud < 2; ++ud)
                {
                    if (ud == updown) continue;
                    if (accum + f[i][j][ud] >= c)
                    {
                        c -= accum;
                        updown = ud;
                        hitRange = true;
                        break;
                    }
                    accum += f[i][j][ud];
                }
                if (hitRange)
                {
                    targetj = j;
                    break;
                }
            }
            ans.push_back(targetj + 1);
            last = targetj;
        }
        for (int i = ans.size() - 1; i >= 0; --i)
        {
            for (int j = i + 1; j < ans.size(); ++j)
                if (ans[j] >= ans[i])
                    ++ans[j];
        }
        for (int i = 0; i < ans.size(); ++i)
            cout << ans[i] << (i == ans.size() - 1 ? '\n' : ' ');
    }

    return 0;
}