#include "CanvasPrimaryRender.h"
#include "../../../headers/log/Logger.h"
#include <string>

// 主渲染插件
CanvasPrimaryRender::CanvasPrimaryRender() : ICanvasRender(), canvas(nullptr) {}
CanvasPrimaryRender::~CanvasPrimaryRender() {}
void CanvasPrimaryRender::sendCanvas(Canvas *c) { canvas = c; }
void CanvasPrimaryRender::render(QPainter *p) {
  if (p->isActive()) { // 确保QPainter有效
  }
  p->end();
}

string CanvasPrimaryRender::name() { return "主绘制插件"; }

#include "moc_CanvasPrimaryRender.cpp"
