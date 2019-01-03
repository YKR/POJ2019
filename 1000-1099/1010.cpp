
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

vector<int> stamps;
vector<int> customers;

struct SingleAns
{
    int diversity;
    // int total;
    int highest;
    vector<int> seq;

    bool lessThan (const SingleAns & other) const
    {
        return diversity < other.diversity
            || diversity == other.diversity && (seq.size() > other.seq.size()
            || seq.size() == other.seq.size() && (highest < other.highest));
    }

    bool operator< (const SingleAns & other) const
    {
        return lessThan(other);
    }
    
    bool operator== (const SingleAns & other) const
    {
        return diversity == other.diversity && seq.size() == other.seq.size() && highest == other.highest;
    }
};

priority_queue<SingleAns> answers;
SingleAns curAns;

void search(int left, int curStamp, int diversity, int highest)
{
    if (left == 0)
    {
        curAns.diversity = diversity;
        curAns.highest = highest;
        answers.push(curAns);
        return;
    }
    if (curStamp == stamps.size())
    {
        return;
    }
    int stampsLeft = 4 - (int)curAns.seq.size();
    if (!answers.empty())
    {
        int maxDiversity = min(stampsLeft, (int)stamps.size() - curStamp);
        if (answers.top().diversity > maxDiversity + diversity)
            return;
        else if (answers.top().diversity == maxDiversity + diversity)
        {
            if (answers.top().seq.size() < curAns.seq.size() + maxDiversity)
                return;
            else if (answers.top().seq.size() == curAns.seq.size() + maxDiversity)
                if (answers.top().highest > highest)
                    return;
        }
    }
    if (stampsLeft * stamps[curStamp] < left) return;
    int maxStamps = min(left / stamps[curStamp], stampsLeft);
    for (int j = 0; j <= maxStamps; ++j) curAns.seq.push_back(stamps[curStamp]);
    for (int i = maxStamps; i >= 0; --i)
    {
        curAns.seq.pop_back();
        search(left - i * stamps[curStamp], curStamp + 1, diversity + (i != 0), i != 0 ? max(highest, stamps[curStamp]) : highest);
    }
}

int main()
{
    ios::sync_with_stdio(false);

    while (!cin.eof())
    {
        stamps.clear();
        customers.clear();
        int tmp;
        while (cin >> tmp, tmp)
            stamps.push_back(tmp);
        while (cin >> tmp, tmp)
            customers.push_back(tmp);
        sort(stamps.begin(), stamps.end(), greater<int>());
        for (int ccc = 0; ccc < customers.size(); ++ccc)
        {
            int c = customers[ccc];
            answers = priority_queue<SingleAns>();
            curAns.seq.clear();
            search(c, 0, 0, 0);
            if (!answers.empty())
            {
                SingleAns top = answers.top(); answers.pop();
                cout << c << " (" << top.diversity << "): ";
                if (answers.size() > 1 && top == answers.top())
                    cout << "tie" << endl;
                else
                {
                    for (int t = top.seq.size() - 1; t > 0; --t)
                        cout << top.seq[t] << " ";
                    cout << top.seq[0] << endl;
                }
            }
            else
            {
                cout << c << " ---- none" << endl;
            }
        }
    }

    return 0;
}