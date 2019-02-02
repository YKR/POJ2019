
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

int n, k, p;
int suspect[1010];
int leftside[1010];
int rightside[1010];
int nonside[1010];

int main()
{
    ios::sync_with_stdio(false);

    cin >> n >> k;
    memset(suspect, 0, sizeof(suspect));
    while (k--)
    {
        bool onbalance[1010];
        memset(onbalance, 0, sizeof(onbalance));
        cin >> p;
        for (int i = 0; i < p; ++i)
            cin >> leftside[i], onbalance[leftside[i]] = true;
        for (int i = 0; i < p; ++i)
            cin >> rightside[i], onbalance[rightside[i]] = true;
        for (int i = 1, cnt = 0; i <= n; ++i)
            if (!onbalance[i])
                nonside[cnt++] = i;
        string result;
        cin >> result;
        int *susleft = leftside, *susright = rightside;
        switch (result[0])
        {
            case '<':
                susleft = rightside;
                susright = leftside;
            case '>':
                for (int i = 0; i < p; ++i)
                    if (suspect[susleft[i]] == -1 || suspect[susleft[i]] == 1)
                        suspect[susleft[i]] = -1;
                    else
                        suspect[susleft[i]] = 2;
                for (int i = 0; i < p; ++i)
                    if (suspect[susright[i]] == -1 || suspect[susright[i]] == 2)
                        suspect[susright[i]] = -1;
                    else
                        suspect[susright[i]] = 1;
                for (int i = 0; i < n - 2 * p; ++i)
                    suspect[nonside[i]] = -1;
                break;
            case '=':
                for (int i = 0; i < p; ++i)
                    suspect[susleft[i]] = suspect[susright[i]] = -1;
                break;
        }
    }
    int lastsus = 0, susnum = 0;
    for (int i = 1; i <= n; ++i)
    {
        if (suspect[i] != -1) lastsus = i, ++susnum;
    }
    if (susnum != 1) cout << 0 << endl;
    else cout << lastsus << endl;

    return 0;
}