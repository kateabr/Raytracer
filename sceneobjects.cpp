#include "sceneobjects.hpp"

#include <QDebug>

Sphere::Sphere(const Vec3 &center, double radius, const Material &material)
    : center_(center), radius_(radius), mat_(material) {}

Material Sphere::material() const { return mat_; }

Segment Sphere::intersect(Ray const &ray) const {
  Vec3 eo = center_.sub(ray.start);
  double v = eo.dot(ray.direction);
  double dist = 0;
  // if angle between -90-90, then ok
  if (v >= 0) {
    // v.dot(v) == v.Magnitude^2
    double disc = radius_ * radius_ - (eo.dot(eo) - v * v);
    dist = disc < 0 ? 0 : v - sqrt(disc);
  }

  Segment ptr = {this, ray, dist};
  if (Utils::eqEps(dist, 0.0))
    ptr.object = nullptr;

  return ptr;
}

Vec3 Sphere::normal(const Vec3 &pos, const Segment &) const {
  return pos.sub(center_).normal();
}

Vec3 Sphere::pos() const { return center_; }

Plane::Plane(const Vec3 &norm, double offset, const Material &material)
    : normal_(norm), mat_(material), offset_(offset) {}

Material Plane::material() const { return mat_; }

Segment Plane::intersect(const Ray &ray) const {
  Segment ptr = {this, ray, 0.0};

  double denom = normal_.dot(ray.direction);
  if (denom > 0) {
    ptr.object = nullptr;
    return ptr;
  }

  ptr.distance = (normal_.dot(ray.start) + offset_) / (-denom);
  return ptr;
}

Vec3 Plane::normal(const Vec3 &, Segment const &) const { return normal_; }

Vec3 Plane::pos() const { return normal_.times(offset_); }

PolygonalObject::PolygonalObject(const Vec3 &pos, const Material &material,
                                 const PolygonalData &data)
    : pos_(pos), mat_(material), data_(data) {
  // делаем триангуляцию;
  // важно! -- fan-triangulation работает только с выпуклыми треугольниками
  // также не проверяем крайние случаи, например, когда кол-во вершин < 3
  size_t k = 0;
  size_t maxVertIndex = 0;

  // ищем, сколько нужно треугольников для объекта
  for (size_t i = 0; i < data_.facesCount; ++i) {
    numTris += data_.faceIndex[i] - 2;
    for (size_t j = 0; j < data_.faceIndex[i]; ++j)
      if (data_.vertexIndex[k + j] > maxVertIndex)
        maxVertIndex = data_.vertexIndex[k + j];
    k += data_.faceIndex[i];
  }
  maxVertIndex += 1;

  // Выделим память для вершин
  vertices.resize(maxVertIndex);
  for (size_t i = 0; i < maxVertIndex; ++i) {
    vertices[i] = data_.vertices[i];
  }

  // Выделяем память
  trisIndex.resize(numTris * 3);

  size_t l = 0;
  // Для каждой грани
  for (size_t i = 0, k = 0; i < data_.facesCount; ++i) {
    // Для каждого треугольника в грани
    for (size_t j = 0; j < data_.faceIndex[i] - 2; ++j) {
      trisIndex[l] = data_.vertexIndex[k];
      trisIndex[l + 1] = data_.vertexIndex[k + j + 1];
      trisIndex[l + 2] = data_.vertexIndex[k + j + 2];
      l += 3;
    }
    k += data_.faceIndex[i];
  }
}

Material PolygonalObject::material() const { return mat_; }

Segment PolygonalObject::intersect(const Ray &ray) const {
  uint32_t j = 0;
  bool isect = false;

  Vec3 norm;
  Segment ptr = {this, ray, Utils::PositiveInfinity, 0, norm};
  for (size_t i = 0; i < numTris; ++i) {
    Vec3 const &v0 = vertices[trisIndex[j]].add(pos_);
    Vec3 const &v1 = vertices[trisIndex[j + 1]].add(pos_);
    Vec3 const &v2 = vertices[trisIndex[j + 2]].add(pos_);

    double t = Utils::PositiveInfinity;
    if (triangleIntersect(ray.start, ray.direction, v0, v1, v2, t, norm) &&
        t < ptr.distance) {
      isect = true;
      ptr.distance = t;
      ptr.triNorm = norm;
      ptr.triIndex = i;
    }
    j += 3;
  }

  if (!isect)
    ptr.object = nullptr;

  return ptr;
}

bool PolygonalObject::triangleIntersect(Vec3 const &pos, Vec3 const &dir,
                                        Vec3 const &v0, Vec3 const &v1,
                                        Vec3 const &v2, double &dist,
                                        Vec3 &norm) const {
  // используем барицентрические координаты
  Vec3 e1 = v1.sub(v0);
  Vec3 e2 = v2.sub(v0);

  Vec3 pvec = dir.cross(e2);
  double det = e1.dot(pvec);

  if (det < Utils::EPS || Utils::eqEps(det, 0.0))
    return false;

  Vec3 tvec = pos.sub(v0);
  double u = tvec.dot(pvec);

  if (u < 0 || u > det)
    return false;

  Vec3 qvec = tvec.cross(e1);
  double v = dir.dot(qvec);
  if (v < 0 || u + v > det)
    return false;

  dist = e1.dot(pvec) / det;
  norm = e2.cross(e1).normal();

  return true;
}

Vec3 PolygonalObject::normal(const Vec3 &, const Segment &seg) const {
  return seg.triNorm;
}

Vec3 PolygonalObject::pos() const { return pos_; }
