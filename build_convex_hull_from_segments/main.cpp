#include <CGAL/Simple_cartesian.h>

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_2 Point_2;
typedef Kernel::Segment_2 Segment_2;

int main() {
  // Generate list of segments that make up a convex polygon
  Segment_2 segments[]{Segment_2(Point_2(1, 1), Point_2(5, 5)),
                       Segment_2(Point_2(5, 5), Point_2(2, 3)),
                       Segment_2(Point_2(2, 3), Point_2(4, 2)),
                       Segment_2(Point_2(4, 2), Point_2(1, 1))};

  // For each segment
  // Remember leftmost vertex.
  Point_2 leftmost_point;
  for (Segment_2 s : segments) {
    // Compute supplementary angles for each segment and store them in an
    // ordered list.
    //
    // Maybe for each point on the segment center it on the origin and use the
    // other point to provide side lengths for a triangle to compute an angle?
    //
    // Keep checking each vertex we look at and seeing if it's the leftmost one.
  }

  // For each supplementary angle
  //   Check if next angle shares an endpoint with previous
  //     If not delete it from the list.
  //     If so add it to ordered list of segments representing the convex hull
  //     in counter-clockwise or clockwise order.
  // Return list
  return 0;
}
