#ifndef QUEUE_H
#define QUEUE_H

#include <CGAL/Simple_cartesian.h>
#include <queue>

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_2 Point;
typedef Kernel::Segment_2 Segment;

class Queue {
public:
  Queue();
  Point pop();
  void insert(std::pair<Point, Segment> PS);

private:
  std::priority_queue<std::pair<Point, Segment>> Q;
};

#endif
