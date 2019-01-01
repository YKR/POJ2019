
#include <ctime>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <iostream>
#include <algorithm>

#include <string>
#include <vector>
#include <queue>
#include <list>
#include <map>

using namespace std;

double length[1000];

int main()
{
    ios::sync_with_stdio(false);

    int maxl = 0;

    memset(length, 0, sizeof(length));
    for (int i = 0;; ++i)
    {
        length[i+1] = length[i] + 1.0 / (i+1);
        if (length[i+1] > 6.20)
        {
            maxl = i+2;
            break;
        }
    }
    double d;
    while (cin >> d, d != 0.0)
    {
        d += 1.0;
        int l, r;
        for (l = 0, r = maxl; l < r;)
        {
            int mid = (l+r)>>1;
            if (length[mid] < d) l = mid + 1;
            else r = mid;
        }
        cout << r - 1 << " card(s)" << endl;
    }

    return 0;
}