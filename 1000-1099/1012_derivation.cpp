
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

int main()
{
    ios::sync_with_stdio(false);

	int f[20] = {0, 2};
	int gameOver[30];

	for (int k = 2; k < 14; ++k)
	{
		f[k] = 0;
		memset(gameOver, 0, sizeof(gameOver));
		for (int m = 1; f[k] == 0; ++m)
		{
			bool pass = true;
			for (int i = 1; i <= k; ++i)
			{
				gameOver[i] = (gameOver[i - 1] - 1 + m) % ((k << 1) - i + 1);
				if (gameOver[i] < k)
				{
					pass = false;
					break;
				}
			}
			if (pass) f[k] = m;
		}
	}
	
    int q;
    while (cin >> q, q != 0)
    {
        cout << f[q] << endl;
    }

    return 0;
}