
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

bool dp[205][25][850];
bool pick[205][25][850];
int ssum[205][25][850];
int n, m;
int d[205]; // d - p
int s[205]; // d + p

int main()
{
    ios::sync_with_stdio(false);

    int caseCount = 0;
    while (cin >> n >> m, !(n == 0 && m == 0))
    {
        ++caseCount;
        for (int i = 0; i < n; ++i)
        {
            int dd, pp;
            cin >> pp >> dd;
            d[i] = dd - pp + 20;
            s[i] = dd + pp;
        }
        memset(dp, 0, sizeof(dp));
        memset(pick, 0, sizeof(pick));
        memset(ssum, -1, sizeof(ssum));
        dp[0][0][0] = true;
        ssum[0][0][0] = 0;
        for (int i = 1; i <= n; ++i)
        {
            for (int j = 0; j <= m; ++j)
            {
                for (int k = 0; k <= m * 40; ++k)
                {
                    if (dp[i-1][j][k])
                    {
                        dp[i][j][k] = true;
                        pick[i][j][k] = false;
                        ssum[i][j][k] = ssum[i-1][j][k];
                    }
                    if (j > 0 && k >= d[i-1] && dp[i-1][j-1][k-d[i-1]]
                        && ssum[i][j][k] < ssum[i-1][j-1][k-d[i-1]] + s[i-1])
                    {
                        dp[i][j][k] = true;
                        pick[i][j][k] = true;
                        ssum[i][j][k] = ssum[i-1][j-1][k-d[i-1]] + s[i-1];
                    }
                }
            }
        }
        for (int o = 0; o <= m * 20; ++o)
        {
            int ans = -1;
            if (dp[n][m][m * 20 + o] && ssum[n][m][m * 20 + o] > ssum[n][m][m * 20 - o])
                ans = m * 20 + o;
            else if (dp[n][m][m * 20 - o])
                ans = m * 20 - o;
            if (ans != -1)
            {
                int seq[25];
                for (int i = n, j = m, k = ans; i > 0;)
                {
                    if (pick[i][j][k])
                    {
                        seq[j - 1] = i;
                        --j;
                        k -= d[i - 1];
                    }
                    --i;
                }
                int sums = ssum[n][m][ans], sumd = ans - m * 20;
                int dd = (sums + sumd) >> 1, pp = (sums - sumd) >> 1;
                cout << "Jury #" << caseCount << endl;
                cout << "Best jury has value " << pp << " for prosecution and value " << dd << " for defence:" << endl;
                for (int i = 0; i < m; ++i)
                    cout << " " << seq[i];
                cout << endl << endl;
                break;
            }
        }
    }

    return 0;
}