
#include <ctime>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <iomanip>
#include <sstream>
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

int n;

int main()
{
    ios::sync_with_stdio(false);

    vector<int> ans;
    cin >> n;
    for (int i = 2; n - i >= 0; ++i) ans.push_back(i), n -= i;
    while (n)
        for (int i = ans.size() - 1; i >= 0 && n; --i)
            ++ans[i], --n;
    for (int i = 0; i < ans.size(); ++i)
        cout << ans[i] << (i == ans.size() - 1 ? '\n' : ' ');

    return 0;
}