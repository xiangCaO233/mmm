#ifndef MMM_CANVAS_H
#define MMM_CANVAS_H

#include "../map/Mmap.h"
#include "../plugin/RenderPlugin.h"
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
  std::vector<RenderPluin *> plugins;

  // 插件信息列表
  std::vector<std::unordered_map<std::string, std::string>> pluginInfos;

protected:
  void paintEvent(QPaintEvent *event) override;
};

#endif // MMM_CANVAS_H
