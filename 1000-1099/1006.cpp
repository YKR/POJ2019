
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

int main()
{
    ios::sync_with_stdio(false);

    int p, e, i, d;
    int anscnt = 0;
    int ans = 0;
    int m = 21252;
    int factors[3] = {5544, 14421, 1288};
    while (cin >> p >> e >> i >> d, !(p==-1 && e==-1 && i==-1 && d==-1))
    {
        ++anscnt;
        int nextday = (p * factors[0] + e * factors[1] + i * factors[2]) % m;
        ans = nextday - d;
        if (ans <= 0) ans += m;
        cout << "Case " << anscnt << ": the next triple peak occurs in " << ans << " days." << endl;
    }

    return 0;
}