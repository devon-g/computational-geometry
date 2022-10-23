#include "EventQueue.h"
#include "Status.h"

typedef std::pair<Point, std::unordered_set<Segment>> Intersection;

// Functions
std::vector<Intersection> FindIntersections(std::vector<Segment> S);
void HandleEventPoint(const Event &p);

// Global variables
EventQueue Q;
Status T;

/// Initiate algorithm on test data
int main(int argc, char **argv) {
  // Some test segments
  std::vector<Segment> S{Segment(Point(0, 0), Point(10, 10)),
                         Segment(Point(0, 10), Point(10, 0)),
                         Segment(Point(0, 0), Point(5, 5))};

  FindIntersections(S);
}

/// Fill event queue with initial events and begin handling said events
std::vector<Intersection> FindIntersections(std::vector<Segment> S) {
  Compare cmp;
  // Add all endpoints as events to the event queue.
  for (int i = 0; i < S.size(); i++) {
    // Make sure segments are directed up and to the left.
    // If source is higher than target then the directed segment.
    if (cmp(S[i].source(), S[i].target()))
      S[i] = S[i].opposite();

    // Copy the events to Q.
    Q.insert(Event(S[i].source(), {S[i]}));
    Q.insert(Event(S[i].target(), {S[i]}));
  }

  Q.print();

  // Handle event points in queue.
  while (!Q.empty()) {
    Event e = Q.pop();
  }

  return {};
}

/// Determine where the given event point lies on all segments and adjust T
/// accordingly.
void HandleEventPoint(const Event &p) {
  std::unordered_set<Segment> U;
  std::unordered_set<Segment> L;
  std::unordered_set<Segment> C;
  std::unordered_set<Segment> UunionC;
  std::unordered_set<Segment> LunionC;
  std::unordered_set<Segment> UunionLunionC;

  // Iterate through segments associated with p.
  // Add all segments with p as their target to U.
  for (auto it = p.second.begin(); it != p.second.end(); it++)
    if (p.first == it->target())
      U.insert(*it);

  // Get all segments in T that contain p.
  Node *t = T.root;
  do {
    // If our event point is the segment's lower point add to L.
    if (t->s.source() == p.first)
      L.insert(t->s);
    // If our event point is somewhere along segment t but not an endpoint add
    // to C.
    else if (t->s.target() != p.first && t->s.has_on(p.first))
      C.insert(t->s);
    // Move to next segment.
    t = t->next;
  } while (t != nullptr);

  // If more than one segment are in the union of U, L, and C, then we know p is
  // an intersection.
  UunionLunionC.insert(U.begin(), U.end());
  UunionLunionC.insert(L.begin(), L.end());
  UunionLunionC.insert(C.begin(), C.end());
}
