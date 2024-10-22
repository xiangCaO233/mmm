#include "widget/MapCanvas.h"
#include <QApplication>
#include <iostream>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  std::cout << "App started" << std::endl;
  MapCanvas canvas;
  canvas.resize(600, 800);
  canvas.show();
  return app.exec();
}
