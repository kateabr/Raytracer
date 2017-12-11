#include "scenebase.hpp"

Color SceneBase::lightColor() const { return material().emissionColor(); }

double SceneBase::lightPower() const { return material().emissionPower(); }

SceneBase::~SceneBase() {}
