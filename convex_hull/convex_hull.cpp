#include <CGAL/Simple_cartesian.h>
#include <bits/chrono.h>
#include <boost/function.hpp>
#include <chrono>
#include <random>

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_2 Point_2;
typedef Kernel::Segment_2 Segment_2;

std::vector<Point_2> SlowConvexHull(std::vector<Point_2> P) {
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

std::vector<Point_2> ConvexHull(std::vector<Point_2> P) {
  // Sort points by x coordinate (by y coordinate if x is the same)
  std::sort(P.begin(), P.end());

  // Put points p1 and p2 into list L_upper with p1 as the first point
  std::vector<Point_2> L_upper{P[0], P[1]};

  // for points p3 to pn
  for (int i = 2; i < P.size(); i++) {
    // append pi to L_upper
    L_upper.push_back(P[i]);
    // while L_upper contains more than two points and the last three points
    // in L_upper don't make a right turn
    while (L_upper.size() > 2 &&
           CGAL::orientation(L_upper[L_upper.size() - 3],
                             L_upper[L_upper.size() - 2],
                             L_upper[L_upper.size() - 1]) != CGAL::RIGHT_TURN) {
      // delete the middle of the last three points from L_upper
      L_upper.erase(L_upper.begin() + L_upper.size() - 2);
    }
  }

  // Put points p_n and p_n-1 in a list L_lower, with p_n as the first point
  std::vector<Point_2> L_lower{P[P.size() - 1], P[P.size() - 2]};
  // for points p_n-2 to p_1
  for (int i = P.size() - 3; i >= 0; i--) {
    // append p_i to L_lower
    L_lower.push_back(P[i]);
    // while L_lower contains more than two points and the last three points
    //   in L_lower do not make a right turn
    while (L_lower.size() > 2 &&
           CGAL::orientation(L_lower[L_lower.size() - 3],
                             L_lower[L_lower.size() - 2],
                             L_lower[L_lower.size() - 1]) != CGAL::RIGHT_TURN) {
      // delete the middle of the last three points from L_lower
      L_lower.erase(L_lower.begin() + L_lower.size() - 2);
    }
  }
  // Remove the first and last point from L_lower to avoid duplication of the
  // points where the upper and lower hull meet
  L_lower.erase(L_lower.begin());
  L_lower.erase(L_lower.begin() + L_lower.size() - 1);

  // Append L_lower to L_upper, and call the resulting list L.
  std::vector<Point_2> L;
  L.insert(L.end(), L_upper.begin(), L_upper.end());
  L.insert(L.end(), L_lower.begin(), L_lower.end());

  // Return L
  return L;
}

std::vector<Point_2> MediumConvexHull(std::vector<Point_2> P) {
  // Find point with smallest x coordinate
  int l = 0;
  for (int i = 1; i < P.size() - 1; i++)
    if (P[i].x() < P[l].x())
      l = i;

  // Store vertices here in L
  std::vector<Point_2> L;

  int p = l;
  L.push_back(P[p]);
  do {
    // Make sure p and q are not equal point
    int q = p + 1;
    if (P[p] != P[q]) {
      for (int i = 1; i < P.size() - 1; i++)
        if (CGAL::left_turn(P[p], P[i], P[q]))
          q = i;
      L.push_back(P[q]);
    }
    p = q;
  } while (p != l);

  return L;
}

int main() {
  // Seed random
  std::random_device rd;
  // std::mt19937 gen(rd());
  std::mt19937 gen(rd());

  // Convex hull repetitions
  int n_iterations = 10;

  // Set point generation settings
  int slow_convex_hull_n_points = 4000;
  int convex_hull_n_points = 2000000;
  int x_max = 1000000000;
  int x_min = -1000000000;
  int y_max = 1000000000;
  int y_min = -1000000000;

  // Give the number generator a range
  std::uniform_int_distribution<> x_distr(x_min, x_max);
  std::uniform_int_distribution<> y_distr(y_min, y_max);

  // Apply convex hull algorithm to different sets of random points
  int slow_convex_hull_total_count = 0;
  int convex_hull_total_count = 0;
  for (int c = 1; c <= 3; c++) {
    // CONVEX HULL
    // Generate random points
    std::vector<Point_2> P_fast;
    for (int i = 0; i < c * convex_hull_n_points; i++)
      P_fast.push_back(Point_2(x_distr(gen), y_distr(gen)));

    // Start timer
    auto start = std::chrono::high_resolution_clock::now();
    // Compute convex hull of points P
    ConvexHull(P_fast);
    // Stop timer
    auto stop = std::chrono::high_resolution_clock::now();
    // Compute and output duration
    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    convex_hull_total_count += duration.count();

    // SLOW CONVEX HULL
    // Generate random points
    std::vector<Point_2> P_slow;
    for (int i = 0; i < c * slow_convex_hull_n_points; i++)
      P_slow.push_back(Point_2(x_distr(gen), y_distr(gen)));
    // Start timer
    start = std::chrono::high_resolution_clock::now();
    // Compute convex hull of points P
    SlowConvexHull(P_slow);
    // Stop timer
    stop = std::chrono::high_resolution_clock::now();
    duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    slow_convex_hull_total_count += duration.count();
    // Output duration
    std::cout << "Convex Hull\nN: " << c * convex_hull_n_points
              << ", Runtime: " << convex_hull_total_count << "ms" << std::endl;
    std::cout << "Slow Convex Hull\nN: " << c * slow_convex_hull_n_points
              << ", Runtime: " << slow_convex_hull_total_count << "ms"
              << std::endl;
  }

  return 0;
}
