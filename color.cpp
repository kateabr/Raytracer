#include "color.hpp"

Color::Color() : r_(0), g_(0), b_(0), a_(1) {}

Color::Color(double r, double g, double b, double a)
    : r_(r), g_(g), b_(b), a_(a) {}

double Color::r() const { return r_; }
double Color::g() const { return g_; }
double Color::b() const { return b_; }
double Color::a() const { return a_; }

void Color::setR(double r) { this->r_ = r; }
void Color::setG(double g) { this->g_ = g; }
void Color::setB(double b) { this->b_ = b; }
void Color::setA(double a) { this->a_ = a; }

bool Color::operator==(const Color &other) const {
  return Utils::eqEps(r_, other.r_) && Utils::eqEps(g_, other.g_) &&
         Utils::eqEps(b_, other.b_) && Utils::eqEps(a_, other.a_);
}

bool Color::operator!=(const Color &other) const { return !operator==(other); }

Color Color::operator*(double n) {
  Color c(*this);
  c.r_ *= n;
  c.g_ *= n;
  c.b_ *= n;
  //  c.a_ *= n;

  return c;
}

Color Color::operator+(const Color &other) {
  Color c(*this);
  c.r_ += other.r_;
  c.g_ += other.g_;
  c.b_ += other.b_;
  c.a_ += other.a_;

  return c;
}

Color Color::operator-(const Color &other) {
  Color c(*this);
  c.r_ -= other.r_;
  c.g_ -= other.g_;
  c.b_ -= other.b_;
  c.a_ -= other.a_;

  return c;
}

Color::operator QColor() const {
  return QColor(Utils::clamp(r_) * 255, Utils::clamp(g_) * 255,
                Utils::clamp(b_) * 255, Utils::clamp(a_) * 255);
}

Color Color::operator*(const Color &other) {
  Color c(*this);
  c.r_ *= other.r_;
  c.g_ *= other.g_;
  c.b_ *= other.b_;
  //  c.a_ *= other.a_;
  return c;
}
