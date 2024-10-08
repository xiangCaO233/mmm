#ifndef MMM_METAPLUGIN_H
#define MMM_METAPLUGIN_H

#include "../map/Mmap.h"
#include "../map/meta/BaseMeta.h"
#include <QObject>
#include <QtPlugin>
#include <memory>
#include <string>
#include <vector>

class MetaPlugin : public QObject {
  Q_OBJECT
public:
  MetaPlugin(){};
  virtual ~MetaPlugin() = default;
  // 创建元数据
  virtual std::shared_ptr<BaseMeta> generate(std::shared_ptr<BaseMeta> source,
                                             Mmap &map) = 0;
  // 获得插件的键列表
  virtual std::vector<std::string> keys() = 0;
  // 获取元数据对应文件类型
  virtual std::string file_type() = 0;
};

#define MetaPlugin_iid "com.github.meta_plugin"

Q_DECLARE_INTERFACE(MetaPlugin, MetaPlugin_iid);

#endif // MMM_METAPLUGIN_H
