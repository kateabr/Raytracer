#ifndef MATRIX4X3_HPP
#define MATRIX4X3_HPP

#include "vec3.hpp"

class Vec3;

class Matrix4x3 {
public:
  /**
   * @brief Единичная матрица
   */
  Matrix4x3();

  /*
   * a   b   c | tx
   * d   e   f | ty
   * g   h   i | tz
   */
  Matrix4x3(double a, double b, double c, double d, double e, double f,
            double g, double h, double i, double u = 0, double v = 0,
            double tz = 0);

  Matrix4x3(Matrix4x3 const &other) = default;
  Matrix4x3(Matrix4x3 &&other) = default;
  Matrix4x3 &operator=(Matrix4x3 const &other) = default;
  Matrix4x3 &operator=(Matrix4x3 &&other) = default;

  Matrix4x3 scale(double sx, double sy, double sz) const;
  Matrix4x3 rotate(double rx, double ry, double rz) const;
  Matrix4x3 translate(double tx, double ty, double tz) const;

  // void transpose();
  void reset();

  Matrix4x3 operator+(Matrix4x3 const &other) const;
  Matrix4x3 operator-(Matrix4x3 const &other) const;
  Matrix4x3 operator*(Matrix4x3 const &other) const;
  Matrix4x3 operator*(double n) const;
  bool operator==(Matrix4x3 const &other) const;

  /**
   * @brief Преобразовать вектор
   * @param v вектор
   * @return преобразованный вектор
   */
  Vec3 transform(Vec3 const &v) const;

  /**
   * @brief Создать матрицу поворота вокруг оси на заданный угол
   * @param axis ось вращения
   * @param angle угол вращения, в градусах
   * @return матрица трансформации
   */
  static Matrix4x3 createFromAxisAngle(const Vec3 &axis, double angle);

private:
  double mat[12];
};

#endif // MATRIX4X3_HPP
