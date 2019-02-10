
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

long long gcd(long long a, long long b)
{
    if (a % b == 0) return b;
    return gcd(b , a % b);
}

struct BigInt
{
    vector<int> digits;
    BigInt & operator=(long long ll)
    {
        digits.clear();
        while (ll)
        {
            digits.push_back(ll % 10);
            ll /= 10;
        }
        return *this;
    }
    bool operator<(const BigInt & other) const
    {
        if (digits.size() != other.digits.size())
            return digits.size() < other.digits.size();
        else
        {
            for (int i = digits.size() - 1; i >= 0; --i)
            {
                if (digits[i] != other.digits[i])
                    return digits[i] < other.digits[i];
            }
        }
        return false; // ==
    }
    int operator%(int m) const
    {
        int rest = 0;
        for (int i = digits.size() - 1; i >= 0; --i)
        {
            rest = (rest * 10 + digits[i]) % m;
        }
        return rest;
    }
    BigInt operator*(int m) const
    {
        BigInt ret(*this);
        for (int i = 0; i < ret.digits.size(); ++i)
        {
            ret.digits[i] *= m;
        }
        for (int i = 0; i < ret.digits.size() - 1; ++i)
        {
            ret.digits[i + 1] += ret.digits[i] / 10;
            ret.digits[i] %= 10;
        }
        for (int carry = 0; carry = ret.digits.back() / 10; ret.digits.push_back(carry))
        {
            ret.digits.back() %= 10;
        }
        return ret;
    }
    friend ostream;
};

ostream & operator<< (ostream & o, const BigInt & bi)
{
    for (int i = bi.digits.size() - 1; i >= 0; --i)
    {
        o << bi.digits[i];
    }
    return o;
}

long long n;
BigInt f[1010];
long long g[1010];

int main()
{
    ios::sync_with_stdio(false);

    cin >> n;
    f[0] = 1;
    g[0] = 0;
    for (int i = 1; i <= n; ++i)
    {
        f[i] = 0;
        for (int j = 0; j < i; ++j)
        {
            int modResult = f[j] % (i - j);
            BigInt curProduct = f[j] * (modResult == 0 ? 1 : (i - j) / gcd(i - j, modResult));
            if (f[i] < curProduct)
            {
                f[i] = curProduct;
                g[i] = i - j;
            }
        }
    }
    for (int i = 1; i <= n; ++i) OUTPUT_DEBUG2(i, f[i]);
    vector<int> ans;
    for (int i = n; i > 0; i -= g[i]) ans.push_back(g[i]);
    sort(ans.begin(), ans.end());
    for (int i = 0; i < ans.size(); ++i)
        cout << ans[i] << (i == ans.size() - 1 ? '\n' : ' ');

    return 0;
}