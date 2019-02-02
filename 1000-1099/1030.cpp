
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

typedef list<vector<int> > RankList;

void OUTPUT_DEBUG_RANKLIST(const RankList & rl)
{
    cerr << "Contest ranks are listed below." << endl;
    for (RankList::const_iterator it = rl.begin(); it != rl.end(); ++it)
    {
        for (int i = 0; i < it->size(); ++i)
        {
            cerr << (*it)[i] << ' ';
        }
        cerr << endl;
    }
}

RankList contests[2], overall;
int place[110][2];

int main()
{
    ios::sync_with_stdio(false);

    memset(place, -1, sizeof(place));
    for (int c = 0; c < 2; ++c)
    {
        int n;
        cin >> n;
        string tmp;
        getline(cin, tmp);
        int accumPlace = 0;
        for (int i = 0; i < n; ++i)
        {
            string line;
            getline(cin, line);
            int nonblank = line.size();
            while (nonblank > 0 && line[nonblank - 1] == ' ')
                --nonblank;
            if (nonblank == 0)
            {
                --i;
                continue;
            }
            line.resize(nonblank);
            istringstream lineIn;
            lineIn.str(line);
            contests[c].push_back(vector<int>());
            int numIds = 0;
            while (!lineIn.eof())
            {
                int id;
                lineIn >> id;
                contests[c].back().push_back(id);
                place[id][c] = accumPlace;
                ++numIds;
            }
            accumPlace += numIds;
        }
    }
    // rule 1, 2
    map<int, vector<int> > overallOfRule12A;
    for (int id = 1; id <= 100; ++id)
        if (place[id][0] != -1 && place[id][1] != -1)
            overallOfRule12A[place[id][0] + place[id][1]].push_back(id);
    // rule A
    vector<bool> arranged[2];
    for (int c = 0; c < 2; ++c)
    {
        for (RankList::iterator it = contests[c].begin(); it != contests[c].end(); ++it)
        {
            arranged[c].push_back(false);
            int ruleAKey = -1;
            for (int i = 0; i < it->size(); ++i)
            {
                int id = (*it)[i];
                if (place[id][0] != -1 && place[id][1] != -1)
                {
                    if (ruleAKey == -1)
                        ruleAKey = place[id][0] + place[id][1];
                    else if (ruleAKey != place[id][0] + place[id][1])
                    {
                        // ambigious
                        ruleAKey = -1;
                        arranged[c].back() = true;
                        break;
                    }
                }
            }
            if (ruleAKey != -1)
            {
                for (int i = 0; i < it->size(); ++i)
                {
                    int id = (*it)[i];
                    if (place[id][0] != -1 && place[id][1] != -1) continue;
                    overallOfRule12A[ruleAKey].push_back(id);
                }
                arranged[c].back() = true;
            }
        }
    }
    // rule B
    for (map<int, vector<int> >::iterator it = overallOfRule12A.begin(); it != overallOfRule12A.end(); ++it)
    {
        overall.push_back(it->second);
    }
    bool contestTeams[2][110];
    memset(contestTeams, 0, sizeof(contestTeams));
    for (int c = 0; c < 2; ++c)
        for (RankList::iterator it = contests[c].begin(); it != contests[c].end(); ++it)
            for (int i = 0; i < it->size(); ++i)
                contestTeams[c][(*it)[i]] = true;
    bool contestTeamsBefore[2][110];
    memset(contestTeamsBefore, 0, sizeof(contestTeamsBefore));
    int contestsPtr[2] = {0, 0};
    RankList::iterator its[2] = {contests[0].begin(), contests[1].begin()};
    while (1)
    {
        for (int c = 0; c < 2; ++c)
            while (contestsPtr[c] < contests[c].size() && arranged[c][contestsPtr[c]])
            {
                for (int i = 0; i < its[c]->size(); ++i)
                {
                    contestTeamsBefore[c][(*its[c])[i]] = true;
                }
                ++its[c];
                ++contestsPtr[c];
            }
        if (its[0] == contests[0].end() && its[1] == contests[1].end()) break;
        bool validIter[2] = {false, false};
        RankList::iterator oits[2];
        for (int c = 0; c < 2; ++c)
            if (contestsPtr[c] < contests[c].size()
                && (contestsPtr[1 - c] == contests[1 - c].size()
                    || place[its[c]->front()][c] <= place[its[1 - c]->front()][1 - c]))
            {
                bool overallTeamsBefore[110];
                memset(overallTeamsBefore, 0, sizeof(overallTeamsBefore));
                for (oits[c] = overall.begin(); oits[c] != overall.end(); ++oits[c])
                {
                    bool hitNonexistElement = false;
                    for (int i = 0; i < oits[c]->size(); ++i)
                        if (contestTeams[c][(*oits[c])[i]]
                            && !contestTeamsBefore[c][(*oits[c])[i]])
                        {
                            hitNonexistElement = true;
                            break;
                        }
                    if (hitNonexistElement) break;
                    for (int i = 0; i < oits[c]->size(); ++i)
                        overallTeamsBefore[(*oits[c])[i]] = true;
                }
                bool validOverallPos = true;
                for (RankList::iterator oit2 = oits[c]; oit2 != overall.end(); ++oit2)
                {
                    for (int i = 0; i < oit2->size(); ++i)
                        if (contestTeamsBefore[c][(*oit2)[i]])
                        {
                            validOverallPos = false;
                            break;
                        }
                    if (!validOverallPos) break;
                }
                validIter[c] = validOverallPos;
            }
        if (validIter[0] && validIter[1]
            && oits[0] == oits[1])
        {
            RankList::iterator overallEntry = overall.insert(oits[0], vector<int>());
            for (int c = 0; c < 2; ++c)
            {
                for (int i = 0; i < its[c]->size(); ++i)
                    overallEntry->push_back((*its[c])[i]);
            }
        }
        else
        {
            for (int c = 0; c < 2; ++c)
                if (validIter[c])
                {
                    overall.insert(oits[c], (*its[c]));
                }
        }
        for (int c = 0; c < 2; ++c)
            if (contestsPtr[c] < contests[c].size()
                && (contestsPtr[1 - c] == contests[1 - c].size()
                    || place[its[c]->front()][c] <= place[its[1 - c]->front()][1 - c]))
            {
                arranged[c][contestsPtr[c]] = true;
            }
    }
    for (RankList::iterator it = overall.begin(); it != overall.end(); ++it)
    {
        sort(it->begin(), it->end());
        for (int i = 0; i < it->size(); ++i)
            cout << (*it)[i] << (i == it->size() - 1 ? '\n' : ' ');
    }

    return 0;
}