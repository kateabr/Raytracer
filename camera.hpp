#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "vec3.hpp"

/** @brief Камера */
class Camera {
public:
  Camera();

  /** @brief Направить камеру на интересующую точку
   * @param lookAt точка интереса
   */
  void lookAt(Vec3 const &lookat);
  Vec3 lookAtValue() const;

  Vec3 direction() const;
  Vec3 pos() const;
  void setPos(const Vec3 &pos);
  Vec3 right() const;
  Vec3 up() const;
  void setFocalDist(double focalDist);

private:
  // Направление низа камеры
  Vec3 down_ = Vec3(0, -1, 0);
  // Позиция камеры
  Vec3 pos_;
  // Куда смотрит камера (нормаль)
  Vec3 direction_;
  // Где у камеры право
  Vec3 right_;
  // Где у камеры верх
  Vec3 up_;
  // Куда смотрим
  Vec3 lookat_;
  // Фокальное расстояние камеры
  double focalDist_ = 0.35;
};

#endif // CAMERA_HPP
