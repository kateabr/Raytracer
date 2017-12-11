#ifndef SCENEOBJECTS_HPP
#define SCENEOBJECTS_HPP

#include <vector>

#include "material.hpp"
#include "ray.hpp"
#include "scenebase.hpp"
#include "segment.hpp"

class Sphere : public SceneBase {
public:
  Sphere(Vec3 const &center, double radius, Material const &material);

  Material material() const override;
  Segment intersect(Ray const &ray) const override;
  Vec3 normal(Vec3 const &pos, Segment const &) const override;

  Vec3 pos() const override;

private:
  Vec3 center_;
  double radius_;
  Material mat_;
};

class Plane : public SceneBase {
public:
  Plane(Vec3 const &norm, double offset, Material const &material);

  Material material() const override;
  Segment intersect(Ray const &ray) const override;
  Vec3 normal(Vec3 const &, Segment const &) const override;

  Vec3 pos() const override;

private:
  Vec3 normal_;
  Material mat_;
  double offset_;
};

/**
 * @brief Информация о данных объекта, объединенная в единую структуру для
 * упрощения сигнатур функции
 */
struct PolygonalData {
  /** @brief Массив вершин */
  std::vector<Vec3> vertices;

  /** @brief Количество граней */
  size_t facesCount;

  /**
   * @brief Количество вершин на грань, количество элементов в массиве
   * совпадает с facesCount
   */
  std::vector<size_t> faceIndex;

  /**
   * @brief Список связности вершин: набор индексов vertices;
   * порядок индексов важен: по часовой или против -- определяет, куда будет
   * "смотреть" нормаль грани; представляют из себя кортежи по faceIndex
   * элементов
   */
  std::vector<size_t> vertexIndex;
};

class PolygonalObject : public SceneBase {
public:
  PolygonalObject(Vec3 const &pos, Material const &material,
                  PolygonalData const &data);

  Material material() const override;
  Segment intersect(Ray const &ray) const override;

  Vec3 normal(Vec3 const &, Segment const &seg) const override;
  Vec3 pos() const override;

private:
  Vec3 pos_;
  Material mat_;
  PolygonalData data_; // храним для возможности сохранения объекта в файл

  bool triangleIntersect(Vec3 const &pos, Vec3 const &dir, Vec3 const &v0,
                         Vec3 const &v1, Vec3 const &v2, double &dist,
                         Vec3 &norm) const;

  // данные о треугольниках
  uint32_t numTris = 0; // кол-во треугольников
  std::vector<Vec3> vertices; // позиция вершин треугольниокв
  std::vector<size_t> trisIndex; // vertex index array
};

#endif // SCENEOBJECTS_HPP
