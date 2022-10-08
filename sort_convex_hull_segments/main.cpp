#include <CGAL/Simple_cartesian.h>
#include <bits/stdc++.h>
#include <cmath>
#include <vector>

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_2 Point;
typedef Kernel::Segment_2 Segment;

using namespace std;

bool sortdescending(const pair<double, Segment> &s_0,
                    const pair<double, Segment> &s_1) {
  return s_0.first > s_1.first;
}

vector<Segment> SortConvexHullSegments(const vector<Segment> &E) {
  vector<Segment> E_new = E;
  // Find inverse of each segment and add to E
  for (Segment s : E) {
    E_new.push_back(s.opposite());
  }

  // Create vector to store sorted segments
  vector<Segment> E_sorted;

  // Create vector to store directed segment angles A
  vector<pair<double, Segment>> A;
  Point uppermost_vertex(INT_MIN, INT_MIN);
  for (Segment s : E_new) {
    // compute arctan(y/x) to find first angle
    int y = s.target().y() - s.source().y();
    int x = s.target().x() - s.source().x();
    // add 2pi and mod 2pi to avoid negative angles
    double angle = fmod(atan2(y, x) + 2 * M_PI, 2 * M_PI);
    // store angle/segment in A
    A.push_back(pair<double, Segment>(angle, s));
    // keep track of vertex with highest y value
    if (s.source().y() > uppermost_vertex.y())
      uppermost_vertex = s.source();
    else if (s.target().y() > uppermost_vertex.y())
      uppermost_vertex = s.target();
  }

  // Sort A descending
  sort(A.begin(), A.end(), sortdescending);

  // In A start at angle associated with uppermost vertex
  for (auto a : A) {
    // If we haven't found segment with uppermost point yet don't do anything
    if (E_sorted.size() == 0) {
      if (a.second.source() == uppermost_vertex ||
          a.second.target() == uppermost_vertex) {
        E_sorted.push_back(a.second);
      }
    } else {
      // If destination of last sorted segment is source of next segment to
      // check, then it is the correct next segment
      if (E_sorted.back().target() == a.second.source()) {
        E_sorted.push_back(a.second);
      }
    }
  }

  // Continue through A and check if next angle's segment shares a point with
  // last sorted vertex
  return E_sorted;
}

int main() {
  // Generate set of segments
  vector<Segment> E{
      Segment(Point(1, 0), Point(-1, 0)),
      Segment(Point(1, 2), Point(1, 0)),
      Segment(Point(-1, 2), Point(1, 2)),
      Segment(Point(-1, 0), Point(-1, 2)),
  };

  // Feed vector of segments to sorting algorithm
  vector<Segment> E_sorted = SortConvexHullSegments(E);

  // Output original and sorted vectors of segments for compairson
  cout << "E:\n";
  for (Segment s : E)
    cout << s << "\n";
  cout << "\nE Sorted:\n";
  for (Segment s : E_sorted)
    cout << s << "\n";
}
