#ifndef CANVAS_HPP
#define CANVAS_HPP

#include <QtWidgets>

#include "raytracer.hpp"
#include "scene.hpp"

/** @brief Виджет канваса, на котором происходит отрисовка.
 * Содержит в себе QPixmap, на котором и происходит фактическое рисование
 * с дальнейшим выводом pixmap'а на экран.
 */
class Canvas : public QFrame {
  Q_OBJECT
public:
  Canvas(QWidget *parent = nullptr);

  Scene *getScene() const;
  void renderImage();

  void setKeyboardRotationAngle(double val);
  void setKeyboardRotationAxis(Vec3 axis);

  void setAntiAliasing(bool enabled);

  Vec3 keyboardRotationAxis() const;

  ~Canvas() override;

signals:
  void updateUi();

protected:
  void paintEvent(QPaintEvent *) override;
  void resizeEvent(QResizeEvent *) override;
  void keyPressEvent(QKeyEvent *) override;

private:
  QPixmap pixmap;
  Scene *scene;
  Raytracer raytracer;

  Vec3 rotationAxis;
  double rotationAngle;
};

#endif // CANVAS_HPP
