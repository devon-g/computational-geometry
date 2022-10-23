#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include <CGAL/Simple_cartesian.h>
#include <map>
#include <set>

// Simplify the usage of these types by relabeling them.
typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_2 Point;
typedef Kernel::Segment_2 Segment;
typedef std::pair<Point, std::unordered_set<Segment>> Event;

/**
 * @brief Compare class to order event points by greatest y and least x.
 */
struct Compare {
  /**
   * @brief Determine if p > q by: p.y > q.y or (p.y = q.y && p.x < q.x).
   */
  bool operator()(const Point &p, const Point &q) const {
    return p.y() == q.y() ? p.x() < q.x() : p.y() > q.y();
  };
};

/**
 * @brief Queue class used to store sweep line events.
 */
class EventQueue {
public:
  /**
   * @brief Removes highest event below the sweep line.
   * @return Highest event below the sweep line.
   */
  Event pop();
  /**
   * @brief Insert a new event if it doesn't already exist.
   * @param e is the event to be inserted.
   */
  void insert(const Event &e);
  /**
   * @brief Check if event queue is empty.
   * @return True if empty false otherwise
   */
  bool empty();

  void print();

private:
  // Represent event queue as an ordered set (binary search tree)
  std::map<Point, std::unordered_set<Segment>, Compare> Q;
};

#endif
