#include "EventQueue.h"

Event EventQueue::pop() {
  // End points beyond last element so we decrement iterator to get last element
  Event e = *this->Q.begin();
  this->Q.erase(this->Q.begin());
  return e;
}

void EventQueue::insert(const Event &e) {
  // Make sure event e hasn't already been added to the event queue
  if (!this->Q.count(e.first))
    this->Q.insert(e);
  else // If event is already present add new segments to associated segments
    this->Q[e.first].insert(e.second.begin(), e.second.end());
}

void EventQueue::print() {
  // Print event point and it's associated segment for debugging.
  for (auto Q_it = this->Q.begin(); Q_it != this->Q.end(); Q_it++) {
    std::cout << "Point:\t" << Q_it->first << ", Segments: ";
    for (auto S_it = Q_it->second.begin(); S_it != Q_it->second.end(); S_it++)
      std::cout << *S_it << "\t";
    std::cout << std::endl;
  }
}

bool EventQueue::empty() { return this->Q.empty(); }
