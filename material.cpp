#include "material.hpp"

Material::Material(Color color, double diff, double spec, double reflect,
                   double roughness, double refraction, double refractionIndex,
                   double emissionPower, Color emissionColor)
    : color_(color), diff_(diff), spec_(spec), reflect_(reflect),
      roughness_(roughness), refraction_(refraction),
      refractionIndex_(refractionIndex), emissionPower_(emissionPower),
      emissionColor_(emissionColor) {
  if (emissionPower_ < 0)
    throw std::invalid_argument("Power cannot be negative");
}

double Material::diffuse() const { return diff_; }

double Material::reflection() const { return reflect_; }

double Material::roughness() const { return roughness_; }

double Material::refraction() const { return refraction_; }

double Material::refractionIndex() const { return refractionIndex_; }

double Material::emissionPower() const { return emissionPower_; }

Color Material::emissionColor() const { return emissionColor_; }

double Material::specular() const { return spec_; }

Color Material::color() const { return color_; }
