
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

string sec2str(int sec)
{
    if (sec < 0) return "[InvalidTime]";
    int h = sec / 3600 % 24;
    int m = sec / 60 % 60;
    int s = sec % 60;
    string ret = "00:00:00";
    ret[0] = h / 10 + '0'; ret[1] = h % 10 + '0';
    ret[3] = m / 10 + '0'; ret[4] = m % 10 + '0';
    ret[6] = s / 10 + '0'; ret[7] = s % 10 + '0';
    return ret;
}
int str2sec(const string & str)
{
    int h = (str[0] - '0') * 10 + (str[1] - '0');
    int m = (str[3] - '0') * 10 + (str[4] - '0');
    int s = (str[6] - '0') * 10 + (str[7] - '0');
    return h * 3600 + m * 60 + s;
}

enum AgentStateType
{
    STATE_None,
    STATE_EntryExit,
    STATE_Stay,
    STATE_Transfer,
    STATE_Waiting
};

inline int RoomGetFloor(int roomNumber)
{
    return roomNumber == -1 ? 1 : roomNumber / 100;
}
inline int RoomGetId(int roomNumber)
{
    return roomNumber == -1 ? -1 : roomNumber % 100;
}
inline int MakeRoomNumber(int roomFloor, int roomId)
{
    if (roomFloor <= 0 || roomId < 0 || roomFloor > 10 || roomId > 10) return -1;
    return roomFloor * 100 + roomId;
}
inline int MakeElevatorNumber(int roomFloor)
{
    return MakeRoomNumber(roomFloor, 0);
}
inline bool IsElevator(int roomNumber)
{
    return RoomGetId(roomNumber) == 0;
}

struct Agent
{
    char identity;
    vector<pair<int, int> > plans;
    int planPtr;
    int state;
    int roomNumber;
    int enterSec;
    Agent() : plans(), planPtr(0), state(STATE_None),
        roomNumber(-1), enterSec(-1) {}
    int GetPlanTarget()
    {
        if (planPtr < 0) planPtr = 0;
        if (planPtr >= plans.size()) return -1;
        return plans[planPtr].first;
    }
    int GetPlanDuration()
    {
        if (planPtr < 0) planPtr = 0;
        if (planPtr >= plans.size()) return -1;
        return plans[planPtr].second;
    }
    int PlotCourse(int targetRoom)
    {
        if (RoomGetFloor(roomNumber) != RoomGetFloor(targetRoom))
        {
            if (IsElevator(roomNumber))
                return MakeElevatorNumber(RoomGetFloor(targetRoom));
            else
                return MakeElevatorNumber(RoomGetFloor(roomNumber));
        }
        else
        {
            return targetRoom;
        }
    }
    void ChangeState(ostringstream & o, int sec, int nextState, int nextRoom = -1)
    {
        if (!(sec == enterSec))
        {
            o << sec2str(enterSec) << ' ' << sec2str(sec) << ' ';
            switch (state)
            {
                case STATE_EntryExit:
                    if (nextState == STATE_None) o << "Exit";
                    else o << "Entry";
                    o << endl;
                    break;
                case STATE_Stay:
                    o << "Stay in ";
                    if (IsElevator(roomNumber)) o << "elevator";
                    else o << "room " << setw(4) << setfill('0') << roomNumber;
                    o << endl;
                    break;
                case STATE_Transfer:
                    o << "Transfer from ";
                    if (IsElevator(roomNumber)) o << "elevator";
                    else o << "room " << setw(4) << setfill('0') << roomNumber;
                    o << " to ";
                    if (IsElevator(nextRoom)) o << "elevator";
                    else o << "room " << setw(4) << setfill('0') << nextRoom;
                    o << endl;
                    break;
                case STATE_Waiting:
                    o << "Waiting in ";
                    if (IsElevator(roomNumber)) o << "elevator queue";
                    else o << "front of room " << setw(4) << setfill('0') << roomNumber;
                    o << endl;
                    break;
            }
        }
        state = nextState;
        if (nextState != STATE_Transfer) roomNumber = nextRoom;
        enterSec = sec;
        // HERE: Switch to next plan
        if (nextState == STATE_Stay && !IsElevator(roomNumber)) ++planPtr;
    }
    int GetExitStateSec(int staySecs = -1)
    {
        switch (state)
        {
            case STATE_None:
                return -1;
            case STATE_EntryExit:
                return enterSec + 30;
            case STATE_Stay:
                if (staySecs == -1) return -1;
                return enterSec + staySecs;
            case STATE_Transfer:
                return enterSec + 10;
            case STATE_Waiting:
                return -1;
        }
        return -1;
    }
};

struct BigBrother
{
    static ostringstream schedules[30];
    vector<Agent> agents;
    priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > timeline;
    bool roomsOccupied[12][12];
    priority_queue<pair<char, int>, vector<pair<char, int> >, greater<pair<char, int> > > roomsQueue[12][12];
    BigBrother() : agents(), timeline()
        { memset(roomsOccupied, 0, sizeof(roomsOccupied)); }
    bool Update()
    {
        if (timeline.empty()) return false;
        int curSec = timeline.top().first;
        list<int> curAgentIds;
        while (!timeline.empty() && timeline.top().first == curSec)
        {
            curAgentIds.push_back(timeline.top().second);
            timeline.pop();
        }
        for (list<int>::iterator it = curAgentIds.begin(); it != curAgentIds.end();)
        {
            int curAgentId = *it;
            Agent & curAgent = agents[curAgentId];
            if (curAgent.state == STATE_Transfer || curAgent.state == STATE_EntryExit)
            {
                int targetRoom = curAgent.PlotCourse(curAgent.GetPlanTarget());
                int nextState = STATE_Waiting;
                if (targetRoom == -1) nextState = STATE_None;
                curAgent.ChangeState(schedules[curAgentId], curSec, nextState, targetRoom);
                if (targetRoom != -1) roomsQueue[RoomGetFloor(targetRoom)][RoomGetId(targetRoom)].push(make_pair(curAgent.identity, curAgentId));
                int nextSec = curAgent.GetExitStateSec();
                if (nextSec != -1) timeline.push(make_pair(nextSec, curAgentId));
                list<int>::iterator tmp = it;
                ++it;
                curAgentIds.erase(tmp);
            }
            else ++it;
        }
        for (list<int>::iterator it = curAgentIds.begin(); it != curAgentIds.end();)
        {
            int curAgentId = *it;
            Agent & curAgent = agents[curAgentId];
            if (curAgent.state == STATE_Stay)
            {
                int curRoom = curAgent.roomNumber;
                int nextState = STATE_Transfer;
                if (RoomGetFloor(curRoom) == 1 && curAgent.GetPlanTarget() == -1)
                    nextState = STATE_EntryExit;
                curAgent.ChangeState(schedules[curAgentId], curSec, nextState);
                int nextSec = curAgent.GetExitStateSec();
                timeline.push(make_pair(nextSec, curAgentId));
                if (!IsElevator(curRoom)) roomsOccupied[RoomGetFloor(curRoom)][RoomGetId(curRoom)] = false;
                list<int>::iterator tmp = it;
                ++it;
                curAgentIds.erase(tmp);
            }
            else ++it;
        }
        curAgentIds.clear(); // Erase waiting agents; will reinsert below
        for (int fl = 1; fl <= 10; ++fl)
        {
            // Elevator
            if (!roomsQueue[fl][0].empty())
            {
                if (curSec % 5 == 0)
                {
                    curAgentIds.push_back(roomsQueue[fl][0].top().second);
                    roomsQueue[fl][0].pop();
                }
                if (!roomsQueue[fl][0].empty())
                {
                    timeline.push(make_pair(curSec / 5 * 5 + 5, roomsQueue[fl][0].top().second));
                }
            }
            // Rooms
            for (int rid = 1; rid <= 10; ++rid)
            {
                if (!roomsOccupied[fl][rid] && !roomsQueue[fl][rid].empty())
                {
                    curAgentIds.push_back(roomsQueue[fl][rid].top().second);
                    roomsQueue[fl][rid].pop();
                }
            }
        }
        for (list<int>::iterator it = curAgentIds.begin(); it != curAgentIds.end();)
        {
            int curAgentId = *it;
            Agent & curAgent = agents[curAgentId];
            // if (curAgent.state == STATE_Waiting) // Always true
            {
                int curRoom = curAgent.roomNumber;
                int targetRoom = curRoom;
                if (IsElevator(curRoom)) targetRoom = curAgent.PlotCourse(curAgent.GetPlanTarget());
                int planDuration = curAgent.GetPlanDuration();
                curAgent.ChangeState(schedules[curAgentId], curSec, STATE_Stay, targetRoom);
                int nextSec = curAgent.GetExitStateSec(IsElevator(targetRoom)
                    ? abs(RoomGetFloor(targetRoom) - RoomGetFloor(curRoom)) * 30
                    : planDuration);
                timeline.push(make_pair(nextSec, curAgentId));
                if (!IsElevator(targetRoom)) roomsOccupied[RoomGetFloor(targetRoom)][RoomGetId(targetRoom)] = true;
                list<int>::iterator tmp = it;
                ++it;
                curAgentIds.erase(tmp);
            }
            //else ++it;
        }
        return true;
    }
};
ostringstream BigBrother::schedules[30];

BigBrother bb;
bool AgentLexicalLess(int a, int b)
{
    return bb.agents[a].identity < bb.agents[b].identity;
}

int main()
{
    ios::sync_with_stdio(false);

    string agentCode, beginTime;
    while (cin >> agentCode, agentCode[0] != '.')
    {
        bb.agents.push_back(Agent());
        int curAgentId = bb.agents.size() - 1;
        Agent & curAgent = bb.agents.back();
        curAgent.identity = agentCode[0];
        cin >> beginTime;
        int beginSec = str2sec(beginTime);
        curAgent.state = STATE_EntryExit;
        curAgent.roomNumber = -1;
        curAgent.enterSec = beginSec;
        bb.timeline.push(make_pair(curAgent.GetExitStateSec(), curAgentId));
        int roomNumber, duration;
        while (cin >> roomNumber, roomNumber != 0)
        {
            cin >> duration;
            curAgent.plans.push_back(make_pair(roomNumber, duration));
        }
    }
    while (bb.Update());
    int agentOrder[30];
    for (int i = 0; i < bb.agents.size(); ++i)
        agentOrder[i] = i;
    sort(agentOrder, agentOrder + bb.agents.size(), AgentLexicalLess);
    for (int i = 0; i < bb.agents.size(); ++i)
    {
        cout << bb.agents[agentOrder[i]].identity << endl;
        cout << BigBrother::schedules[agentOrder[i]].str() << endl;
    }

    return 0;
}