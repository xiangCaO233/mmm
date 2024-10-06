#ifndef MMM_CANVAS_H
#define MMM_CANVAS_H

#include "../map/Mmap.h"
#include "../plugin/ICanvasRender.h"
#include <QDir>
#include <QPainter>
#include <QPluginLoader>
#include <QWidget>
#include <vector>

class Canvas : public QWidget {

public:
  Canvas(Mmap *map);
  ~Canvas();

  // 当前canvas绑定的map
  Mmap *currentMap;

  // 画板插件列表
  std::vector<ICanvasRender *> plugins;

protected:
  void paintEvent(QPaintEvent *event) override;
};

#endif // MMM_CANVAS_H
