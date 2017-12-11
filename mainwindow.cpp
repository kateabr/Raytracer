#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include "camera.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  initSlots();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_spawn_clicked() {
  Vec3 pos(ui->xPos->value(), ui->yPos->value(), ui->zPos->value());
  Material mat(
      Color(ui->colorR->value(), ui->colorG->value(), ui->colorB->value()),
      ui->diffuse->value(), ui->specular->value(), ui->reflection->value(),
      ui->roughness->value(), ui->refraction->value(),
      ui->refractionIndex->value(), ui->emissionPower->value(),
      Color(ui->colorR->value(), ui->colorG->value(), ui->colorB->value()));

  if (ui->sphere->isChecked())
    ui->canvas->getScene()->addObject(new Sphere(pos, 1, mat));
  else if (ui->plane->isChecked())
    ui->canvas->getScene()->addObject(new Plane(pos, ui->offset->value(), mat));
  else {
    PolygonalData polyData;
    std::vector<Vec3> vertices;
    size_t faceCount = 0;
    std::vector<size_t> vertPerFace;
    std::vector<size_t> vertIndex;

    if (ui->tetrahedron->isChecked()) {
      vertices = {Vec3(0, 0, 0), Vec3(1, 0, 1), Vec3(1, 1, 0), Vec3(0, 1, 1)};
      faceCount = 4;
      vertPerFace = {3, 3, 3, 3};
      vertIndex = {0, 1, 2, 0, 2, 3, 0, 3, 1, 1, 3, 2};
    } else if (ui->hexahedron->isChecked()) {
      vertices = {Vec3(0, 0, 0), Vec3(1, 0, 0), Vec3(1, 0, 1), Vec3(0, 0, 1),
                  Vec3(0, 1, 0), Vec3(1, 1, 0), Vec3(1, 1, 1), Vec3(0, 1, 1)};
      faceCount = 6;
      vertPerFace = {4, 4, 4, 4, 4, 4};
      vertIndex = {0, 1, 2, 3, 0, 4, 5, 1, 0, 3, 7, 4,
                   2, 1, 5, 6, 3, 2, 6, 7, 7, 6, 5, 4};
    } else if (ui->icosahedron->isChecked()) {
      vertices = {Vec3(0, 0, 1.17557),
                  Vec3(1.05146, 0, 0.525731),
                  Vec3(0.32492, 1, 0.525731),
                  Vec3(-0.850651, 0.618034, 0.525731),
                  Vec3(-0.850651, -0.618034, 0.525731),
                  Vec3(0.32492, -1, 0.525731),
                  Vec3(0.850651, 0.618034, -0.525731),
                  Vec3(0.850651, -0.618034, -0.525731),
                  Vec3(-0.32492, 1, -0.525731),
                  Vec3(-1.05146, 0, -0.525731),
                  Vec3(-0.32492, -1, -0.525731),
                  Vec3(0, 0, -1.17557)};
      faceCount = 20;
      vertPerFace = {3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
                     3, 3, 3, 3, 3, 3, 3, 3, 3, 3};
      vertIndex = {0,  1, 2, 2,  1, 6, 6,  1, 7,  7,  1, 5,  5,  1, 0,
                   5,  0, 4, 4,  0, 3, 3,  0, 2,  3,  2, 8,  8,  2, 6,
                   3,  8, 9, 4,  3, 9, 4,  9, 10, 10, 9, 11, 11, 9, 8,
                   11, 8, 6, 11, 6, 7, 11, 7, 10, 10, 7, 5,  10, 5, 4};
    }
    // HALF DONE
    else if (ui->dodecahedron->isChecked()) {
      vertices = {Vec3(0, 0, 1.07047),
                  Vec3(0.713644, 0, 0.797878),
                  Vec3(-0.356822, 0.618034, 0.797878),
                  Vec3(-0.356822, -0.618034, 0.797878),
                  Vec3(0.797878, 0.618034, 0.356822),
                  Vec3(0.797878, -0.618034, 0.356822),
                  Vec3(-0.934172, 0.381966, 0.356822),
                  Vec3(0.136294, 1, 0.356822),
                  Vec3(0.136294, -1, 0.356822),
                  Vec3(-0.934172, -0.381966, 0.356822),
                  Vec3(0.934172, 0.381966, -0.356822),
                  Vec3(0.934172, -0.381966, -0.356822),
                  Vec3(-0.797878, 0.618034, -0.356822),
                  Vec3(-0.136294, 1, -0.356822),
                  Vec3(-0.136294, -1, -0.356822),
                  Vec3(-0.797878, -0.618034, -0.356822),
                  Vec3(0.356822, 0.618034, -0.797878),
                  Vec3(0.356822, -0.618034, -0.797878),
                  Vec3(-0.713644, 0, -0.797878),
                  Vec3(0, 0, -1.07047)};
      faceCount = 12;
      vertPerFace = {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5};

      // TODO !!!
      // clang-format off
      vertIndex = {4, 7, 2, 0, 1,
                   0, 3, 8, 5, 1,
                   0, 2, 6, 9, 3,
                   1, 5, 11, 10, 4,
                   2, 7, 13, 12, 6,
                   3, 9, 15, 14, 8,
                   7, 4, 10, 16, 13,
                   5, 8, 14, 17, 11,
                   9, 6, 12, 18, 15,
                   14, 15, 18, 19, 17,
                   10, 11, 17, 19, 16,
                   12, 13, 16, 19, 18};
      // clang-format on

    } else if (ui->octahedron->isChecked()) {

      vertices = {Vec3(0, 0, 1.41421),  Vec3(1.41421, 0, 0),
                  Vec3(0, 1.41421, 0),  Vec3(-1.41421, 0, 0),
                  Vec3(0, -1.41421, 0), Vec3(0, 0, -1.41421)};
      faceCount = 8;
      vertPerFace = {3, 3, 3, 3, 3, 3, 3, 3};

      vertIndex = {0, 2, 3, 3, 2, 5, 5, 2, 1, 1, 2, 0,
                   1, 4, 5, 0, 4, 1, 3, 4, 0, 5, 4, 3};
    }
    ui->canvas->getScene()->addObject(new PolygonalObject(
        pos, mat, {vertices, faceCount, vertPerFace, vertIndex}));
  }

  ui->canvas->renderImage();
  ui->canvas->repaint();
}

void MainWindow::initSlots() {
  auto spinBoxValueChanged = static_cast<void (QDoubleSpinBox::*)(double)>(
      &QDoubleSpinBox::valueChanged);

  // update UI
  connect(ui->canvas, &Canvas::updateUi, [&]() {
    Scene *scene = ui->canvas->getScene();
    Camera *cam = scene->camera();

    Vec3 camPos = cam->pos();
    Vec3 camLookAt = cam->lookAtValue();

    ui->camLookAtXValue->setValue(camLookAt.x());
    ui->camLookAtYValue->setValue(camLookAt.y());
    ui->camLookAtZValue->setValue(camLookAt.z());

    ui->camXValue->setValue(camPos.x());
    ui->camYValue->setValue(camPos.y());
    ui->camZValue->setValue(camPos.z());
  });

  // focal distance
  connect(ui->focalLength, spinBoxValueChanged, [&](double focalDist) {
    ui->canvas->getScene()->camera()->setFocalDist(focalDist);

    ui->canvas->renderImage();
    ui->canvas->repaint();
  });

  // antialiasing
  connect(ui->AACheckBox, &QCheckBox::stateChanged, [&](int state) {
    ui->canvas->setAntiAliasing(state == Qt::Checked);

    ui->canvas->renderImage();
    ui->canvas->repaint();
  });

  // camera pos
  connect(ui->camXValue, spinBoxValueChanged, [&](double val) {
    Scene *scene = ui->canvas->getScene();
    Camera *cam = scene->camera();

    Vec3 pos = cam->pos();
    pos.setX(val);
    cam->setPos(pos);

    ui->canvas->renderImage();
    ui->canvas->repaint();
  });
  connect(ui->camYValue, spinBoxValueChanged, [&](double val) {
    Scene *scene = ui->canvas->getScene();
    Camera *cam = scene->camera();

    Vec3 pos = cam->pos();
    pos.setY(val);
    cam->setPos(pos);

    ui->canvas->renderImage();
    ui->canvas->repaint();
  });
  connect(ui->camZValue, spinBoxValueChanged, [&](double val) {
    Scene *scene = ui->canvas->getScene();
    Camera *cam = scene->camera();

    Vec3 pos = cam->pos();
    pos.setZ(val);
    cam->setPos(pos);

    ui->canvas->renderImage();
    ui->canvas->repaint();
  });

  // look at
  connect(ui->camLookAtXValue, spinBoxValueChanged, [&](double val) {
    Scene *scene = ui->canvas->getScene();
    Camera *cam = scene->camera();

    Vec3 lookAt = cam->lookAtValue();
    lookAt.setX(val);
    cam->lookAt(lookAt);

    ui->canvas->renderImage();
    ui->canvas->repaint();
  });
  connect(ui->camLookAtYValue, spinBoxValueChanged, [&](double val) {

    Scene *scene = ui->canvas->getScene();
    Camera *cam = scene->camera();

    Vec3 lookAt = cam->lookAtValue();
    lookAt.setY(val);
    cam->lookAt(lookAt);

    ui->canvas->renderImage();
    ui->canvas->repaint();
  });
  connect(ui->camLookAtZValue, spinBoxValueChanged, [&](double val) {
    Scene *scene = ui->canvas->getScene();
    Camera *cam = scene->camera();

    Vec3 lookAt = cam->lookAtValue();
    lookAt.setZ(val);
    cam->lookAt(lookAt);

    ui->canvas->renderImage();
    ui->canvas->repaint();
  });

  // keyboard properties
  connect(ui->keyboardAngleStep, spinBoxValueChanged, [&](double val) {
    ui->canvas->setKeyboardRotationAngle(val);

    ui->canvas->renderImage();
    ui->canvas->repaint();
  });
  connect(ui->rotationAxisX, spinBoxValueChanged, [&](double val) {
    Vec3 axis = ui->canvas->keyboardRotationAxis();
    axis.setX(val);
    ui->canvas->setKeyboardRotationAxis(axis);

    ui->canvas->renderImage();
    ui->canvas->repaint();
  });
  connect(ui->rotationAxisY, spinBoxValueChanged, [&](double val) {
    Vec3 axis = ui->canvas->keyboardRotationAxis();
    axis.setY(val);
    ui->canvas->setKeyboardRotationAxis(axis);

    ui->canvas->renderImage();
    ui->canvas->repaint();
  });
  connect(ui->rotationAxisZ, spinBoxValueChanged, [&](double val) {
    Vec3 axis = ui->canvas->keyboardRotationAxis();
    axis.setZ(val);
    ui->canvas->setKeyboardRotationAxis(axis);

    ui->canvas->renderImage();
    ui->canvas->repaint();
  });
}

void MainWindow::on_performTransform_clicked() {}
