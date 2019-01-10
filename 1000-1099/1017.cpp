
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

int numPackets[10];

int main()
{
    ios::sync_with_stdio(false);

    while (1)
    {
        int totalPackets = 0;
        for (int i = 1; i <= 6; ++i)
        {
            cin >> numPackets[i];
            totalPackets += numPackets[i];
        }
        if (totalPackets == 0) break;
        int ans = 0, extra2x2 = 0, extra1x1 = 0;
        ans += numPackets[6] + numPackets[5] + numPackets[4];
        extra2x2 += numPackets[4] * 5;
        extra1x1 += numPackets[5] * 11;
        ans += numPackets[3] / 4;
        switch(numPackets[3] % 4)
        {
            case 3:
                ++ans;
                extra2x2 += 1;
                extra1x1 += 5;
                break;
            case 2:
                ++ans;
                extra2x2 += 3;
                extra1x1 += 6;
                break;
            case 1:
                ++ans;
                extra2x2 += 5;
                extra1x1 += 7;
                break;
            default:
                break;
        }
        int cur2x2 = numPackets[2] - extra2x2;
        if (cur2x2 <= 0)
            extra1x1 += -cur2x2 * 4;
        else
        {
            ans += cur2x2 / 9;
            if (cur2x2 % 9)
            {
                ++ans;
                extra1x1 += (9 - cur2x2 % 9) * 4;
            }
        }
        int cur1x1 = numPackets[1] - extra1x1;
        if (cur1x1 > 0)
            ans += cur1x1 / 36 + (cur1x1 % 36 > 0 ? 1 : 0);
        cout << ans << endl;
    }

    return 0;
}