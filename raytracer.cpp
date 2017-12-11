#include "raytracer.hpp"
#include <QDebug>
#include <QImage>
#include <omp.h>

#include "color.hpp"
#include "ray.hpp"
#include "vec3.hpp"

Color Raytracer::bgColor() {
  static Color bg(0, 0, 0);
  return bg;
}

Raytracer::Raytracer(const QSize &size, const Scene *scene)
    : size_(size), scene_(scene) {
  updateSize(size_);
  antialiasing_ = false;
}

void Raytracer::updateSize(const QSize &size) {
  size_ = size;
  aspectratio_ = size_.width() / (size_.height() * 1.0);
}

void Raytracer::setAntialiasing(bool enabled) { antialiasing_ = enabled; }

QPixmap Raytracer::render() {
  // матрица смещений для антиалиасинга
  static double jitterMatrix[4 * 2] = {
      -1.0 / 4.0, 3.0 / 4.0,  3.0 / 4.0, 1.0 / 3.0,
      -3.0 / 4.0, -1.0 / 4.0, 1.0 / 4.0, -3.0 / 4.0,
  };
  // финальное изображение
  QImage img(size_.width(), size_.height(), QImage::Format_RGB32);

  // основной цикл
#pragma omp parallel for
  for (int x = 0; x < size_.width(); ++x) {
    for (int y = 0; y < size_.height(); ++y) {
      Color color = bgColor();
      if (antialiasing_) {
        for (int m = 0; m < 4; ++m) {
          Ray r = {scene_->camera()->pos(),
                   screenPixelDirection(x + jitterMatrix[2 * m],
                                        y + jitterMatrix[2 * m + 1])};
          std::vector<Segment> inters = sceneIntersections(r);
          if (!inters.empty()) {
            color = color + rayTrace(inters[0], 1, 0);
          }
        }
        color = color * 0.25;
      } else {
        Ray r = {scene_->camera()->pos(), screenPixelDirection(x, y)};
        std::vector<Segment> inters = sceneIntersections(r);

        if (!inters.empty()) {
          color = rayTrace(inters[0], 1, 0);
        }
      }
      // сохраняем цвет
      img.setPixelColor(x, y, static_cast<QColor>(color));
    }
  }
  return QPixmap::fromImage(img);
}

Vec3 Raytracer::screenPixelDirection(double x, double y) {
  // -------------------------|
  // |          |             |
  // |          |             |
  // |--------- X ------------|
  // |          |             |
  // |          |             |
  // -------------------------|
  //
  // X -- центр камеры, от которого мы делаем "отступы"
  // чтобы получить цвет соответствующего пикселя
  double newX =
      (x - size_.width() / 2.0) / (2.0 * size_.width()) * aspectratio_;
  double newY = -(y - size_.height() / 2.0) / (2.0 * size_.height());

  Camera const *cam = scene_->camera();
  return cam->direction()
      .add(cam->right().times(newX))
      .add(cam->up().times(newY))
      .normal();
}

Color Raytracer::rayTrace(Segment const &seg, double prevRefrIndex, int depth) {
  if (depth > MaxDepth)
    return bgColor();

  // точка пересечения луча с объектом
  Vec3 interPoint = seg.ray.direction.times(seg.distance).add(seg.ray.start);

  // нормаль в точке пересечения viewPos с объектов viewPos передается для
  // моделирования сферы и плоскости с помощью мат. уравнений нежели чем
  // полигонов
  Vec3 norm = seg.object->normal(interPoint, seg);

  Color ret = rayTraceColor(seg, interPoint, norm);

  // константа для смещения луча чуть-чуть вперед
  // чтобы он не пересекался с объектом, с которого пускается новый луч
  constexpr double bias = 1e-4;

  // луч отражения
  double refl = seg.object->material().reflection();
  if ((depth < MaxDepth)) {
    // отраженный луч камеры
    Vec3 reflDir = seg.ray.direction.reflect(norm);
    // новый луч, исходящий из точки пересечения в отраженном направлении
    Ray r = {interPoint.add(reflDir.times(bias)), reflDir};
    std::vector<Segment> reflInters = sceneIntersections(r);
    if (!reflInters.empty()) {
      Color reflColor = rayTrace(reflInters[0], prevRefrIndex, depth + 1);
      ret = ret + reflColor * seg.object->material().color() * refl;
    }
  }

  //  // луч преломления
  //  double refr = seg.object->material().refraction();
  //  if (refr > 0) {
  //    double rindex = seg.object->material().refractionIndex();
  //    double n = prevRefrIndex / rindex;
  //    Vec3 N = norm;
  //    double cosI = -seg.ray.direction.dot(N);
  //    double sinT2 = n * n * (1.0 - cosI * cosI);
  //    if (sinT2 < 1.0) {

  //      double cosT = sqrt(1.0 - sinT2);
  //      Vec3 T = seg.ray.direction.times(n).add(N.times(n * cosI - cosT));

  //      Ray r = {interPoint.add(T.times(bias)), T};
  //      std::vector<Segment> refrInters = sceneIntersections(r);
  //      if (!refrInters.empty()) {
  //        Color reflColor = rayTrace(refrInters[0], rindex, depth + 1);
  //        ret = ret + reflColor; //* seg.object->material().refraction();
  //      }
  //    }
  //  }

  return ret;
}

std::vector<Segment> Raytracer::sceneIntersections(const Ray &ray) {
  std::vector<Segment> inters = scene_->intersect(ray);

  // сортируем пересечения по расстоянию от начала луча до точки
  // пересечения
  std::sort(inters.begin(), inters.end(), compareByDist);

  return inters;
}

Color Raytracer::rayTraceColor(Segment const &seg, const Vec3 &interPoint,
                               const Vec3 &norm) {
  Color ret = bgColor();

  for (SceneBase const *light : scene_->lights()) {
    // вектор точка пересечения -> источник света
    Vec3 lightDir = light->pos().sub(interPoint);
    // нормируем, чтобы получить чистое направление
    Vec3 lightDirNorm = lightDir.normal();

    // строим новый луч из точки пересечения в источник света
    Ray r = {interPoint, lightDirNorm};
    //    qDebug() << interPoint << " " << lightDirNorm;
    // ищем пересечение на пути из точки пересечения в источник света
    std::vector<Segment> inters = sceneIntersections(r);
    double dist = 0;
    if (!inters.empty()) {
      dist = inters[0].distance;
    }

    // если что-то есть на пути -- мы в тени другого объекта
    bool inShadow = !(dist > lightDir.mag() || Utils::eqEps(dist, 0.0));

    // получаем "бесплатные" тени -- просто не вызываем расчет цвета пикселя,
    // если объект в тени
    if (!inShadow) {
      // основной цвет
      if (seg.object->material().diffuse() > 0) {
        double dot = lightDirNorm.dot(norm);
        if (dot > 0) {
          double diff = seg.object->material().diffuse() * dot;
          Color lColor = light->lightColor() * light->lightPower();
          ret = ret + seg.object->material().color() * diff * lColor;
        }
      }

      // цвет бликов
      if (seg.object->material().specular() > 0) {
        Vec3 V = seg.ray.direction;
        Vec3 R = lightDirNorm.reflect(norm);
        double dot = V.dot(R);
        if (dot > 0) {
          double spec = pow(dot, seg.object->material().roughness()) *
                        seg.object->material().specular();
          ret = ret + light->lightColor() * spec;
        }
      }
    }
  }

  return ret;
}
