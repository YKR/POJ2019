
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

int n;
vector<int> sticks;
bool used[100];
int maxStickLen;
int sum;

bool verify(int mergeLen, int mergeLeft, int curMergeNum, int curStickNum)
{
    if (mergeLeft == 0)
    {
        mergeLeft = mergeLen;
        ++curMergeNum;
    }
    if (curMergeNum == sum / mergeLen) return true;
    if (mergeLeft == mergeLen)
    {
        // Start of a new merge
        // Longest stick remained must be used
        for (int i = 0; i < sticks.size(); ++i)
        {
            if (!used[i])
            {
                used[i] = true;
                if (verify(mergeLen, mergeLeft - sticks[i], curMergeNum, i + 1))
                    return true;
                used[i] = false;
                return false;
            }
        }
        return false; // never here?
    }
    for (int i = curStickNum; i < sticks.size(); ++i)
    {
        if (used[i]) continue;
        if (mergeLeft < sticks[i]) continue;
        if (mergeLeft > sticks[i] * (sticks.size() - i)) return false;
        used[i] = true;
        if (verify(mergeLen, mergeLeft - sticks[i], curMergeNum, i + 1))
            return true;
        used[i] = false;
        while (i < sticks.size() - 1 && sticks[i + 1] == sticks[i]) ++i;
    }
    return false;
}

int main()
{
    ios::sync_with_stdio(false);

    sticks.reserve(64);
    while (cin >> n, n != 0)
    {
        sticks.clear();
        sticks.resize(n);
        maxStickLen = 0;
        sum = 0;
        for (int i = 0; i < n; ++i)
        {
            cin >> sticks[i];
            sum += sticks[i];
            maxStickLen = max(maxStickLen, sticks[i]);
        }
        sort(sticks.begin(), sticks.end(), greater<int>());
        memset(used, 0, sizeof(used));
        bool ansIsSum = true;
        for (int f = maxStickLen; f <= sum / 2; ++f)
        {
            if (sum % f) continue;
            if (verify(f, f, 0, 0))
            {
                ansIsSum = false;
                cout << f << endl;
                break;
            }
        }
        if (ansIsSum)
            cout << sum << endl;
    }

    return 0;
}