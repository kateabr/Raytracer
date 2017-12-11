#ifndef VEC3_HPP
#define VEC3_HPP

#include <QDebug>
#include <cmath>
#include <iostream>

#include "matrix4x3.hpp"
#include "utils.hpp"

class Matrix4x3;

/** @brief Класс вектора (точки) */
class Vec3 {
public:
  Vec3();

  Vec3(Vec3 const &other) = default;
  Vec3(Vec3 &&other) = default;
  Vec3 &operator=(Vec3 const &other) = default;
  Vec3 &operator=(Vec3 &&other) = default;

  Vec3(double x, double y, double z);

  double x() const;
  double y() const;
  double z() const;

  void setX(double x);
  void setY(double y);
  void setZ(double z);

  bool operator==(Vec3 const &other) const;

  bool operator!=(Vec3 const &other) const;

  friend std::ostream &operator<<(std::ostream &os, Vec3 const &vec) {
    os << "(" << vec.x() << ", " << vec.y() << ", " << vec.z() << ")";
    return os;
  }

  friend QDebug operator<<(QDebug os, Vec3 const &vec) {
    os.nospace() << "(" << vec.x() << ", " << vec.y() << ", " << vec.z() << ")";
    return os;
  }

  /** @brief Умножить вектор на скаляр
   * @param n множитель
   * @return вектор, умноженный на число
   */
  Vec3 times(double n) const;

  /** @brief Вычесть вектор из вектора
   * @param other второй вектор
   * @return вектор-разница
   */
  Vec3 sub(Vec3 const &other) const;

  /** @brief Прибавить вектор к вектору
   * @param other второй вектор
   * @return вектор-сумма
   */
  Vec3 add(Vec3 const &other) const;

  /** @brief Вычисление нормали вектора
   * @return нормаль вектора
   */
  Vec3 normal() const;

  /** @brief Вычисление длины вектора
   * @return длина вектора
   */
  double mag() const;

  /** @brief Вычисление векторного произведения
   * @param other второй вектор
   * @return векторное произведение
   */
  Vec3 cross(Vec3 const &other) const;

  /** @brief Вычисление скалярного произведения векторов
   * @param other второй вектор
   * @return скалярное произведение
   */
  double dot(Vec3 const &other) const;

  /** @brief Поворот вектора вокруг указанной оси
   * @param pos ось
   * @param angle угол поворота, в градусах
   * @return новый вектор, повернутый на angle градусов вокруг оси pos
   */
  Vec3 rotate(Vec3 const &axis, double angle) const;

  /** @brief transform
   * @param mat
   * @return
   */
  Vec3 transform(Matrix4x3 const &mat) const;

  /** @brief reflect
   * @param norm
   * @return
   */
  Vec3 reflect(const Vec3 &norm) const;

private:
  double x_;
  double y_;
  double z_;
};

#endif // VEC3_HPP
