#include "matrix4x3.hpp"

#include <QtMath>

#include "utils.hpp"

Matrix4x3::Matrix4x3() {
  std::fill(mat, mat + 12, 0);
  mat[0] = 1;
  mat[4] = 1;
  mat[8] = 1;
}

Matrix4x3::Matrix4x3(double a, double b, double c, double d, double e, double f,
                     double g, double h, double i, double tx, double ty,
                     double tz) {
  mat[0] = a;
  mat[1] = b;
  mat[2] = c;
  mat[3] = d;
  mat[4] = e;
  mat[5] = f;
  mat[6] = g;
  mat[7] = h;
  mat[8] = i;
  mat[9] = tx;
  mat[10] = ty;
  mat[11] = tz;
}

Matrix4x3 Matrix4x3::scale(double sx, double sy, double sz) const {
  Matrix4x3 res(sx, 0, 0, 0, sy, 0, 0, 0, sz);
  return (*this) * res;
}

Matrix4x3 Matrix4x3::rotate(double rx, double ry, double rz) const {
  double rxR = qDegreesToRadians(rx);
  double ryR = qDegreesToRadians(ry);
  double rzR = qDegreesToRadians(rz);
  Matrix4x3 rotX(1, 0, 0, 0, qCos(rxR), -qSin(rxR), 0, qSin(rxR), qCos(rxR));
  Matrix4x3 rotY(qCos(ryR), 0, qSin(ryR), 0, 1, 0, -qSin(ryR), 0, qCos(ryR));
  Matrix4x3 rotZ(qCos(rzR), -qSin(rzR), 0, qSin(rzR), qCos(rzR), 0, 0, 0, 1);
  return (*this) * rotX * rotY * rotZ;
}

Matrix4x3 Matrix4x3::translate(double tx, double ty, double tz) const {
  Matrix4x3 res(*this);
  res.mat[9] += tx;
  res.mat[10] += ty;
  res.mat[11] += tz;
  return res;
}

void Matrix4x3::reset() {
  std::fill(mat, mat + 12, 0);
  mat[0] = 1;
  mat[4] = 1;
  mat[8] = 1;
}

Matrix4x3 Matrix4x3::operator+(const Matrix4x3 &other) const {
  Matrix4x3 res;
  for (int i = 0; i < 12; ++i)
    res.mat[i] = mat[i] + other.mat[i];
  return res;
}

Matrix4x3 Matrix4x3::operator-(const Matrix4x3 &other) const {
  Matrix4x3 res;
  for (int i = 0; i < 12; ++i)
    res.mat[i] = mat[i] - other.mat[i];
  return res;
}

Matrix4x3 Matrix4x3::operator*(const Matrix4x3 &other) const {
  Matrix4x3 res;
  res.reset();
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      res.mat[3 * i + j] += mat[3 * i + j] * other.mat[j * 3 + i];
  return res;
}

Matrix4x3 Matrix4x3::operator*(double n) const {
  Matrix4x3 res;
  for (int i = 0; i < 12; ++i)
    res.mat[i] = mat[i] * n;
  return res;
}

bool Matrix4x3::operator==(const Matrix4x3 &other) const {
  for (int i = 0; i < 12; ++i)
    if (!Utils::eqEps(mat[i], other.mat[i]))
      return false;
  return true;
}

Vec3 Matrix4x3::transform(const Vec3 &v) const {
  Vec3 res(v);
  res.setX(v.x() * mat[0] + v.y() * mat[3] + v.z() * mat[6] + mat[9]);
  res.setY(v.x() * mat[1] + v.y() * mat[4] + v.z() * mat[7] + mat[10]);
  res.setZ(v.x() * mat[2] + v.y() * mat[5] + v.z() * mat[8] + mat[11]);
  return res;
}

Matrix4x3 Matrix4x3::createFromAxisAngle(const Vec3 &axis, double angle) {
  // https://en.wikipedia.org/wiki/Rotation_matrix#Rotation_matrix_from_axis_and_angle
  Matrix4x3 res;
  Vec3 ax = axis.normal();
  double angleRad = qDegreesToRadians(angle);
  double cosT = cos(angleRad);
  double sinT = sin(angleRad);

  double cosT1 = 1 - cosT;

  double x = ax.x();
  double y = ax.y();
  double z = ax.z();

  double xx = x * x;
  double xy = x * y;
  double xz = x * z;
  double yy = y * y;
  double yz = y * z;
  double zz = z * z;

  double xyCosT1 = xy * cosT1;
  double xzCosT1 = xz * cosT1;
  double yzCosT1 = yz * cosT1;

  double xSinT = x * sinT;
  double ySinT = y * sinT;
  double zSinT = z * sinT;

  res.mat[0] = cosT + xx * cosT1;
  res.mat[1] = xyCosT1 - zSinT;
  res.mat[2] = xzCosT1 + ySinT;

  res.mat[3] = xyCosT1 + zSinT;
  res.mat[4] = cosT + yy * cosT1;
  res.mat[5] = yzCosT1 - xSinT;

  res.mat[6] = xzCosT1 - ySinT;
  res.mat[7] = yzCosT1 + xSinT;
  res.mat[8] = cosT + zz * cosT1;

  res.mat[9] = 0;
  res.mat[10] = 0;
  res.mat[11] = 0;

  return res;
}
