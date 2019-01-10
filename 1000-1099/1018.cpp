
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

int T;
int n;
vector<pair<int, int> > devices[110];
vector<int> bw;
int f[110][10010];
double ans;

int main()
{
    ios::sync_with_stdio(false);

    cin >> T;
    while (T--)
    {
        cin >> n;
        for (int i = 0; i < n; ++i)
        {
            int m;
            cin >> m;
            devices[i].resize(m);
            for (int j = 0; j < devices[i].size(); ++j)
            {
                cin >> devices[i][j].first >> devices[i][j].second;
            }
            // never choose a when devices[a].bandwidth <= devices[b].bandwidth but devices[a].price >= devices[b].price
            sort(devices[i].begin(), devices[i].end());
            int offset = 0;
            for (int j = 0; j < devices[i].size(); ++j)
            {
                if (j - offset > 0
                    && devices[i][j - offset - 1].first <= devices[i][j].first
                    && devices[i][j - offset - 1].second >= devices[i][j].second)
                    ++offset;
                devices[i][j - offset] = devices[i][j];
            }
            devices[i].resize(devices[i].size() - offset);
            // after this, devices will be monotonically increasing on both bandwidth and price
            for (int j = 0; j < devices[i].size(); ++j)
                bw.push_back(devices[i][j].first);
        }
        sort(bw.begin(), bw.end());
        int offset = 0;
        for (int i = 0; i + offset < bw.size(); ++i)
        {
            bw[i] = bw[i + offset];
            while (i + offset + 1 < bw.size() && bw[i + offset + 1] == bw[i])
                ++offset;
        }
        bw.resize(bw.size() - offset);
        memset(f, 0x7f, sizeof(f));
        memset(f[0], 0, sizeof(f[0]));
        for (int i = 1; i <= n; ++i)
        {
            for (int j = 0, k = 0; j < bw.size(); ++j)
            {
                while (k < devices[i - 1].size() && bw[j] > devices[i - 1][k].first) ++k;
                if (k == devices[i - 1].size()) break;
                if (f[i - 1][j] == 0x7f7f7f7f) break;
                f[i][j] = f[i - 1][j] + devices[i - 1][k].second;
            }
        }
        ans = 0.0;
        for (int j = 0; j < bw.size(); ++j)
            if (ans < (double)bw[j] / f[n][j])
                ans = (double)bw[j] / f[n][j];
        cout << fixed << setprecision(3) << ans << endl;
    }

    return 0;
}