
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

struct numRange
{
    long long wordlength; // range[1].wordlength = 1, range[2].wordlength = 2
    long long lowerbound; // range[1].lowerbound = 1, range[2].lowerbound = 10
    long long upperbound; // range[1].upperbound = 9, range[2].upperbound = 99
    long long accumlength; // ..., 1, 2, ...,98, {1, 2, ..., 99}: range[2].accumlength = number of digits in {...} = 189
    long long endpos; // {1, 1, 2, 1, 2, 3, ..., 1, ..., 9}: range[1].endpos = number of digits in {...} = 45
};

numRange ranges[10010];
long long totalRanges;
long long T;

int main()
{
    ios::sync_with_stdio(false);

    ranges[0].wordlength = 0;
    ranges[0].lowerbound = 0;
    ranges[0].upperbound = 0;
    ranges[0].accumlength = 0;
    ranges[0].endpos = 0;
    ranges[1].wordlength = 1;
    ranges[1].lowerbound = 1;
    ranges[1].upperbound = 9;
    ranges[1].accumlength = 9;
    ranges[1].endpos = 45;
    for (long long i = 2; ; ++i)
    {
        ranges[i].wordlength = ranges[i - 1].wordlength + 1;
        ranges[i].lowerbound = ranges[i - 1].lowerbound * 10;
        ranges[i].upperbound = ranges[i - 1].upperbound * 10 + 9;
        ranges[i].accumlength = ranges[i - 1].accumlength
            + (ranges[i].upperbound - ranges[i].lowerbound + 1) * ranges[i].wordlength;
        ranges[i].endpos = ranges[i - 1].endpos
            + (ranges[i - 1].accumlength + ranges[i].wordlength + ranges[i].accumlength)
            * (ranges[i].upperbound - ranges[i].lowerbound + 1) / 2;
        if (ranges[i].endpos > 0x7FFFFFFFLL)
        {
            totalRanges = i + 1;
            break;
        }
    }

    cin >> T;
    while (T--)
    {
        long long n;
        cin >> n;
        long long l, r;
        for (l = 0, r = totalRanges; l < r;)
        {
            long long mid = (l + r) >> 1;
            if (ranges[mid].endpos < n)
                l = mid + 1;
            else
                r = mid;
        }
        long long startpos = ranges[r - 1].endpos + 1;
        // ..., 99, {1}, 2, ..., 100: [r == 3] startpos = position of the 1 in {...}
        long long endpos = ranges[r].endpos;
        // ..., 998, 1, 2, ..., {999}: [r == 3] endpos = position of the last 9 in {...}
        long long offset = n - startpos;
        // ..., 99, {1}, 2, ..., 100, ..., n, ..., 1, 2, ..., 999: [r == 3] offset = position offset from {1} to n
        long long arrayend, arrayaccum;
        // ..., 1, 2, 3, .., n, ., 232: arrayend == 232
        // ..., {1, 2, ..., 100, ..., 1, ..., 231}, 1, ..., n, ..., 232: arrayaccum = number of digits in {...}
        for (arrayend = ranges[r].lowerbound, arrayaccum = 0; arrayaccum <= offset; ++arrayend)
            arrayaccum += (arrayend - ranges[r].lowerbound + 1) * ranges[r].wordlength + ranges[r - 1].accumlength;
        --arrayend, arrayaccum -= (arrayend - ranges[r].lowerbound + 1) * ranges[r].wordlength + ranges[r - 1].accumlength;
        long long accumleft = offset - arrayaccum;
        // ..., 1, 2, ..., 100, ..., 1, ..., 231, {1}, ..., n, ..., 232: accumleft = position offset from {1} to n
        long long t;
        for (t = 1; t < totalRanges && accumleft >= ranges[t].accumlength; ++t);
        accumleft -= ranges[t - 1].accumlength;
        // ..., 231, 1, ..., {100}, ..., n, ..., 232: accumleft = position offset from the first digit of {100} to n
        long long word = accumleft / ranges[t].wordlength + ranges[t].lowerbound;
        long long wordbit = accumleft % ranges[t].wordlength;
        while (wordbit < ranges[t].wordlength - 1)
        {
            word = word / 10;
            ++wordbit;
        }
        cout << word % 10 << endl;
    }

    return 0;
}