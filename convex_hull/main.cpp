#include "gnuplot-iostream.h"
#include <CGAL/Simple_cartesian.h>
#include <iostream>
#include <random>

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_2 Point_2;
typedef Kernel::Segment_2 Segment_2;

std::vector<Segment_2> compute_convex_hull_segments(std::vector<Point_2> P) {
  // Vector of segments that make up the convex hull
  std::vector<Segment_2> E;

  // Go through all pairs of points
  for (int p = 0; p < P.size(); p++) {
    for (int q = 0; q < P.size(); q++) {
      // Make sure we aren't looking at two of the same point
      if (P[p] != P[q]) {
        // Start by assuming the directed segment is a valid hull segment
        bool valid = true;

        // Go through all points to make sure none of them are right of the
        // segment
        for (int r = 0; r < P.size(); r++) {
          // Make sure r is not p or q and check if it is right of directed
          // segment pq if all of these things are true then pq is not a segment
          // of the convex hull
          if (P[r] != P[p] && P[r] != P[q] &&
              CGAL::right_turn(P[p], P[q], P[r])) {
            valid = false;
            break;
          }
        }

        // If valid segment of convex hull add to list of segments
        if (valid)
          E.push_back(Segment_2(P[p], P[q]));
      }
    }
  }
  return E;
}

std::vector<Point_2> segments_to_vertices(std::vector<Segment_2> E) {
  // List of vertices that make up the convex hull of the set of points P
  std::vector<Point_2> L;

  // Store origin and destination of segment 1 in L
  L.push_back(E[0].point(0));
  L.push_back(E[0].point(1));
  // Pop initial CH segment
  E.erase(E.begin());

  // Keep popping segments and saving their points in order
  while (E.size() > 0) {
    Segment_2 e_i;
    // Find segment in E whose origin is equal to the last segment's destination
    for (int i = 0; i < E.size(); i++) {
      // Check if the previous destination matches origin of segment i
      if (L.back() == E[i].point(0)) {
        // If the destination of segment i isn't the original vertex in L we add
        // it to L
        if (L[0] != E[i].point(1))
          L.push_back(E[i].point(1));
        // Remove segment i from E and find the next vertex of the CH
        E.erase(E.begin() + i);
        break;
      }
    }
  }
  return L;
}

int main() {
  // Seed random
  std::random_device rd;
  std::mt19937 gen(rd());

  // Instantiate gnuplot stream
  Gnuplot gp;

  // Set point generation settings
  int num_points = 10;
  int x_max = 20;
  int x_min = -20;
  int y_max = 20;
  int y_min = -20;

  // Give the number generator a range
  std::uniform_int_distribution<> x_distr(x_min, x_max);
  std::uniform_int_distribution<> y_distr(y_min, y_max);

  while (true) {
    std::vector<Point_2> P;
    for (int i = 0; i < num_points; i++) {
      bool duplicate;
      if (i > 0)
        bool duplicate = true;
      int x = x_distr(gen);
      int y = y_distr(gen);

      while (duplicate) {
        x = x_distr(gen);
        y = y_distr(gen);
        for (Point_2 p : P)
          if (!(p.x() == x && p.y() == y))
            duplicate = false;
      }

      P.push_back(Point_2(x, y));
    }
    std::vector<Segment_2> E = compute_convex_hull_segments(P);
    std::vector<Point_2> L = segments_to_vertices(E);

    // Duplicate first point in L for gnuplot
    L.push_back(L[0]);

    // Plot P and L
    gp << "plot '-' with lines title 'CH', '-' title 'P'\n";
    gp.send1d(L);
    gp.send1d(P);
    gp << "pause 1\n";
  }

  return 0;
}
