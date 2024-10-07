#ifndef MMM_RENDERPLUGIN_H
#define MMM_RENDERPLUGIN_H

#include <QObject>
#include <QPainter>
#include <QtPlugin>

class Canvas;

class RenderPluin : public QObject {
  Q_OBJECT
public:
  RenderPluin(){};
  ~RenderPluin() override = default;
  // 传递canvas
  virtual void sendCanvas(Canvas *canvas) = 0;
  // 绘制接口
  virtual void render(QPainter *paiter) = 0;
};

#define RenderPluin_iid "com.github.render_plugin"

Q_DECLARE_INTERFACE(RenderPluin, RenderPluin_iid);

#endif // MMM_RENDERPLUGIN_H
