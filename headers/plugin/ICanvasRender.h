#ifndef MMM_ICANVASRENDER_H
#define MMM_ICANVASRENDER_H

#include <QObject>
#include <QPainter>
#include <QtPlugin>
#include <string>

class Canvas;

class ICanvasRender : public QObject {
  Q_OBJECT
public:
  ICanvasRender() = default;
  ~ICanvasRender() override = default;
  // 传递canvas
  virtual void sendCanvas(Canvas *canvas) = 0;
  // 绘制接口
  virtual void render(QPainter *paiter) = 0;

  virtual std::string name() = 0;
};

#define ICanvasRender_iid "com.github.canvas_render"

Q_DECLARE_INTERFACE(ICanvasRender, ICanvasRender_iid);

#endif // MMM_ICANVASRENDER_H
