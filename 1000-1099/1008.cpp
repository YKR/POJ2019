
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

string Haab[20] = { "pop", "no", "zip", "zotz", "tzec", "xul", "yoxkin", "mol", "chen", "yax", "zac", "ceh", "mac", "kankin", "muan", "pax", "koyab", "cumhu", "uayet" };
string Tzolkin[20] = { "imix", "ik", "akbal", "kan", "chicchan", "cimi", "manik", "lamat", "muluk", "ok", "chuen", "eb", "ben", "ix", "mem", "cib", "caban", "eznab", "canac", "ahau" };

int n;

int findHaab(const char * mstr)
{
    for (int i = 0; i < 19; ++i)
        if (Haab[i] == mstr) return i;
    return 0;
}

int main()
{
    ios::sync_with_stdio(false);

    cin >> n;
    cout << n << endl;
    int absday;
    int hday, hmonth, hyear;
    char dot, monthstr[20];
    for (int i = 0; i < n; ++i)
    {
        cin >> hday >> dot >> monthstr >> hyear;
        hmonth = findHaab(monthstr);
        absday = hyear * 365 + hmonth * 20 + hday;
        int dm = absday % 260;
        cout << dm % 13 + 1 << " " << Tzolkin[dm % 20] << " " << absday / 260 << endl;
    }

    return 0;
}