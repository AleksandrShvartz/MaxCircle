#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include <vector>
#include <set>
#include <fstream>
#include <iostream>
#include <limits>
using namespace std;

const double EPS = 1E-10;
int steps = 100;
struct pt {
    double x, y;
    pt(double x, double y) : x(x), y(y) { }
};

struct Answer {
    double x, y, radius;
    Answer(double x, double y, double r):x(x),y(y),radius(r){}
};

struct Line {
    double a, b, c;
    Line(){}
    Line(double a, double b,double c):a(a),b(b),c(c){}
    Line(const pt& p, const pt& q) {
        a = p.y - q.y;
        b = q.x - p.x;
        c = -a * p.x - b * p.y;
        double z = sqrt(a * a + b * b);
        a /= z, b /= z, c /= z;
    }
};

double dist (double x, double y, Line & l) {
    return abs (x * l.a + y * l.b + l.c);
}

double radius (double x, double y, vector<Line> & l) {
    int n = (int) l.size();
    double res = std::numeric_limits<double>::max();
    for (int i=0; i<n; ++i)
        res = min (res, dist (x, y, l[i]));
    return res;
}

Answer y_radius (double x, vector<pt> & a, vector<Line> & l) {
    int n = (int) a.size();
    double ly = std::numeric_limits<double>::max(),  ry = std::numeric_limits<double>::min();;
    for (int i=0; i<n; ++i) {
        int x1 = a[i].x,  x2 = a[(i+1)%n].x,  y1 = a[i].y,  y2 = a[(i+1)%n].y;
        if (x1 == x2)  continue;
        if (x1 > x2)  swap (x1, x2),  swap (y1, y2);
        if (x1 <= x+EPS && x-EPS <= x2) {
            double y = y1 + (x - x1) * (y2 - y1) / (x2 - x1);
            ly = min (ly, y);
            ry = max (ry, y);
        }
    }
    for (int sy=0; sy< steps; ++sy) {
        double diff = (ry - ly) / 3;
        double y1 = ly + diff,  y2 = ry - diff;
        double f1 = radius (x, y1, l),  f2 = radius (x, y2, l);
        if (f1 < f2)
            ly = y1;
        else
            ry = y2;
    }
    return Answer(x, ly, radius (x, ly, l));
}

int main() {
    ifstream in("input.txt");
    ofstream out("output.txt");
    if (in.is_open()) {
        int n;
        in >> n;
        vector<pt> a;
        a.reserve(n);
        int x, y;
        for (int i = 0; i != n; ++i) {
            in >> x >> y;
            a.emplace_back(pt(x, y));
//            std::cout << '(' << x << ',' << y << ')' << '\n';
        }
        vector<Line> l(n);
        for (int i = 0; i < n; ++i) {
            l[i].a = a[i].y - a[(i + 1) % n].y;
            l[i].b = a[(i + 1) % n].x - a[i].x;
            double sq = sqrt(l[i].a * l[i].a + l[i].b * l[i].b);
            l[i].a /= sq, l[i].b /= sq;
            l[i].c = -(l[i].a * a[i].x + l[i].b * a[i].y);
        }
        double lx = std::numeric_limits<double>::max();
        double rx = std::numeric_limits<double>::min();
        for (int i = 0; i < n; ++i) {
            lx = min(lx, a[i].x);
            rx = max(rx, a[i].x);
        }

        for (int sx = 0; sx < steps; ++sx) {
            double diff = (rx - lx) / 3;
            double x1 = lx + diff, x2 = rx - diff;
            double f1 = y_radius(x1, a, l).radius;
            double f2 = y_radius(x2, a, l).radius;
            if (f1 < f2)
                lx = x1;
            else
                rx = x2;
        }

        Answer ans = y_radius(lx, a, l);
        if (out.is_open()) {
            out << ans.x<<" "<< ans.y<<" "<< ans.radius;
            out.close();
        }
        else {
            std::cout << "Error with output file\n";
        }
        in.close();
    }
    else {
        std::cout << "Error with input file\n";
    }
    return 0;
}
