
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

vector<string> dictionary;

int main()
{
    ios::sync_with_stdio(false);

    string str;
    while (cin >> str, str[0] != '#')
    {
        dictionary.push_back(str);
    }
    while (cin >> str, str[0] != '#')
    {
        bool matchExact = false;
        vector<int> replacements;
        for (int i = 0; i < dictionary.size(); ++i)
        {
            if (str == dictionary[i])
            {
                matchExact = true;
                break;
            }
            if (str.size() == dictionary[i].size() - 1)
            {
                bool matched = true;
                bool changed = false;
                for (int j = 0; j < str.size(); ++j)
                {
                    if (str[j] != dictionary[i][j + changed])
                    {
                        if (changed)
                        {
                            matched = false;
                            break;
                        }
                        changed = true;
                        --j;
                    }
                }
                if (matched)
                {
                    replacements.push_back(i);
                }
            }
            else if (str.size() == dictionary[i].size())
            {
                bool matched = true;
                bool changed = false;
                for (int j = 0; j < str.size(); ++j)
                {
                    if (str[j] != dictionary[i][j])
                    {
                        if (changed)
                        {
                            matched = false;
                            break;
                        }
                        changed = true;
                    }
                }
                if (matched)
                {
                    replacements.push_back(i);
                }
            }
            else if (str.size() == dictionary[i].size() + 1)
            {
                bool matched = true;
                bool changed = false;
                for (int j = 0; j < str.size(); ++j)
                {
                    if (str[j] != dictionary[i][j - changed])
                    {
                        if (changed)
                        {
                            matched = false;
                            break;
                        }
                        changed = true;
                    }
                }
                if (matched)
                {
                    replacements.push_back(i);
                }
            }
        }
        if (matchExact)
        {
            cout << str << " is correct" << endl;
        }
        else
        {
            cout << str << ':';
            for (int i = 0; i < replacements.size(); ++i)
            {
                cout << ' ' << dictionary[replacements[i]];
            }
            cout << endl;
        }
    }

    return 0;
}