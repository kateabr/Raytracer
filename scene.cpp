#include "scene.hpp"
#include <stdexcept>

Scene::Scene() : camera_(new Camera()) {}

Camera *Scene::camera() const { return camera_; }

Scene &Scene::addObject(SceneBase *obj) {
  if (dynamic_cast<Light *>(obj) == nullptr)
    objects_.emplace_back(obj);
  else
    throw std::invalid_argument("Cannot add lights to the object pool");
  return *this;
}

Scene &Scene::addLight(Light *light) {
  lights_.emplace_back(light);
  return *this;
}

std::vector<Segment> Scene::intersect(const Ray &r) const {
  std::vector<Segment> result;

  for (SceneBase const *obj : objects_) {
    Segment res = obj->intersect(r);
    if (res.object)
      result.emplace_back(res);
  }

  // TODO: не работает
  //  for (SceneBase const *obj : lights_) {
  //    Segment res = obj->intersect(r);
  //    if (res.object)
  //      result.emplace_back(res);
  //  }

  return result;
}

std::vector<SceneBase *> Scene::lights() const {
  std::vector<SceneBase *> lights;
  for (Light *light : lights_) {
    lights.emplace_back(light);
  }
  for (SceneBase *obj : objects_) {
    if (obj->material().emissionPower() > 0)
      lights.emplace_back(obj);
  }
  return lights;
}

Scene::~Scene() {
  delete camera_;
  for (SceneBase *obj : objects_)
    delete obj;
  for (Light *l : lights_)
    delete l;
}
