#ifndef MMM_OPENGLCANVAS_H
#define MMM_OPENGLCANVAS_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>

class OpenGLCanvas : public QOpenGLWidget, QOpenGLFunctions {

public:
  OpenGLCanvas(QWidget *parent = nullptr);
  ~OpenGLCanvas();
public slots:
  void refresh_frame();

protected:
  float time = 0.0f;
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

private:
  QOpenGLShaderProgram *shaderProgram;
  GLuint VBO, VAO;
};

#endif // MMM_OPENGLCANVAS_H
