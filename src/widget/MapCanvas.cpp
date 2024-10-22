#define GL_SILENCE_DEPRECATION
#include "widget/MapCanvas.h"
#include <OpenGL/OpenGL.h>

// 初始化opengl画布
MapCanvas::MapCanvas(QWidget *parent) : QOpenGLWidget(parent) {
  QSurfaceFormat format;
  // 请求OpenGL3.3
  format.setVersion(3, 3);
  // 抗锯齿
  format.setSamples(4);
  // 应用
  setFormat(format);
}

// 释放画布
MapCanvas::~MapCanvas() {}

void MapCanvas::initializeGL() {
  // 初始化opengl函数
  initializeOpenGLFunctions();
}

// 调整大小回调
void MapCanvas::resizeGL(int w, int h) { glViewport(0, 0, w, h); }

// 绘制画布
void MapCanvas::paintGL() {
  // 绘制背景
  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
  // 清除色彩缓存
  glClear(GL_COLOR_BUFFER_BIT);
}
