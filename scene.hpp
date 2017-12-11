#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>

#include "camera.hpp"
#include "light.hpp"
#include "sceneobjects.hpp"
#include "segment.hpp"

/** @brief Сцена, на которой располагаются объекты */
class Scene {
public:
  Scene();

  /**
   * @brief Получить камеру сцены
   * @return камера сцены
   */
  Camera *camera() const;

  /**
   * @brief Добавить объект на сцену
   * @param obj объект
   * @return сцену
   */
  Scene &addObject(SceneBase *obj);

  /**
   * @brief Добавить источник света на сцену
   * @param light источник света
   * @return сцену
   */
  Scene &addLight(Light *light);

  /**
   * @brief Ищем пересечения луча с объектами сцены
   * @param r луч
   * @return список сегментов пересечений
   */
  std::vector<Segment> intersect(Ray const &r) const;

  /**
   * @brief Получить список источников света, включает как чистые источники
   * света, так и объекты с ненулевым световым излучением
   * @return список источников света
   */
  std::vector<SceneBase *> lights() const;

  ~Scene();

private:
  /** @brief список объектов сцены */
  std::vector<SceneBase *> objects_;

  /** @brief список источников света */
  std::vector<Light *> lights_;

  /** @brief камера сцены */
  Camera *camera_;
};

#endif // SCENE_HPP
