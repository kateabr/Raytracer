#ifndef SEGMENT_HPP
#define SEGMENT_HPP

#include "ray.hpp"

class SceneBase;

/** @brief Структура контекста */
struct Segment {
  SceneBase const *object;
  Ray ray;
  double distance;
  size_t triIndex;
  Vec3 triNorm;
};

bool compareByDist(Segment const &a, Segment const &b);

#endif // SEGMENT_HPP
