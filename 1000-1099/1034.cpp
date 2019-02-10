
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

int n, m;
int xybob[210], xydog[210];

inline int i2x(int i) { return i << 1; }
inline int i2y(int i) { return (i << 1) + 1; }

inline double dist(int x0, int y0, int x1, int y1)
{
    return sqrt(double(x0 - x1) * double(x0 - x1) + double(y0 - y1) * double(y0 - y1));
}

bool validChoice[110][110];
int match[2][110];

bool visited[110];
bool hungary(int curA)
{
    for (int j = 0; j < m; ++j)
    {
        if (!validChoice[curA][j] || visited[j]) continue;
        visited[j] = true;
        if (match[1][j] == -1 || hungary(match[1][j]))
        {
            match[0][curA] = j;
            match[1][j] = curA;
            return true;
        }
    }
    return false;
}

int main()
{
    ios::sync_with_stdio(false);

    memset(validChoice, 0, sizeof(validChoice));
    cin >> n >> m;
    for (int i = 0; i < n; ++i)
        cin >> xybob[i2x(i)] >> xybob[i2y(i)];
    for (int j = 0; j < m; ++j)
    {
        cin >> xydog[i2x(j)] >> xydog[i2y(j)];
        for (int i = 1; i < n; ++i)
        {
            double distbob = dist(xybob[i2x(i)], xybob[i2y(i)],
                xybob[i2x(i - 1)], xybob[i2y(i - 1)]);
            double distdog = dist(xybob[i2x(i)], xybob[i2y(i)],
                xydog[i2x(j)], xydog[i2y(j)])
                + dist(xydog[i2x(j)], xydog[i2y(j)],
                xybob[i2x(i - 1)], xybob[i2y(i - 1)]);
            if (distdog <= distbob * 2.0 + 1e-9)
                validChoice[i][j] = true;
        }
    }
    memset(match, -1, sizeof(match));
    int ansn = 0;
    for (int i = 1; i < n; ++i)
    {
        if (match[0][i] == -1)
        {
            memset(visited, 0, sizeof(visited));
            ansn += hungary(i) ? 1 : 0;
        }
    }
    cout << ansn + n << endl;
    for (int i = 0; i < n; ++i)
    {
        if (match[0][i] != -1)
            cout << xydog[i2x(match[0][i])] << ' ' << xydog[i2y(match[0][i])] << ' ';
        cout << xybob[i2x(i)] << ' ' << xybob[i2y(i)] << (i == n - 1 ? '\n' : ' ');
    }

    return 0;
}