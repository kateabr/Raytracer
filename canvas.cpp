#include "canvas.hpp"
#include "raytracer.hpp"

Canvas::Canvas(QWidget *parent)
    : QFrame(parent), scene(new Scene()), raytracer(QSize(0, 0), scene) {
  rotationAxis = {0, 1, 0};
  rotationAngle = 5;

  // примеры (две сферы и плоскость)
  Material planeMat(Color(0.1, 0.1, 0.1), 1, 0.4, 0.75, 10);
  Material sphereMat1(Color(0, 0, 1), 0.7, 0.5, 0.9, 17);
  Material sphereMat2(Color(1, 0, 0), 0.7, 0.2, 0.3, 15, 0.7, 1.1);

  scene->addObject(new Plane(Vec3(0, 1, 0), 0, planeMat));
  scene->addObject(new Plane(Vec3(1, 0, 0), 5, planeMat));
  scene->addObject(new Plane(Vec3(0, 0, 1), 5, planeMat));
  scene->addObject(new Plane(Vec3(0, -1, 0), 15, planeMat));
  scene->addObject(new Sphere(Vec3(0, 1, 0), 1, sphereMat1));
  scene->addObject(new Sphere(Vec3(-2, 1, 3), 1, sphereMat2));

  Material lightMat(Color(), 0, 0, 0, 0, 0, 0, 1, Color(1, 1, 1));
  scene->addLight(new Light(Vec3(10, 10, 0), lightMat));
  scene->addLight(new Light(Vec3(50, 50, 50), lightMat));

  scene->camera()->setPos({5, 8, 45});
  scene->camera()->lookAt({0, 0, 0});
}

Scene *Canvas::getScene() const { return scene; }

void Canvas::paintEvent(QPaintEvent *) {
  QPainter p(this);
  p.drawPixmap(0, 0, pixmap);
}

void Canvas::resizeEvent(QResizeEvent *e) {
  raytracer.updateSize(e->size());
  renderImage();
}

void Canvas::keyPressEvent(QKeyEvent *e) {
  Camera *cam = scene->camera();
  Vec3 pos = cam->pos();
  bool needRepaint = false;

  if ((e->key() == Qt::Key_Up)) {
    pos.setY(pos.y() + 1);
    needRepaint = true;
  } else if ((e->key() == Qt::Key_Down)) {
    pos.setY(pos.y() - 1);
    needRepaint = true;
  } else if ((e->key() == Qt::Key_Left)) {
    pos = pos.rotate(rotationAxis, -rotationAngle);
    needRepaint = true;
  } else if ((e->key() == Qt::Key_Right)) {
    pos = pos.rotate(rotationAxis, rotationAngle);
    needRepaint = true;
  }

  if (needRepaint) {
    cam->setPos(pos);
    renderImage();
    repaint();

    emit updateUi();
  }
}

void Canvas::renderImage() {
  pixmap = QPixmap(size());
  pixmap = raytracer.render();
}

void Canvas::setKeyboardRotationAngle(double val) { rotationAngle = val; }

void Canvas::setKeyboardRotationAxis(Vec3 axis) { rotationAxis = axis; }

void Canvas::setAntiAliasing(bool enabled) {
  raytracer.setAntialiasing(enabled);
}

Vec3 Canvas::keyboardRotationAxis() const { return rotationAxis; }

Canvas::~Canvas() { delete scene; }
