#ifndef SCENEBASE_HPP
#define SCENEBASE_HPP

#include "color.hpp"
#include "material.hpp"
#include "ray.hpp"
#include "segment.hpp"
#include "vec3.hpp"

/**
 * @brief Базовый объект сцены
 */
class SceneBase {
public:
  virtual Material material() const = 0;

  /**
   * @brief Создает контекст пересечения луча с объектом
   * @param ray луч
   * @return Если пересечение отсутствует, Segment.object == nullptr
   */
  virtual Segment intersect(Ray const &ray) const = 0;

  virtual Vec3 normal(Vec3 const &pos, Segment const &seg) const = 0;

  virtual Vec3 pos() const = 0;

  Color lightColor() const;

  double lightPower() const;

  virtual ~SceneBase();
};

#endif // SCENEBASE_HPP
