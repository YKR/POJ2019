
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

int main()
{
    ios::sync_with_stdio(false);

    cin >> n;
    int cnt = 0;
    while(n--)
    {
        ++cnt;
        double x, y;
        cin >> x >> y;
        double radius2 = x*x + y*y;
        double area = 0.5 * 3.1415926535897932384626 * radius2;
        double year = ceil(area / 50.0);
        cout << "Property " << cnt << ": This property will begin eroding in year " << (int)year << "." << endl;
    }
    cout << "END OF OUTPUT." << endl;

    return 0;
}