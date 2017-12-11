#ifndef COLOR_HPP
#define COLOR_HPP

#include <QColor>

#include "utils.hpp"

/** @brief Класс представления цвета. Поддерживает базовые арифметические
 * операции.
 */
class Color {
public:
  Color();

  Color(Color const &other) = default;
  Color(Color &&other) = default;
  Color &operator=(Color const &other) = default;
  Color &operator=(Color &&other) = default;

  Color(double r, double g, double b, double a = 1);

  double r() const;
  double g() const;
  double b() const;
  double a() const;

  void setR(double r);
  void setG(double g);
  void setB(double b);
  void setA(double a);

  bool operator==(Color const &other) const;

  bool operator!=(Color const &other) const;

  Color operator*(Color const &other);

  Color operator*(double n);

  Color operator+(Color const &other);

  Color operator-(Color const &other);

  explicit operator QColor() const;

private:
  double r_;
  double g_;
  double b_;
  double a_;
};

#endif // COLOR_HPP
