
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
int s, n;
int piece[20];
bool used[20];
int cutted[200];

bool cutcake(int cakeAreaLeft)
{
    if (cakeAreaLeft == 0) return true;
    int minCut = 0x7f7f7f, jMin = 0;
    for (int j = 0; j < s; ++j)
        if (minCut > cutted[j])
            minCut = cutted[j], jMin = j;
    int jMax;
    for (jMax = jMin; jMax < s && cutted[jMax] == minCut; ++jMax);
    for (int i = 0; i < n; ++i)
        if (!used[i] && piece[i] <= jMax - jMin && minCut + piece[i] <= s)
        {
            // cut
            for (int j = jMin; j < jMin + piece[i]; ++j)
                cutted[j] += piece[i];
            used[i] = true;
            // search
            if (cutcake(cakeAreaLeft - piece[i] * piece[i])) return true;
            // uncut
            for (int j = jMin; j < jMin + piece[i]; ++j)
                cutted[j] -= piece[i];
            used[i] = false;
            while (i < n - 1 && piece[i] == piece[i + 1]) ++i;
        }
    return false;
}

int main()
{
    ios::sync_with_stdio(false);

    cin >> T;
    while (T--)
    {
        cin >> s >> n;
        int sumSqrPieces = 0;
        int singleton = 0;
        for (int i = 0; i < n; ++i)
        {
            cin >> piece[i];
            sumSqrPieces += piece[i] * piece[i];
            if (piece[i] > (s >> 1)) ++singleton;
        }
        sort(piece, piece + n, greater<int>());
        memset(cutted, 0, sizeof(cutted));
        memset(used, 0, sizeof(used));
        if (sumSqrPieces == s * s && singleton <= 1 && cutcake(sumSqrPieces))
            cout << "KHOOOOB!" << endl;
        else
            cout << "HUTUTU!" << endl;
    }

    return 0;
}