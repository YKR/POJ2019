
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

#define ULL_HIGHEST_BIT 0x8000000000000000ULL

typedef unsigned long long ulltype;
typedef long long lltype;

int T;
int n;
string ppp;
lltype target;

int main()
{
    ios::sync_with_stdio(false);

    cin >> T;
    while (T--)
    {
        cin >> n >> ppp >> target;
        ulltype maxx = 0;
        ulltype offset = 0;
        for (int i = n - 1; i >= 0; --i)
        {
            if (ppp[n - i - 1] == 'n')
                offset |= (1ULL << i);
            maxx |= (1ULL << i);
        }
        bool impossible = false;
        ulltype bits = 0;
        if (target >= 0)
        {
            ulltype utar = target;
            if (utar <= (maxx ^ offset))
                bits = offset + utar;
            else
                impossible = true;
        }
        else
        {
            ulltype utar = -target;
            offset = ~offset & maxx;
            if (utar <= (maxx ^ offset))
                bits = offset + utar;
            else
                impossible = true;
        }
        if (impossible)
            cout << "Impossible" << endl;
        else
        {
            string outstr; outstr.resize(n);
            for (int i = n - 1; i >= 0; --i)
            {
                if (target >= 0 && ppp[i] == 'n' || target < 0 && ppp[i] == 'p')
                    outstr[i] = '1' - (bits & 1ULL);
                else
                    outstr[i] = (bits & 1ULL) + '0';
                bits >>= 1ULL;
            }
            cout << outstr << endl;
        }
    }

    return 0;
}