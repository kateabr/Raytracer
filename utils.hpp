#ifndef UTILS_HPP
#define UTILS_HPP

#include <QtGlobal>

namespace Utils {
constexpr const double EPS = 1e-6;
constexpr const double NegativeInfinity = -1e10;
constexpr const double PositiveInfinity = 1e10;

template <typename T> bool eqEps(T a, T b) { return qAbs(a - b) < EPS; }

template <typename T> T clamp(T d, T min = 0.0, T max = 1.0) {
  return qBound(min, d, max);
}
} // namespace Utils

#endif // UTILS_HPP
