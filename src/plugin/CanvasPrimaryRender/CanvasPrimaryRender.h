#ifndef MMM_CANVASPRIMARYRENDER_H
#define MMM_CANVASPRIMARYRENDER_H

#include "../../../headers/plugin/ICanvasRender.h"
#include "../../../headers/wgt/Canvas.h"
#include <QPainter>
#include <QtPlugin>

// 主渲染插件
class CanvasPrimaryRender : public ICanvasRender {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID ICanvasRender_iid)
  Q_INTERFACES(ICanvasRender)
protected:
  Canvas *canvas;

public:
  CanvasPrimaryRender();
  ~CanvasPrimaryRender() override;
  void sendCanvas(Canvas *c) override;
  void render(QPainter *p) override;
};

#endif // MMM_CANVASPRIMARYRENDER_H
