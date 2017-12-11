#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QtWidgets>

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void on_spawn_clicked();

  void on_performTransform_clicked();

private:
  Ui::MainWindow *ui;

  void initSlots();
};

#endif // MAINWINDOW_HPP
