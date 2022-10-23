#ifndef STATUS_H
#define STATUS_H

#include <CGAL/Simple_cartesian.h>
#include <set>

// Simplify the usage of these types by relabeling them.
typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_2 Point;
typedef Kernel::Segment_2 Segment;
typedef std::pair<Point, std::unordered_set<Segment>> Event;

/**
 * @brief Node for status that holds single segment and it's next neighbor.
 */
class Node {
public:
  Node(Segment s) : s(s) {};
  Segment s;
  Node* next = nullptr;
};

/**
 * @brief Stores segments intersecting with sweep line.
 */
class Status {
public:
  void insert(Segment s);
  void remove(Segment s);
  Node* root = nullptr;
};

#endif
