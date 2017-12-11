#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <stdexcept>

#include "color.hpp"
#include "scenebase.hpp"
#include "sceneobjects.hpp"
#include "vec3.hpp"

/** @brief Источник света */
class Light : public SceneBase {
public:
  Light() = delete;
  Light(Vec3 pos, Material material);

  Light(Light const &other) = default;
  Light(Light &&other) = default;
  Light &operator=(Light const &other) = default;
  Light &operator=(Light &&other) = default;

  Material material() const override;
  Segment intersect(Ray const &ray) const override;
  Vec3 normal(Vec3 const &pos, Segment const &) const override;

  Vec3 pos() const override;

private:
  Vec3 pos_;
  Material mat_;
};

#endif // LIGHT_HPP
