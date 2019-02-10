
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

enum AgentStatus
{
    AGENT_ENTRY = 0,
    AGENT_IN_ROOM,
    AGENT_IN_ELEVATOR,
    AGENT_WAIT_ROOM,
    AGENT_WAIT_ELEVATOR,
    AGENT_EXIT,
};

int str2sec(const string & s)
{
    int hours = (s[0] - '0') * 10 + (s[1] - '0');
    int minutes = (s[3] - '0') * 10 + (s[4] - '0');
    int seconds = (s[6] - '0') * 10 + (s[7] - '0');
    return hours * 3600 + minutes * 60 + seconds;
}
string sec2str(int secs)
{
    int hours = secs / 3600;
    int minutes = secs % 3600 / 60;
    int seconds = secs % 60;
    string ret; ret.resize(8);
    ret[0] = hours / 10 + '0'; ret[1] = hours % 10 + '0'; ret[2] = ':';
    ret[3] = minutes / 10 + '0'; ret[4] = minutes % 10 + '0'; ret[5] = ':';
    ret[6] = seconds / 10 + '0'; ret[7] = seconds % 10 + '0';
    return ret;
}

struct Agent
{
    char name;
    vector<pair<int, int> > plan;
    ostringstream schedule;
    AgentStatus status;
    int curPos;
    int curPlan;
    int lastSec;
    Agent()
        : name(0)
        , plan()
        , schedule("", ios_base::app)
        , status(AGENT_EXIT)
        , curPos(0)
        , curPlan(0)
        , lastSec(0)
    {}
    Agent(const Agent & other)
        : name(other.name)
        , plan(other.plan)
        , schedule(other.schedule.str(), ios_base::app)
        , status(other.status)
        , curPos(other.curPos)
        , curPlan(other.curPlan)
        , lastSec(other.lastSec)
    {}
    Agent & operator= (const Agent & other)
    {
        name = other.name;
        plan = other.plan;
        schedule.str(other.schedule.str());
        status = other.status;
        curPos = other.curPos;
        curPlan = other.curPlan;
        lastSec = other.lastSec;
        return *this;
    }
    int plotCourse()
    {
        if (curPlan == plan.size()) // go to exit
        {
            if (curPos / 100 > 1)
            {
                if (curPos % 100 > 0) return curPos / 100 * 100;
                else return 100;
            }
            return 111;
        }
        int nexRoom = plan[curPlan].first;
        if (nexRoom / 100 != curPos / 100)
        {
            if (curPos % 100 == 0)
            {
                return nexRoom / 100 * 100;
            }
            return curPos / 100 * 100;
        }
        return nexRoom;
    }
};

struct BigBrother
{
    vector<Agent> agents;
    pair<int, int> inRoom[11][11];
    priority_queue<char, vector<char>, greater<char> > waitQueue[11][11];
    priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > timeline;
    BigBrother() { memset(inRoom, 0, sizeof(inRoom)); }
    void addAgent(const Agent & _agent, int startSec)
    {
        agents.push_back(_agent);
        Agent & curAgent = agents.back();
        curAgent.schedule << curAgent.name << endl;
        curAgent.status = AGENT_ENTRY;
        curAgent.curPos = 111;
        curAgent.curPlan = 0;
        curAgent.lastSec = startSec;
        timeline.push(make_pair(startSec, agents.size() - 1));
    }
    bool update()
    {
        if (timeline.empty()) return false;
        vector<int> pendingAgents;
        int curSec = timeline.top().first;
        while (!timeline.empty() && timeline.top().first == curSec)
        {
            pendingAgents.push_back(timeline.top().second);
            timeline.pop();
        }
        char queueHead[11][11];
        memset(queueHead, 0, sizeof(queueHead));
        for (int e = 0; e < pendingAgents.size(); ++e)
        {
            int curAgentId = pendingAgents[e];
            Agent & curAgent = agents[curAgentId];
            if (curAgent.status == AGENT_IN_ROOM)
                inRoom[curAgent.curPos / 100][curAgent.curPos % 100] = make_pair(0, 0);
        }
        for (int e = 0; e < pendingAgents.size(); ++e)
        {
            int curAgentId = pendingAgents[e];
            Agent & curAgent = agents[curAgentId];
            if (curAgent.status == AGENT_WAIT_ROOM
                && queueHead[curAgent.curPos / 100][curAgent.curPos % 100] == 0
                && inRoom[curAgent.curPos / 100][curAgent.curPos % 100].first == 0
                && waitQueue[curAgent.curPos / 100][curAgent.curPos % 100].top() == curAgent.name)
            {
                inRoom[curAgent.curPos / 100][curAgent.curPos % 100] = make_pair(curAgent.name, curSec + curAgent.plan[curAgent.curPlan].second);
                queueHead[curAgent.curPos / 100][curAgent.curPos % 100] = curAgent.name;
                waitQueue[curAgent.curPos / 100][curAgent.curPos % 100].pop();
            }
            else if (curAgent.status == AGENT_WAIT_ELEVATOR
                && queueHead[curAgent.curPos / 100][curAgent.curPos % 100] == 0
                && curSec % 5 == 0
                && waitQueue[curAgent.curPos / 100][curAgent.curPos % 100].top() == curAgent.name)
            {
                queueHead[curAgent.curPos / 100][curAgent.curPos % 100] = curAgent.name;
                waitQueue[curAgent.curPos / 100][curAgent.curPos % 100].pop();
            }
        }
        for (int e = 0; e < pendingAgents.size(); ++e)
        {
            int curAgentId = pendingAgents[e];
            Agent & curAgent = agents[curAgentId];
            int nexSec, nexPos;
            switch (curAgent.status)
            {
                case AGENT_ENTRY:
                    nexSec = curSec + 30;
                    curAgent.schedule << sec2str(curAgent.lastSec) << " " << sec2str(nexSec) << " "
                        << "Entry"
                        << endl;
                    curAgent.curPos = curAgent.plotCourse();
                    curAgent.status = curAgent.curPos % 100 == 0 ? AGENT_WAIT_ELEVATOR : AGENT_WAIT_ROOM;
                    waitQueue[curAgent.curPos / 100][curAgent.curPos % 100].push(curAgent.name);
                    curAgent.lastSec = nexSec;
                    break;
                case AGENT_IN_ROOM:
                    curAgent.schedule << sec2str(curAgent.lastSec) << " " << sec2str(curSec) << " "
                        << "Stay in room " << setw(4) << setfill('0') << curAgent.curPos
                        << endl;
                    ++curAgent.curPlan;
                    nexPos = curAgent.plotCourse();
                    if (nexPos == 111)
                    {
                        nexSec = curSec + 30;
                        curAgent.schedule << sec2str(curSec) << " " << sec2str(nexSec) << " "
                            << "Exit"
                            << endl;
                        curAgent.status = AGENT_EXIT;
                    }
                    else
                    {
                        nexSec = curSec + 10;
                        curAgent.schedule << sec2str(curSec) << " " << sec2str(nexSec) << " "
                            << "Transfer from room " << setw(4) << setfill('0') << curAgent.curPos << " to ";
                        if (nexPos % 100 != 0)
                            curAgent.schedule << "room " << setw(4) << setfill('0') << nexPos;
                        else
                            curAgent.schedule << "elevator";
                        curAgent.schedule << endl;
                        curAgent.curPos = nexPos;
                        curAgent.status = curAgent.curPos % 100 == 0 ? AGENT_WAIT_ELEVATOR : AGENT_WAIT_ROOM;
                        waitQueue[curAgent.curPos / 100][curAgent.curPos % 100].push(curAgent.name);
                        curAgent.lastSec = nexSec;
                    }
                    break;
                case AGENT_IN_ELEVATOR:
                    curAgent.schedule << sec2str(curAgent.lastSec) << " " << sec2str(curSec) << " "
                        << "Stay in elevator"
                        << endl;
                    nexPos = curAgent.plotCourse();
                    if (nexPos == 111)
                    {
                        nexSec = curSec + 30;
                        curAgent.schedule << sec2str(curSec) << " " << sec2str(nexSec) << " "
                            << "Exit"
                            << endl;
                        curAgent.status = AGENT_EXIT;
                    }
                    else
                    {
                        nexSec = curSec + 10;
                        curAgent.schedule << sec2str(curSec) << " " << sec2str(nexSec) << " "
                            << "Transfer from elevator to ";
                        if (nexPos % 100 != 0)
                            curAgent.schedule << "room " << setw(4) << setfill('0') << nexPos;
                        else
                            curAgent.schedule << "elevator";
                        curAgent.schedule << endl;
                        curAgent.curPos = nexPos;
                        curAgent.status = curAgent.curPos % 100 == 0 ? AGENT_WAIT_ELEVATOR : AGENT_WAIT_ROOM;
                        waitQueue[curAgent.curPos / 100][curAgent.curPos % 100].push(curAgent.name);
                        curAgent.lastSec = nexSec;
                    }
                    break;
                case AGENT_WAIT_ROOM:
                    nexSec = inRoom[curAgent.curPos / 100][curAgent.curPos % 100].second;
                    if (queueHead[curAgent.curPos / 100][curAgent.curPos % 100] == curAgent.name)
                    {
                        if (curAgent.lastSec != curSec)
                        {
                            curAgent.schedule << sec2str(curAgent.lastSec) << " " << sec2str(curSec) << " "
                                << "Waiting in front of room " << setw(4) << setfill('0') << curAgent.curPos
                                << endl;
                        }
                        curAgent.status = AGENT_IN_ROOM;
                        curAgent.lastSec = curSec;
                    }
                    break;
                case AGENT_WAIT_ELEVATOR:
                    if (curSec % 5 != 0)
                    {
                        nexSec = curSec / 5 * 5 + 5;
                        break;
                    }
                    if (queueHead[curAgent.curPos / 100][curAgent.curPos % 100] == curAgent.name)
                    {
                        if (curAgent.lastSec != curSec)
                        {
                            curAgent.schedule << sec2str(curAgent.lastSec) << " " << sec2str(curSec) << " "
                                << "Waiting in elevator queue"
                                << endl;
                        }
                        nexPos = curAgent.plotCourse();
                        nexSec = curSec + 30 * abs(nexPos / 100 - curAgent.curPos / 100);
                        curAgent.curPos = nexPos;
                        curAgent.status = AGENT_IN_ELEVATOR;
                        curAgent.lastSec = curSec;
                    }
                    else
                    {
                        nexSec = curSec + 5;
                    }
                    break;
                default:
                    break;
            }
            if (curAgent.status != AGENT_EXIT)
                timeline.push(make_pair(nexSec, curAgentId));
        }
        return true;
    }
};

BigBrother bob;
bool agentsOrderCompare(int a, int b)
{
    return bob.agents[a].name < bob.agents[b].name;
}

int main()
{
    ios::sync_with_stdio(false);

    Agent alice;
    while (cin >> alice.name, alice.name != '.')
    {
        string hms;
        cin >> hms;
        int startSec = str2sec(hms);
        int roomNumber, stayTime;
        alice.plan.clear();
        while (cin >> roomNumber, roomNumber != 0)
        {
            cin >> stayTime;
            alice.plan.push_back(make_pair(roomNumber,stayTime));
        }
        bob.addAgent(alice, startSec);
    }
    while (bob.update());
    int agentsOrder[30];
    for (int i = 0; i < bob.agents.size(); ++i)
        agentsOrder[i] = i;
    sort(agentsOrder, agentsOrder + bob.agents.size(), agentsOrderCompare);
    for (int i = 0; i < bob.agents.size(); ++i)
        cout << bob.agents[agentsOrder[i]].schedule.str() << endl;

    return 0;
}