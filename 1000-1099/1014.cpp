
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

int nMarbles[10];

bool fill(int sumLeft, int cur)
{
    if (sumLeft == 0) return true;
    if (cur == 0) return false;
    int maxm = min(sumLeft / cur, nMarbles[cur]);
    for (int i = maxm; i >= 0; --i)
    {
        nMarbles[cur] -= i;
        if (fill(sumLeft - cur * i, cur - 1)) return true;
        nMarbles[cur] += i;
    }
    return false;
}

int main()
{
    ios::sync_with_stdio(false);

    int caseCount = 1;
    while(1)
    {
        int sum = 0;
        for (int i = 1; i <= 6; ++i)
        {
            cin >> nMarbles[i];
            int oddeven = nMarbles[i] & 1;
            nMarbles[i] = min(nMarbles[i], 6 | oddeven);
            sum += nMarbles[i] * i;
        }
        if (sum == 0) break;
        if (!(sum & 1) && fill(sum >> 1, 6))
        {
            cout << "Collection #" << caseCount << ":" << endl;
            cout << "Can be divided." << endl;
            cout << endl;
        }
        else
        {
            cout << "Collection #" << caseCount << ":" << endl;
            cout << "Can't be divided." << endl;
            cout << endl;
        }
        ++caseCount;
    }

    return 0;
}