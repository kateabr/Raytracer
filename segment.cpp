#include "segment.hpp"

bool compareByDist(const Segment &a, const Segment &b) {
  return a.distance < b.distance;
}
