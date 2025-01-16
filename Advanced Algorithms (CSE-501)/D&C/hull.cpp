#include<bits/stdc++.h>
using namespace std;

// Function to check whether the line is crossing the polygon or not
int orientation(pair<int, int> a, pair<int, int> b, pair<int, int> c) {
    int r = (b.second-a.second)*(c.first-b.first) - (c.second-b.second)*(b.first-a.first);
    return (r==0 ? 0 : r>0 ? 1 : -1);
}

vector<pair<int, int>> merge(vector<pair<int, int> > c1, vector<pair<int, int> > c2){
    int n1 = c1.size(), n2 = c2.size();
    int ia = 0, ib = 0;

    // // ia -> leftmost point of a
    for (int i=1; i<n1; i++)
        if (c1[i].first > c1[ia].first)
            ia = i;
    
    // ib -> leftmost point of b
    for (int i=1; i<n2; i++)
        if (c2[i].first < c2[ib].first)
            ib=i;
    
    // finding the upper tangent
    int inda = ia, indb = ib;
    bool done = 0;
    while (!done) {
        done = 1;
        while (orientation(c2[indb], c1[inda], c1[(inda+1)%n1]) >=0)
            inda = (inda + 1) % n1;
        while (orientation(c1[inda], c2[indb], c2[(n2+indb-1)%n2]) <=0) {
            indb = (n2+indb-1)%n2;
            done = 0;
        }
    }
    int uppera = inda, upperb = indb;
    
    //finding the lower tangent
    inda = ia, indb=ib, done = 0;
    int g = 0;
    while (!done) {
        done = 1;
        while (orientation(c1[inda], c2[indb], c2[(indb+1)%n2])>=0)
            indb=(indb+1)%n2;
        while (orientation(c2[indb], c1[inda], c1[(n1+inda-1)%n1])<=0) {
            inda=(n1+inda-1)%n1;
            done=0;
        }
    }
    int lowera = inda, lowerb = indb;
    
    vector<pair<int, int>> ret;
    int ind = uppera;
    ret.push_back(c1[uppera]);
    while (ind != lowera) {
        ind = (ind+1)%n1;
        ret.push_back(c1[ind]);
    }
    ind = lowerb;
    ret.push_back(c2[lowerb]);
    while (ind != upperb) {
        ind = (ind+1)%n2;
        ret.push_back(c2[ind]);
    }
    return ret;
}

// Brute force algorithm to find convex hull for a set of less than 6 points
vector<pair<int, int>> bruteHull(vector<pair<int, int>> c) {
    set<pair<int, int> >s;
    for (int i=0; i<c.size(); i++) {
        for (int j=i+1; j<c.size(); j++) {
            int x1 = c[i].first, y1 = c[i].second, x2 = c[j].first, y2 = c[j].second;
            // line a1*x + b1*y + c = 0
            // (x-x1)/(x-x2) = (y-y1)/(y-y2)
            // (y1-y2)*x + (x2-x1)*y + (x1*y2-x2*y1) = 0
            int a1 = y1-y2, b1 = x2-x1;
            int c1 = x1*y2-y1*x2;
            int pos = 0, neg = 0;
            for (int k=0; k<c.size(); k++) {
                if (a1*c[k].first+b1*c[k].second+c1 <= 0)
                    neg++; // point lies on or below the line
                if (a1*c[k].first+b1*c[k].second+c1 >= 0)
                    pos++; // point lies on or above the line
            }
            if (pos == c.size() || neg == c.size())  // all the points are on the same side
                s.insert(c[i]), s.insert(c[j]);
        }
    }
    vector<pair<int, int>>ret;
    for (auto e:s)
        ret.push_back(e);

    int n = ret.size();     
    // Calculate the center of the points
    double centerX = 0.0, centerY = 0.0;
    for (const auto& point : ret)
        centerX += point.first, centerY += point.second;

    centerX /= n; centerY /= n;

    // Calculate the angles of each point with respect to the center
    sort(ret.begin(), ret.end(), [&](const auto& a, const auto& b) {
        double angleA = atan2(a.second - centerY, a.first - centerX);
        double angleB = atan2(b.second - centerY, b.first - centerX);
        return angleA < angleB;
    });
    return ret;
}

// Function to find the convex hull for a set of points
vector<pair<int, int>> convexHull(vector<pair<int, int>> points) {
    int n = points.size();
    if (n <= 3)
        return bruteHull(points);
    
    int mid = n/2;
    vector<pair<int, int>> left(points.begin(), points.begin()+mid);
    vector<pair<int, int>> right(points.begin()+mid, points.end());

    vector<pair<int, int>> leftHull = convexHull(left);
    vector<pair<int, int>> rightHull = convexHull(right);
    
    return merge(leftHull, rightHull);
}

int main() {
    vector<pair<int, int>> points = {{-1, -3}, {-2, -2}, {-1, 0}, {-2, -1}, {-1, 1}, {0, 0}, {1, 1}, {2, 3}, {3, 0}, {2, -1}, {1, -3}};
    sort(points.begin(), points.end());
    vector<pair<int, int>> ans = convexHull(points);
    cout << "The points in the convex hull are:\n";
    for (auto e:ans)
      cout << e.first << " " << e.second << endl;
    return 0;
}