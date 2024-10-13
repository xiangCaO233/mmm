#ifndef MMM_WORKSPACE_H
#define MMM_WORKSPACE_H

#include "../map/Mmap.h"
#include "plugin/MapReaderPlugin.h"
#include "plugin/MetaPlugin.h"
#include <QDir>
#include <QLayout>
#include <QTabWidget>
#include <QWidget>
#include <string>
#include <vector>

class WorkSpace : public QWidget {
  // 根布局
  QVBoxLayout *rootLayout;
  // 标签组件
  QTabWidget *tabWgt;
  // 图列表
  std::vector<Mmap *> mapList;
  std::vector<MapReaderPlugin *> reader_plugins;
  std::vector<std::unordered_map<std::string, std::string>> readerPluginInfos;
  // meta插件列表
  std::vector<MetaPlugin *> meta_plugins;
  // meta插件信息列表
  std::vector<std::unordered_map<std::string, std::string>> metaPluginInfos;

public:
  WorkSpace(QWidget *parent);
  ~WorkSpace();

  // 添加map
  void put_map(Mmap *map);
  void read_file(std::string file);
  void load_plugin(QDir &pluginsDir, std::string ppath);
};

#endif // MMM_WORKSPACE_H
