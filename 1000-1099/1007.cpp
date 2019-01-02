
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

int n, m;
char seq[110][60];
vector<pair<int, int> > sf;

char dup[60];
int mergeSort(char *s, int l, int r) // [l,r)
{
    if (l + 1 >= r) return 0;
    int ret = 0;
    int mid = (l + r) >> 1;
    ret += mergeSort(s, l, mid);
    ret += mergeSort(s, mid, r);
    memcpy(dup + l, s + l, r - l);
    int i, j, k;
    for (i = l, j = mid, k = l; i < mid && j < r;)
    {
        if (dup[i] <= dup[j]) s[k++] = dup[i++];
        else
        {
            ret += mid - i;
            s[k++] = dup[j++];
        }
    }
    while (i < mid) s[k++] = dup[i++];
    while (j < r) s[k++] = dup[j++];
    return ret;
}

char cur[60];
int calcSortedness(int num)
{
    memcpy(cur, seq[num], sizeof(cur));
    return mergeSort(cur, 0, n);
}

int main()
{
    ios::sync_with_stdio(false);

    cin >> n >> m;
    sf.reserve(m);

    for (int i = 0; i < m; ++i)
    {
        cin >> seq[i];
        sf.push_back(make_pair(calcSortedness(i), i));
    }
    sort(sf.begin(), sf.end());
    for (int i = 0; i < m; ++i)
    {
        cout << seq[sf[i].second] << endl;
    }

    return 0;
}