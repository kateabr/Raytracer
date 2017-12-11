#ifndef RAYTRACER_HPP
#define RAYTRACER_HPP

#include <QPixmap>

#include "camera.hpp"
#include "color.hpp"
#include "ray.hpp"
#include "scene.hpp"
#include "segment.hpp"

class Raytracer {
public:
  /**
   * @brief Конструктор рейтрейсера
   * @param size размер выходного изображения
   * @param scene связанная сцена
   */
  Raytracer(const QSize &size, const Scene *scene);

  /**
   * @brief Обновить размер выходного изображения
   * @param size новый размер
   */
  void updateSize(QSize const &size);

  /**
   * @brief Установить 4x антиалиасинг
   * @param enabled параметр включения
   */
  void setAntialiasing(bool enabled);

  /**
   * @brief Запуск рейтрейсера
   * @return выходное изображение
   */
  QPixmap render();

  /**
   * @brief Цвет фона
   * @return цвет фона
   */
  static Color bgColor();

  /**
   * @brief максимальная глубина рекурсии для отскоков лучей
   */
  static constexpr int MaxDepth = 5;

private:
  QSize size_;
  Scene const *scene_;
  double aspectratio_;
  bool antialiasing_;

  // константа для смещения луча чуть-чуть вперед
  // чтобы он не пересекался с объектом, с которого пускается новый луч
  static constexpr double bias = Utils::EPS;

  /**
   * @brief Создать вектор-луч, выходящий из камеры в пикселе (х, у)
   * @param координата по оси Ох
   * @param координата по оси Оу
   * @return единичный вектор-направление
   */
  Vec3 screenPixelDirection(double x, double y);

  /**
   * @brief Вспомогательная функция поиска цвета пикселя
   * @param seg текущий контекст
   * @param prevRefrIndex предыдущий индекс преломления
   * @param depth глубина отражений
   * @return цвет пикселя
   */
  Color rayTrace(Segment const &seg, double prevRefrIndex, int depth);

  /**
   * @brief Ищет пересечения луча с объектами сцены и сортирует список по
   * возрастанию расстояния
   * @param ray луч
   * @return отсортированный по растоянию вектор или пустой вектор
   */
  std::vector<Segment> sceneIntersections(const Ray &ray);

  /**
   * @brief Ищем отраженный цвет объекта при заданных позиции и нормали
   * @param interPoint точка пересечения
   * @param norm нормаль в точке пересечения луча камеры и объекта
   * @return цвет пикселя
   */
  Color rayTraceColor(const Segment &seg, Vec3 const &interPoint,
                      Vec3 const &norm);
};

#endif // RAYTRACER_HPP
