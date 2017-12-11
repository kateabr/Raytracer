#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "color.hpp"

/** @brief Материал объекта сцены */
class Material {
public:
  Material(Color color, double diffuse, double specular, double reflection,
           double roughness, double refraction = 0, double refractionIndex = 1,
           double emissionPower = 0, Color emissionColor = Color(1, 1, 1));

  Material() = delete;
  Material(Material const &other) = default;
  Material(Material &&other) = default;

  Material &operator=(Material const &other) = default;
  Material &operator=(Material &&other) = default;

  Color color() const;
  double diffuse() const;
  double specular() const;
  double reflection() const;
  double roughness() const;
  double refraction() const;
  double refractionIndex() const;
  double emissionPower() const;
  Color emissionColor() const;

private:
  Color color_;
  double diff_;
  double spec_;
  double reflect_;
  double roughness_;
  double refraction_;
  double refractionIndex_;
  double emissionPower_;
  Color emissionColor_;
};

#endif // MATERIAL_HPP
