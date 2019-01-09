
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

string myToString(int nn)
{
    string ret;
    while (nn)
    {
        ret.push_back(nn % 10 + '0');
        nn /= 10;
    }
    for (int i = 0; i < (ret.size() >> 1); ++i)
        swap(ret[i], ret[ret.size() - i - 1]);
    return ret;
}

int main()
{
    ios::sync_with_stdio(false);

    string s;
    while (cin >> s, s != "-1")
    {
        bool selfinv = false;
        string curs = s;
        map<string, int> inventories;
        for (int i = 0; i < 15; ++i)
        {
            inventories[curs] = i - 1;
            string inventory;
            int cnt[10];
            memset(cnt, 0, sizeof(cnt));
            for (int t = 0; t < curs.size(); ++t)
                ++cnt[curs[t] - '0'];
            for (int nn = 0; nn < 10; ++nn)
            {
                if (cnt[nn] > 0)
                {
                    inventory += myToString(cnt[nn]);
                    inventory.push_back(nn + '0');
                }
            }
            map<string, int>::iterator found = inventories.find(inventory);
            if (found != inventories.end())
            {
                selfinv = true;
                if (found->second == -1)
                {
                    if (i == 0)
                        cout << s << " is self-inventorying" << endl;
                    else
                        cout << s << " enters an inventory loop of length " << i + 1 << endl;
                }
                else if (found->second == i - 1)
                {
                    cout << s << " is self-inventorying after " << i << " steps" << endl;
                }
                else
                {
                    cout << s << " enters an inventory loop of length " << i - found->second << endl;
                }
                break;
            }
            else
                inventories[inventory] = i;
            curs = inventory;
        }
        if (!selfinv)
        {
            cout << s << " can not be classified after 15 iterations" << endl;
        }
    }

    return 0;
}