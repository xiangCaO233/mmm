#include <QApplication>
#include <iostream>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  std::cout << "App started" << std::endl;
  return app.exec();
}
