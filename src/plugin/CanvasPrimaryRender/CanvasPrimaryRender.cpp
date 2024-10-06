#include "CanvasPrimaryRender.h"

// 主渲染插件
CanvasPrimaryRender::CanvasPrimaryRender() : ICanvasRender(), canvas(nullptr) {}
CanvasPrimaryRender::~CanvasPrimaryRender() {}
void CanvasPrimaryRender::sendCanvas(Canvas *c) { canvas = c; }
void CanvasPrimaryRender::render(QPainter *p) {
  if (p->isActive()) { // 确保QPainter有效
    p->setPen(Qt::white);
    p->setBrush(Qt::NoBrush);
    p->drawEllipse(canvas->rect());
  }
  p->end();
}

#include "moc_CanvasPrimaryRender.cpp"
