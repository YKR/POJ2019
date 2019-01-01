
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

#include <iomanip>

using namespace std;

int main()
{
    ios::sync_with_stdio(false);

    double sum;
    for (int i = 0; i < 12; ++i)
    {
        double money;
        cin >> money;
        sum += money;
    }
    sum /= 12.0;
    cout << "$" << setprecision(2) << fixed << sum <<endl;

    return 0;
}