
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

#define OUTPUT_DEBUG(TARGET) cerr << #TARGET << " == " << TARGET << endl;

struct SegmentTreeNode
{
    pair<int, int> range;
    bool even;
    int value;
};

struct SegmentTree
{
    vector<SegmentTreeNode> nodes;

    SegmentTree() : nodes()
    {
        nodes.push_back(SegmentTreeNode());
        nodes.push_back(SegmentTreeNode());
    }

    void setRange(const pair<int, int> & range)
    {
        int numElements = range.second - range.first;
        if (numElements < 1) return;
        int nodesSize = numElements << 1;
        nodes.resize(nodesSize);
        for (int i = 0; i < nodes.size(); ++i)
        {
            nodes[i].range = make_pair(range.first - 1, range.first - 1);
            nodes[i].even = false;
            nodes[i].value = 0;
        }
        nodes[1].range = range;
        nodes[1].even = true;
        nodes[1].value = 0;
    }

    void recurAdd(int curNode, const pair<int, int> & adding)
    {
        if (nodes[curNode].range == adding)
        {
            ++nodes[curNode].value;
            return;
        }
        nodes[curNode].even = false;
        int mid = (nodes[curNode].range.first + nodes[curNode].range.second) >> 1;
        pair<int, int> lRange = make_pair(nodes[curNode].range.first, mid);
        pair<int, int> rRange = make_pair(mid, nodes[curNode].range.second);
        int lIndex = curNode << 1;
        int rIndex = lIndex + 1;
        if (nodes[lIndex].range != lRange)
        {
            nodes[lIndex].range = lRange;
            nodes[lIndex].even = true;
            nodes[lIndex].value = 0;
        }
        if (nodes[rIndex].range != rRange)
        {
            nodes[rIndex].range = rRange;
            nodes[rIndex].even = true;
            nodes[rIndex].value = 0;
        }
        if (adding.first < lRange.second)
        {
            recurAdd(lIndex, make_pair(adding.first, min(lRange.second, adding.second)));
        }
        if (adding.second > rRange.first)
        {
            recurAdd(rIndex, make_pair(max(rRange.first, adding.first), adding.second));
        }
    }

    void recurQuery(int curNode, int targetValue, vector<pair<int, int> > & ranges)
    {
        if (nodes[curNode].even)
        {
            if (nodes[curNode].value == targetValue)
            {
                if (!ranges.empty() && ranges.back().second >= nodes[curNode].range.first)
                    ranges.back().second = nodes[curNode].range.second;
                else
                    ranges.push_back(nodes[curNode].range);
            }
            return;
        }
        int lIndex = curNode << 1;
        int rIndex = lIndex + 1;
        if (nodes[curNode].value)
        {
            nodes[lIndex].value += nodes[curNode].value;
            nodes[rIndex].value += nodes[curNode].value;
            nodes[curNode].value = 0;
        }
        recurQuery(lIndex, targetValue, ranges);
        recurQuery(rIndex, targetValue, ranges);
        if (nodes[lIndex].even && nodes[rIndex].even && nodes[lIndex].value == nodes[rIndex].value)
        {
            nodes[curNode].even = true;
            nodes[curNode].value += nodes[lIndex].value;
            nodes[lIndex].value = nodes[rIndex].value = 0;
        }
    }

    void rangeAdd(const pair<int, int> & adding)
    {
        if (adding.first >= nodes[1].range.second
            || adding.second <= nodes[1].range.first)
            return;
        pair<int, int> trimmedAdding = make_pair(max(nodes[1].range.first, adding.first), min(nodes[1].range.second, adding.second));
        recurAdd(1, trimmedAdding);
    }
    
    void rangeQuery(int targetValue, vector<pair<int, int> > & querying)
    {
        recurQuery(1, targetValue, querying);
    }
};

struct XRange
{
    int x;
    int k;
    int kAdd;
    int leftAdd;
    int rightAdd;
    pair<int, int> range;
    void calcRange()
    {
        int base = x * (k + kAdd);
        range.first = base + leftAdd;
        range.second = base + rightAdd;
    }
    void setX(int _x)
    {
        x = _x;
        calcRange();
    }
    XRange(int _x, int _k, int _ka, int _la, int _ra)
        : k(_k), kAdd(_ka), leftAdd(_la), rightAdd(_ra)
    {
        setX(_x);
    }
    bool operator< (const XRange & other) const
    {
        return range < other.range;
    }
};

bool verify(int m, int k)
{
    bool gameOver[30];
    memset(gameOver, 0, sizeof(gameOver));
    int overCount = 0, curPos = 0;
    while (overCount < k)
    {
        int targetMove = (m - 1) % ((k << 1) - overCount) + 1;
        int moveCount = 0;
        while ((moveCount += gameOver[curPos] ? 0 : 1) < targetMove)
            curPos = (curPos + 1) % (k << 1);
        if (curPos < k) return false;
        gameOver[curPos] = true;
        ++overCount;
    }
    return true;
}

int main()
{
    ios::sync_with_stdio(false);

    int f[20] = {0};

    cout << "int f[20] = { 0, 2, ";

    for (int k = 2; k < 14; ++k)
    {
        vector<XRange> ranges;
        for (int i = 1; i < k; ++i)
            ranges.push_back(XRange(1, k, i, -(i - 1), i + 1));
        ranges.push_back(XRange(1, k, k, -k + 1, 1));
        SegmentTree st;
        int searchStep = 8192;
        int searchRangeL = 0;
        while (f[k] == 0)
        {
            st.setRange(make_pair(searchRangeL, searchRangeL + searchStep));
            for (int i = 0; i < ranges.size(); ++i)
            {
                while (ranges[i].range.second < searchRangeL + searchStep)
                {
                    st.rangeAdd(ranges[i].range);
                    ranges[i].setX(ranges[i].x + 1);
                }
                st.rangeAdd(ranges[i].range);
            }
            vector<pair<int, int> > validRanges;
            st.rangeQuery(k, validRanges);
            for (int i = 0; i < validRanges.size(); ++i)
            {
                for (int m = validRanges[i].first; m <= validRanges[i].second && f[k] == 0; ++m)
                {
                    if (verify(m, k))
                        f[k] = m;
                }
                if (f[k]) break;
            }
            searchRangeL += searchStep;
        }
        cout << f[k] << (k == 13 ? " };\n" : ", ") << endl;
    }

    return 0;
}