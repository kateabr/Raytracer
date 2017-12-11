#ifndef RAY_HPP
#define RAY_HPP

#include "vec3.hpp"

/** @brief Луч */
struct Ray {
  /** @brief Начало луча */
  Vec3 start;
  /** @brief Направление луча */
  Vec3 direction;
};

#endif // RAY_HPP
