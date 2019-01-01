
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

int convert(const string & ss)
{
    int s = 0;
    for (int i = 0; i < ss.size(); i++)
    {
        if (ss[i] == '-') continue;
        s *= 10;
        if (ss[i] >= '0' && ss[i] <= '9') s += ss[i] - '0';
        else switch(ss[i])
        {
            case 'A': case 'B': case 'C': s += 2; break;
            case 'D': case 'E': case 'F': s += 3; break;
            case 'G': case 'H': case 'I': s += 4; break;
            case 'J': case 'K': case 'L': s += 5; break;
            case 'M': case 'N': case 'O': s += 6; break;
            case 'P': case 'R': case 'S': s += 7; break;
            case 'T': case 'U': case 'V': s += 8; break;
            case 'W': case 'X': case 'Y': s += 9; break;
        }
    }
    return s;
}

int main()
{
    ios::sync_with_stdio(false);

    int n;
    typedef map<int, int> mymap;
    mymap telemap;
    cin >> n;
    for (int i = 0; i < n; ++i)
    {
        string ss;
        cin >> ss;
        int s = convert(ss);
        if (telemap.find(s) != telemap.end())
            ++telemap[s];
        else
            telemap[s] = 1;
    }
    bool nodup = true;
    for (mymap::iterator i = telemap.begin(); i != telemap.end(); ++i)
    {
        if (i->second > 1)
        {
            int num = i->first;
            char dialnum[9];
            dialnum[8] = 0;
            for (int t = 7; t >= 0; t--)
            {
                if (t == 3) dialnum[t] = '-';
                else
                {
                    dialnum[t] = num % 10 + '0';
                    num /= 10;
                }
            }
            cout << dialnum << " " << i->second << endl;
            nodup = false;
        }
    }
    if (nodup)
        cout << "No duplicates." <<endl;

    return 0;
}