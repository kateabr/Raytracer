#include "vec3.hpp"

Vec3::Vec3() : x_(0), y_(0), z_(0) {}

Vec3::Vec3(double x, double y, double z) : x_(x), y_(y), z_(z) {}

double Vec3::x() const { return x_; }
double Vec3::y() const { return y_; }
double Vec3::z() const { return z_; }

void Vec3::setX(double x) { x_ = x; }
void Vec3::setY(double y) { y_ = y; }
void Vec3::setZ(double z) { z_ = z; }

bool Vec3::operator==(const Vec3 &other) const {
  return Utils::eqEps(x_, other.x_) && Utils::eqEps(y_, other.y_) &&
         Utils::eqEps(z_, other.z_);
}

bool Vec3::operator!=(const Vec3 &other) const { return !operator==(other); }

Vec3 Vec3::times(double n) const { return Vec3(n * x_, n * y_, n * z_); }

Vec3 Vec3::sub(const Vec3 &other) const {
  return Vec3(x_ - other.x_, y_ - other.y_, z_ - other.z_);
}

Vec3 Vec3::add(const Vec3 &other) const {
  return Vec3(x_ + other.x_, y_ + other.y_, z_ + other.z_);
}

Vec3 Vec3::normal() const {
  double length = mag();

  // если слишком малое значение
  if (Utils::eqEps(qAbs(length), 0.0))
    return times(std::numeric_limits<double>::max());
  else
    return times(1.0 / length);
}

double Vec3::mag() const { return sqrt(this->dot(*this)); }

Vec3 Vec3::cross(const Vec3 &other) const {
  return Vec3(y_ * other.z_ - z_ * other.y_, z_ * other.x_ - x_ * other.z_,
              x_ * other.y_ - y_ * other.x_);
}

double Vec3::dot(const Vec3 &other) const {
  return x_ * other.x_ + y_ * other.y_ + z_ * other.z_;
}

Vec3 Vec3::rotate(const Vec3 &axis, double angle) const {
  Matrix4x3 mat = Matrix4x3::createFromAxisAngle(axis, angle);
  return mat.transform(*this);
}

Vec3 Vec3::transform(const Matrix4x3 &mat) const {
  return mat.transform(*this);
}

Vec3 Vec3::reflect(const Vec3 &norm) const {
  // r=d−2(d⋅n)n
  return (*this).sub(norm.times((*this).dot(norm) * 2));
}
