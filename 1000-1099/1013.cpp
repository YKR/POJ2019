
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

int T;

int main()
{
    ios::sync_with_stdio(false);

    cin >> T;
    while (T--)
    {
        bool excluded[30];
        int suspect[30];
        memset(excluded, 0, sizeof(excluded));
        memset(suspect, 0, sizeof(suspect));
        string a, b, res;
        for (int i = 0; i < 3; ++i)
        {
            cin >> a >> b >> res;
            int bits = 0;
            if (res == "even")
            {
                for (int k = 0; k < a.size(); ++k)
                    excluded[a[k]-'A'] = true;
                for (int k = 0; k < b.size(); ++k)
                    excluded[b[k]-'A'] = true;
            }
            else if (res == "up")
            {
                for (int k = 0; k < a.size(); ++k)
                {
                    if (suspect[a[k]-'A'] == 1)
                        excluded[a[k]-'A'] = true;
                    else
                        suspect[a[k]-'A'] = -1;
                    bits |= 1 << (a[k]-'A');
                }
                for (int k = 0; k < b.size(); ++k)
                {
                    if (suspect[b[k]-'A'] == -1)
                        excluded[b[k]-'A'] = true;
                    else
                        suspect[b[k]-'A'] = 1;
                    bits |= 1 << (b[k]-'A');
                }
                for (int j = 'A'; j <= 'L'; ++j)
                {
                    if (bits & (1 << (j-'A'))) continue;
                    excluded[j-'A'] = true;
                }
            }
            else // res == "down"
            {
                for (int k = 0; k < a.size(); ++k)
                {
                    if (suspect[a[k]-'A'] == -1)
                        excluded[a[k]-'A'] = true;
                    else
                        suspect[a[k]-'A'] = 1;
                    bits |= 1 << (a[k]-'A');
                }
                for (int k = 0; k < b.size(); ++k)
                {
                    if (suspect[b[k]-'A'] == 1)
                        excluded[b[k]-'A'] = true;
                    else
                        suspect[b[k]-'A'] = -1;
                    bits |= 1 << (b[k]-'A');
                }
                for (int j = 'A'; j <= 'L'; ++j)
                {
                    if (bits & (1 << (j-'A'))) continue;
                    excluded[j-'A'] = true;
                }
            }
        }
        int faker = 'A';
        for (int i = 'A'; i <= 'L'; ++i)
        {
            if (!excluded[i-'A'] && suspect[i-'A'])
            {
                faker = i;
                break;
            }
        }
        cout << (char)faker << " is the counterfeit coin and it is " << (suspect[faker-'A'] == 1 ? "light" : "heavy") << "." << endl;
    }

    return 0;
}