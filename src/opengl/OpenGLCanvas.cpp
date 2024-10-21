#include "../../headers/wgt/opengl/OpenGLCanvas.h"
#include <QTimer>

OpenGLCanvas::OpenGLCanvas(QWidget *parent)
    : QOpenGLWidget(parent), shaderProgram(nullptr), VAO(0), VBO(0) {
  QSurfaceFormat format;
  // 请求 OpenGL ES 2.0
  format.setVersion(2, 1);
  // 核心模式
  format.setProfile(QSurfaceFormat::CoreProfile);
  // 启用抗锯齿
  format.setSamples(4);
  setFormat(format);
  // 创建定时器
  QTimer *timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &OpenGLCanvas::refresh_frame);
  timer->start(16);
}

OpenGLCanvas::~OpenGLCanvas() {
  // 确保当前上下文
  makeCurrent();
  // 删除缓冲对象
  glDeleteBuffers(1, &VBO);
  // 删除数组对象
  glDeleteVertexArrays(1, &VAO);
  // 删除着色器程序
  delete shaderProgram;
  doneCurrent();
}

void OpenGLCanvas::refresh_frame() { repaint(); }

void OpenGLCanvas::initializeGL() {
  // 初始化绘制函数
  initializeOpenGLFunctions();
  // 顶点着色器源代码
  const char *vertexSource = R"(
    #version 120
    attribute vec4 position;
    attribute vec4 color;
    varying vec4 fragColor;
    
    void main() {
        gl_Position = position;
        fragColor = position;
    }
)";
  // 片段着色器源代码
  const char *fragmentSource = R"(
    #version 120
    varying vec4 fragColor;
		uniform vec4 color;
    
    void main() {
        gl_FragColor = color;
    }
)";
  // 编译链接着色器
  shaderProgram = new QOpenGLShaderProgram();
  shaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexSource);
  shaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                         fragmentSource);
  shaderProgram->link();
  // 顶点初始数据
  GLfloat vertices[] = {
      -1.0f, 1.0f, // 左上角
      1.0f, -1.0f  // 右下角
  };

  // 生成一个顶点数组对象 (VAO)
  glGenVertexArrays(1, &VAO);
  // 将 VAO 绑定到当前上下文
  glBindVertexArray(VAO);
  // 生成一个缓冲对象 (VBO)
  glGenBuffers(1, &VBO);
  // 将 VBO 绑定到当前上下文
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // 为 VBO 分配内存并初始化数据
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

  // 启用顶点属性数组 (为 VAO 绑定的顶点属性启用)
  glEnableVertexAttribArray(
      0); // 这里的 0 是属性位置，即顶点着色器中对应的 attribute 的位置
  // 指定 VBO 中的顶点属性格式
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);
  // 这里的参数解释:
  // 0: 属性位置，与顶点着色器中的 attribute 变量对应
  // 2: 每个顶点有 2 个浮点数 (x 和 y)
  // GL_FLOAT: 数据类型是浮点数
  // GL_FALSE: 是否标准化（对于浮点数，这里一般为 false）
  // 0: 相邻顶点之间的步长（字节数），这里设为 0 表示紧凑布局
  // (GLvoid*)0: 数据的偏移量，这里为 0 表示从缓冲区的开头开始读取数据

  // 解绑 VAO，结束对 VAO 的操作
  glBindVertexArray(0);
}

void OpenGLCanvas::resizeGL(int w, int h) { glViewport(0, 0, w, h); }

void OpenGLCanvas::paintGL() {
  // 设置背景颜色
  glClearColor(0.13f, 0.13f, 0.13f, 1.0f);
  // 清除色彩缓存
  glClear(GL_COLOR_BUFFER_BIT);
  // 60fps(更新计时器)
  time += 0.016f;
  // t 在 0.0 到 2.0 之间循环
  float t = fmod(time, 2.0f);
  // 计算线段的端点位置
  float x1 =
      -1.0f + (t < 1.0f ? 2.0f * t : 2.0f * (2.0f - t)); // 从左到右再到左
  float y1 = 1.0f;                                       // 左上角的y坐标
  float x2 = 1.0f - (t < 1.0f ? 2.0f * t : 2.0f * (2.0f - t)); // 从右到左再到右
  float y2 = -1.0f; // 右下角的y坐标
  // qDebug() << (const char *)glGetString(GL_VERSION);
  // qDebug() << (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION);

  // 更新顶点数据
  GLfloat vertices[] = {x1, y1, x2, y2};

  // 更新顶点数据到 VBO
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices),
                  vertices); // 更新 VBO 的顶点数据

  // 计算渐变颜色
  GLfloat r = 1.0f * (1.0f - t / 2.0f) +
              0.0f * (t / 2.0f); // 从红色（1.0）到蓝色（0.0）
  GLfloat g = 0.0f * (1.0f - t / 2.0f) + 0.0f * (t / 2.0f); // 绿色保持为 0
  GLfloat b = 1.0f * (1.0f - t / 2.0f) +
              0.0f * (t / 2.0f); // 从粉色（1.0）到深蓝色（0.0）

  // 使用着色器程序
  shaderProgram->bind();
  glBindVertexArray(VAO);

  // 设置线段的颜色
  GLint colorLocation = shaderProgram->uniformLocation("color");
  glUniform4f(colorLocation, r, g, b, 1.0f); // 更新颜色

  glLineWidth(4.0f);
  glDrawArrays(GL_LINES, 0, 2); // 绘制线段
  // 解绑 VAO 和着色器程序
  glBindVertexArray(0);
  shaderProgram->release();
}
