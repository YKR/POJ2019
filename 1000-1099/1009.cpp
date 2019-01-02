
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

typedef pair<int, int> rlePair;
typedef vector<rlePair> rleImage;

int w, h;
rleImage src;
rleImage ret;

struct ImageRow
{
    int rowNumber;
    int stIndex;
    int stOffset;
    int enIndex;
    int enOffset;

    /// pixelNumber[rowNumber] should be in [rle[stIndex]+stOffset, rle[enIndex]+enOffset)
    
    ImageRow() : rowNumber(-1), stIndex(0), stOffset(0), enIndex(0), enOffset(0) {}

    void CalcEnBasedOnSt()
    {
        enIndex = stIndex;
        enOffset = stOffset;
        int nextRowPixelNumber = (rowNumber + 1) * w;
        int accumPixelNumber = rowNumber * w - stOffset;
        while (enIndex < src.size() && src[enIndex].second + accumPixelNumber < nextRowPixelNumber)
        {
            accumPixelNumber += src[enIndex].second;
            ++enIndex;
        }
        enOffset = nextRowPixelNumber - accumPixelNumber;
    }

    void StepToNextRow()
    {
        stIndex = enIndex;
        stOffset = enOffset;
        if (stOffset >= src[stIndex].second)
        {
            stOffset -= src[stIndex].second;
            ++stIndex;
        }
        ++rowNumber;
        if (rowNumber >= h)
            return;
        CalcEnBasedOnSt();
    }

    void StepForwardToRow(int targetRowNumber)
    {
        int targetPixelNumber = targetRowNumber * w;
        int accumPixelNumber = rowNumber * w - stOffset;
        rowNumber = targetRowNumber;
        if (rowNumber >= h)
            return;
        while (stIndex < src.size() && src[stIndex].second + accumPixelNumber <= targetPixelNumber)
        {
            accumPixelNumber += src[stIndex].second;
            ++stIndex;
        }
        stOffset = targetPixelNumber - accumPixelNumber;
        CalcEnBasedOnSt();
    }

    void StepToRow(int targetRowNumber)
    {
        if (targetRowNumber == rowNumber)
            return;
        if (targetRowNumber == rowNumber + 1)
        {
            StepToNextRow();
            return;
        }
        if (targetRowNumber > rowNumber)
        {
            StepForwardToRow(targetRowNumber);
            return;
        }
        rowNumber = 0;
        stIndex = 0;
        stOffset = 0;
        StepForwardToRow(targetRowNumber);
    }
};

int GetIndex(int rowNumber, int rowPixelCursor, int cIndex, int cOffset, int direction)
{
    if (rowNumber < 0 || rowNumber >= h) return -1;
    if (rowPixelCursor == 0 && direction < 0)
    {
        return -1;
    }
    if (rowPixelCursor + 1 == w && direction > 0)
    {
        return -1;
    }
    if (cOffset == 0 && direction < 0)
        return cIndex - 1;
    if (cOffset + 1 == src[cIndex].second && direction > 0)
        return cIndex + 1;
    return cIndex;
}

int GetDiff(int aIndex, int bIndex)
{
    if (aIndex < 0 || aIndex >= src.size()
        || bIndex < 0 || bIndex >= src.size())
        return 0;
    return abs(src[aIndex].first - src[bIndex].first);
}

void PushBackAns(rlePair rlep)
{
    if (!ret.empty() && ret.back().first == rlep.first)
        ret.back().second += rlep.second;
    else
        ret.push_back(rlep);
}

void CalcRows(ImageRow *rows)
{
    if (rows[1].rowNumber < 0 || rows[1].rowNumber >= h)
    {
        return;
    }
    int rowPixelCursor = 0;
    int rleIndexCursor[3] = {rows[0].stIndex, rows[1].stIndex, rows[2].stIndex};
    int rleOffsetCursor[3] = {rows[0].stOffset, rows[1].stOffset, rows[2].stOffset};
    while (rowPixelCursor < w)
    {
        int nextCriticalPoint = w;
        for (int i = 0; i < 3; ++i)
        {
            if (rows[i].rowNumber >= 0 && rows[i].rowNumber < h)
                nextCriticalPoint = min(nextCriticalPoint, src[rleIndexCursor[i]].second - rleOffsetCursor[i] + rowPixelCursor);
        }
        ///     rpc       ncp  /// rpc == rowPixelCursor, ncp == nextCriticalPoint
        ///      v         v   /// (arrow)
        /// 0: ? ? ? ... ? ?   /// ? == pixel or void
        /// 1: ? o o ... o|?   /// o == pixel
        /// 2: ? ? ? ... ? ?   /// | == end of rlePair
        /// [TARGET] calculate (1, rpc); (1, rpc+1) == (1, rpc+2) == ... == (1, ncp-2); (1, ncp-1);
        int maxdiff;
        /// --- BEGIN (1, rpc) ---
        maxdiff = 0;
        for (int i = 0; i < 3; ++i)
        {
            for (int j = -1; j <= 1; ++j)
            {
                if (i == 1 && j == 0) continue;
                int diff = GetDiff(rleIndexCursor[1],
                    GetIndex(rows[i].rowNumber, rowPixelCursor, rleIndexCursor[i], rleOffsetCursor[i], j));
                maxdiff = max(maxdiff, diff);
            }
        }
        PushBackAns(make_pair(maxdiff, 1));
        /// --- BEGIN (1, rpc+1) ~ (1, ncp-2) ---
        if (nextCriticalPoint - 2 >= rowPixelCursor + 1)
        {
            maxdiff = 0;
            for (int i = 0; i < 3; ++i)
            {
                if (i == 1) continue;
                if (!(rows[i].rowNumber >= 0 && rows[i].rowNumber < h)) continue;
                int diff = GetDiff(rleIndexCursor[1],
                    rleIndexCursor[i]);
                maxdiff = max(maxdiff, diff);
            }
            PushBackAns(make_pair(maxdiff, nextCriticalPoint - rowPixelCursor - 2));
        }
        /// --- BEGIN (1, ncp-1) ---
        if (nextCriticalPoint - 1 >= rowPixelCursor + 1) // else: ncp-1 == rpc, already calculated in (1, rpc)
        {
            maxdiff = 0;
            for (int i = 0; i < 3; ++i)
            {
                int curOffset = rleOffsetCursor[i] + nextCriticalPoint - rowPixelCursor - 1;
                for (int j = -1; j <= 1; ++j)
                {
                    if (i == 1 && j == 0) continue;
                    int diff = GetDiff(rleIndexCursor[1],
                        GetIndex(rows[i].rowNumber, nextCriticalPoint - 1, rleIndexCursor[i], curOffset, j));
                    maxdiff = max(maxdiff, diff);
                }
            }
            PushBackAns(make_pair(maxdiff, 1));
        }
        /// --- UPDATE rpc ---
        for (int i = 0; i < 3; ++i)
        {
            if (rows[i].rowNumber >= 0 && rows[i].rowNumber < h)
            {
                rleOffsetCursor[i] += nextCriticalPoint - rowPixelCursor;
                if (rleOffsetCursor[i] >= src[rleIndexCursor[i]].second)
                {
                    rleOffsetCursor[i] -= src[rleIndexCursor[i]].second;
                    ++rleIndexCursor[i];
                }
            }
        }
        rowPixelCursor = nextCriticalPoint;
    }
}

int main()
{
    ios::sync_with_stdio(false);

    while (cin >> w, w != 0)
    {
        src.clear();
        ret.clear();

        int area = 0;
        int a, b;
        while (cin >> a >> b, !(a == 0 && b == 0))
        {
            if (b == 0) continue;
            src.push_back(make_pair(a, b));
            area += b;
        }
        h = area / w;
        ImageRow rows[3];
        rows[2].StepToNextRow(); rows[1] = rows[2];
        rows[2].StepToNextRow();
        while (rows[1].rowNumber < h)
        {
            CalcRows(rows);
            /// skip rows
            /// 0: b b ... a a
            /// 1: a a ... a a
            /// 2: a a ... a a  /// all zero
            ///    a a ... a a  /// all zero
            ///    a a ... a a
            ///    a a ... b b  <- curIndexEndRow
            if (rows[2].rowNumber >= 0 && rows[2].rowNumber < h
                && rows[1].stIndex == rows[2].enIndex)
            {
                int curIndexEndRow = rows[1].rowNumber + (src[rows[1].stIndex].second - rows[1].stOffset) / w;
                int blankRows = curIndexEndRow - rows[2].rowNumber - 1;
                if (blankRows > 0)
                {
                    PushBackAns(make_pair(0, blankRows * w));
                    rows[1].StepForwardToRow(curIndexEndRow - 2);
                    rows[2] = rows[1]; rows[2].StepToNextRow();
                }
            }
            rows[0] = rows[1];
            rows[1] = rows[2];
            rows[2].StepToNextRow();
        }
        cout << w << endl;
        for (int i = 0; i < ret.size(); ++i)
            cout << ret[i].first << " " << ret[i].second << endl;
        cout << "0 0" << endl;
    }
    cout << "0" << endl;

    return 0;
}