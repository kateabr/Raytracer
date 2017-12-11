#include "light.hpp"

Light::Light(Vec3 pos, Material material) : pos_(pos), mat_(material) {}

Material Light::material() const { return mat_; }

// copy-paste Sphere
Segment Light::intersect(const Ray &ray) const {
  Vec3 eo = pos_.sub(ray.start);
  double v = eo.dot(ray.direction);
  double dist = 0;
  // if angle between -90-90, then ok
  if (v >= 0) {
    // v.dot(v) == v.Magnitude^2
    double radius_ = 0.1;
    double disc = radius_ * radius_ - (eo.dot(eo) - v * v);
    dist = disc < 0 ? 0 : v - sqrt(disc);
  }

  Segment ptr = {this, ray, dist};

  if (Utils::eqEps(dist, 0.0))
    ptr.object = nullptr;

  return ptr;
}

Vec3 Light::normal(const Vec3 &pos, const Segment &) const { return pos.sub(pos_).normal(); }

Vec3 Light::pos() const { return pos_; }
