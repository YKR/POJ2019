
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

    vector<string> pages;
    pages.push_back("http://www.acm.org/");
    int pagePtr = 0;
    string command;
    while (cin >> command, command != "QUIT")
    {
        bool ignored = false;
        if (command == "VISIT")
        {
            string page;
            cin >> page;
            if (pages.size() > pagePtr + 1)
                pages.resize(pagePtr + 1);
            pages.push_back(page);
            ++pagePtr;
        }
        else if (command == "BACK")
        {
            if (pagePtr > 0) --pagePtr;
            else ignored = true;
        }
        else // if (command == "FORWARD")
        {
            if (pagePtr + 1 < pages.size()) ++pagePtr;
            else ignored = true;
        }
        if (ignored) cout << "Ignored" << endl;
        else cout << pages[pagePtr] << endl;
    }

    return 0;
}