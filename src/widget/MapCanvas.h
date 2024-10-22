#ifndef MMM_MAPCANVAS_H
#define MMM_MAPCANVAS_H

#include <QOpenGLFunctions>
#include <QOpenGLWidget>

class MapCanvas : public QOpenGLWidget, QOpenGLFunctions {
public:
  MapCanvas(QWidget *parent = nullptr);
  ~MapCanvas();

protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;
};

#endif // MMM_MAPCANVAS_H
