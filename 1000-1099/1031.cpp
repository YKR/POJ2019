
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

const double myPi = 3.1415926535897932;

int n;
double k, h;
double alphas[110];

int main()
{
    ios::sync_with_stdio(false);

    cin >> k >> h >> n;
    for (int i = 0; i < n; ++i)
    {
        double x, y;
        cin >> x >> y;
        double r = sqrt(x * x + y * y);
        alphas[i] = acos(x / r);
        if (y < 0) alphas[i] = 2 * myPi - alphas[i];
    }
    alphas[n] = alphas[0];
    double borderAlpha[2] = {alphas[0], alphas[0]};
    double curAlpha = alphas[0];
    for (int i = 1; i <= n; ++i)
    {
        double deltaAlpha = alphas[i] - alphas[i - 1];
        if (deltaAlpha > myPi)
        {
            deltaAlpha -= 2 * myPi;
        }
        else if (deltaAlpha < -myPi)
        {
            deltaAlpha += 2 * myPi;
        }
        curAlpha += deltaAlpha;
        borderAlpha[0] = min(borderAlpha[0], curAlpha);
        borderAlpha[1] = max(borderAlpha[1], curAlpha);
    }
    double ans = k * h * min(borderAlpha[1] - borderAlpha[0], 2 * myPi);
    cout << fixed << setprecision(2) << ans << endl;

    return 0;
}