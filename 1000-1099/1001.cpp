
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

struct BigDouble
{
    vector<int> nums;
    int point;

    void eliminateBlankZero()
    {
        int nonzero = nums.size();
        while (nonzero > point && nums[nonzero - 1] == 0) nums.pop_back(), --nonzero;
        nonzero = 0;
        while (nonzero < point && nums[nonzero] == 0) ++nonzero;
        if (nonzero)
        {
            nums.erase(nums.begin(), nums.begin() + nonzero);
            point -= nonzero;
        }
    }

    BigDouble operator* (const BigDouble & t)
    {
        BigDouble ret;
        ret.nums.resize(nums.size() + t.nums.size() + 1);
        for (int i = 0; i < ret.nums.size(); i++)
            ret.nums[i] = 0;
        for (int i = 0; i < nums.size(); i++)
            for (int j = 0; j < t.nums.size(); j++)
                ret.nums[i+j] += nums[i] * t.nums[j];
        for (int i = 0; i < ret.nums.size() - 1; i++)
            ret.nums[i+1] += ret.nums[i] / 10,
            ret.nums[i] %= 10;
        ret.point = point + t.point;
        ret.eliminateBlankZero();
        return ret;
    }

    BigDouble & operator= (const string & s)
    {
        nums.clear();
        nums.reserve(s.size());
        point = 0;
        for (int i = s.size() - 1, j = 0; i >= 0; --i)
        {
            if (s[i] != '.') nums.push_back(s[i]-'0'), ++j;
            else point = j;
        }
        eliminateBlankZero();
    }

    operator string()
    {
        string ret;
        ret.reserve(nums.size() + 1);
        if (point == nums.size()) ret.push_back('.');
        for (int i = nums.size() - 1; i >= 0; --i)
        {
            ret.push_back(nums[i] + '0');
            if (i == point && i != 0) ret.push_back('.');
        }
        return ret;
    }
};

int main()
{
    ios::sync_with_stdio(false);
    
    string r;
    int n;

    while (cin >> r >> n)
    {
        BigDouble ans, e;
        ans = "1"; e = r;
        while (n)
        {
            if (n & 1) ans = ans * e;
            e = e * e;
            n >>= 1;
        }
        string ansStr = ans;
        if (ansStr == ".")
            cout << "0" << endl;
        else
            cout << string(ans) << endl;
    }

    return 0;
}