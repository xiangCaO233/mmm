#include "../../headers/plugin/RenderPlugin.h"
#include "../../headers/wgt/Canvas.h"
#include <QPainter>
#include <QtPlugin>

// 主渲染插件
class CanvasPrimaryRender : public RenderPluin {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID RenderPluin_iid)
  Q_INTERFACES(RenderPluin)
protected:
  Canvas *canvas;

public:
  int data;
  CanvasPrimaryRender() : RenderPluin(), canvas(nullptr){};
  ~CanvasPrimaryRender() override{};
  void sendCanvas(Canvas *c) override { canvas = c; };
  void render(QPainter *p) override {
    if (p->isActive()) { // 确保QPainter有效

      QPen pen(Qt::yellow, 4);
      p->setPen(pen);
      p->setBrush(Qt::NoBrush);
      p->drawEllipse(canvas->rect().adjusted(10, 10, -10, -10));
    }
    p->end();
  };
};

#include "CanvasPrimaryRender.moc"
