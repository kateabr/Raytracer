#include "camera.hpp"

Camera::Camera() : pos_(0, 0, 0), direction_(1, 0, 0), lookat_(0, 0, 0) {
  right_ = direction_.cross(down_).normal().times(focalDist_);
  up_ = direction_.cross(right_).normal().times(focalDist_);
}

void Camera::lookAt(const Vec3 &lookat) {
  lookat_ = lookat;
  direction_ = lookat.sub(pos_).normal();
  right_ = direction_.cross(down_).normal().times(focalDist_);
  up_ = direction_.cross(right_).normal().times(focalDist_);
}

Vec3 Camera::lookAtValue() const { return lookat_; }

Vec3 Camera::direction() const { return direction_; }

Vec3 Camera::pos() const { return pos_; }

void Camera::setPos(const Vec3 &pos) {
  pos_ = pos;

  lookAt(lookat_);
}

Vec3 Camera::right() const { return right_; }

Vec3 Camera::up() const { return up_; }

void Camera::setFocalDist(double focalDist) {
  focalDist_ = focalDist;
  lookAt(lookat_);
}
