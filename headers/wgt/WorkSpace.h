#ifndef MMM_WORKSPACE_H
#define MMM_WORKSPACE_H

#include "../map/Mmap.h"
#include "plugin/MetaPlugin.h"
#include <QLayout>
#include <QTabWidget>
#include <QWidget>
#include <vector>

class WorkSpace : public QWidget {
  // 根布局
  QVBoxLayout *rootLayout;
  // 标签组件
  QTabWidget *tabWgt;
  // 图列表
  std::vector<Mmap *> mapList;
  // meta插件列表
  std::vector<MetaPlugin *> plugins;
  // meta插件信息列表
  std::vector<std::unordered_map<std::string, std::string>> metaPluginInfos;

public:
  WorkSpace(QWidget *parent);
  ~WorkSpace();

  // 设置map
  void put_map(Mmap *map);
};

#endif // MMM_WORKSPACE_H
