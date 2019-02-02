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

int disp[210];
int n, k;

void dispMult(int * dst, int * src)
{
    int tmp[210];
    for (int i = 0; i < n; ++i)
        tmp[i] = dst[src[i]];
    for (int i = 0; i < n; ++i)
        dst[i] = tmp[i];
}

int main()
{
    ios::sync_with_stdio(false);

    while (cin >> n, n != 0)
    {
        int revdisp[210];
        for (int i = 0; i < n; ++i) cin >> revdisp[i], disp[revdisp[i] - 1] = i;
        while (cin >> k, k != 0)
        {
            char line[210];
            cin.get(line[0]);
            cin.getline(line, n + 1);
            int l = strlen(line);
            for (int i = l; i < n; ++i) line[i] = ' ';
            line[n] = 0;
            int code[210], e[210];
            for (int i = 0; i < n; ++i) code[i] = i, e[i] = disp[i];
            while (k)
            {
                if (k & 1) dispMult(code, e);
                dispMult(e, e);
                k >>= 1;
            }
            for (int i = 0; i < n; ++i) cout << line[code[i]];
            cout << endl;
        }
        cout << endl;
    }

    return 0;
}