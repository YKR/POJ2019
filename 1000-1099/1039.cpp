
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

#define EPS 1e-6
#include <cfloat>

struct Coord
{
    double x, y;
    Coord(double _x, double _y) : x(_x), y(_y) {}
    Coord operator+(const Coord & other) const { return Coord(x + other.x, y + other.y); }
    bool operator==(const Coord & other) const { return fabs(x - other.x) < EPS && fabs(y - other.y) < EPS; }
    bool operator!=(const Coord & other) const { return !(*this == other); }
    operator bool() const { return !(x == -DBL_MAX || y == -DBL_MAX); }
};
const Coord InvalidCoord(-DBL_MAX, -DBL_MAX);
const Coord PipeCoordOffset(0.0, -1.0);

struct StraightLine
{
    double k, b;
    StraightLine(const Coord & p0, const Coord & p1)
    {
        k = (p0.y - p1.y) / (p0.x - p1.x);
        b = p0.y - k * p0.x;
    }
};

struct LineSegment
{
    double k, b;
    double xl, xr;
    LineSegment(const Coord & p0, const Coord & p1)
    {
        if (fabs(p0.x - p1.x) < EPS)
        {
            k = DBL_MAX;
            b = p0.x;
            xl = min(p0.y, p1.y);
            xr = max(p0.y, p1.y);
            return;
        }
        k = (p0.y - p1.y) / (p0.x - p1.x);
        b = p0.y - k * p0.x;
        xl = min(p0.x, p1.x);
        xr = max(p0.x, p1.x);
    }
};

Coord intersect(const StraightLine & l, const LineSegment & r)
{
    if (fabs(l.k - r.k) < EPS) return InvalidCoord;
    if (r.k == DBL_MAX)
    {
        double y = l.k * r.b + l.b;
        if (y < r.xl - EPS || y > r.xr + EPS) return InvalidCoord;
        return Coord(r.b, y);
    }
    Coord point((r.b - l.b) / (l.k - r.k), (r.b * l.k - l.b * r.k) / (l.k - r.k));
    if (point.x < r.xl - EPS || point.x > r.xr + EPS) return InvalidCoord;
    return point;
}

int n;
vector<Coord> pipeCoords;

Coord calcIntersectionWithPipe(const StraightLine & line, int k)
{
    LineSegment pipe0(pipeCoords[k - 1], pipeCoords[k]);
    Coord intersectPoint0(intersect(line, pipe0));
    if (intersectPoint0
        && (intersectPoint0 != pipeCoords[k - 1] || line.k > pipe0.k + EPS)
        && intersectPoint0 != pipeCoords[k])
    {
        return intersectPoint0;
    }
    LineSegment pipe1(pipeCoords[k - 1] + PipeCoordOffset, pipeCoords[k] + PipeCoordOffset);
    Coord intersectPoint1(intersect(line, pipe1));
    if (intersectPoint1
        && (intersectPoint1 != pipeCoords[k - 1] + PipeCoordOffset || line.k < pipe1.k - EPS)
        && intersectPoint1 != pipeCoords[k] + PipeCoordOffset)
    {
        return intersectPoint1;
    }
    return InvalidCoord;
}

void testLine(const StraightLine & line, int i, int j, Coord & maxIntersection, bool & through)
{
    if (intersect(line, LineSegment(pipeCoords[0], pipeCoords[0] + PipeCoordOffset)))
    {
        bool validLine = true;
        for (int k = 1; k <= i; ++k)
            if(calcIntersectionWithPipe(line, k))
            {
                validLine = false;
                break;
            }
        if (validLine)
        {
            bool intersected = false;
            for (int k = i + 1; k < n; ++k)
            {
                Coord intersection(calcIntersectionWithPipe(line, k));
                if (intersection)
                {
                    if (maxIntersection.x < intersection.x)
                        maxIntersection = intersection;
                    intersected = true;
                    break;
                }
            }
            if (!intersected)
                through = true;
        }
    }
}

int main()
{
    ios::sync_with_stdio(false);

    while (cin >> n, n != 0)
    {
        pipeCoords.clear();
        for (int i = 0; i < n; ++i)
        {
            double x, y;
            cin >> x >> y;
            pipeCoords.push_back(Coord(x, y));
        }
        Coord maxIntersection(-DBL_MAX, -DBL_MAX);
        bool through = false;
        for (int i = 1; i < n && !through; ++i)
        {
            for (int j = 0; j < i && !through; ++j)
            {
                testLine(StraightLine(pipeCoords[j], pipeCoords[i]), i, j, maxIntersection, through);
                if (through) break;
                testLine(StraightLine(pipeCoords[j], pipeCoords[i] + PipeCoordOffset), i, j, maxIntersection, through);
                if (through) break;
                testLine(StraightLine(pipeCoords[j] + PipeCoordOffset, pipeCoords[i]), i, j, maxIntersection, through);
                if (through) break;
                testLine(StraightLine(pipeCoords[j] + PipeCoordOffset, pipeCoords[i] + PipeCoordOffset), i, j, maxIntersection, through);
                if (through) break;
            }
        }
        if (through)
            cout << "Through all the pipe." << endl;
        else
            cout << fixed << setprecision(2) << maxIntersection.x << endl;
    }

    return 0;
}