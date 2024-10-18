#ifndef MMM_MAPREADERPLUGIN_H
#define MMM_MAPREADERPLUGIN_H

#include "../map/Mmap.h"
#include <QObject>
#include <QtPlugin>
#include <memory>
#include <string>

class MapReaderPlugin : public QObject {
  Q_OBJECT
protected:
  // 交互meta数据
  std::shared_ptr<BaseMeta> _meta;

public:
  MapReaderPlugin(){};
  virtual ~MapReaderPlugin() = default;
  virtual Mmap *read(std::string file_dir) = 0;
  virtual std::string suffix() = 0;
  void place_meta(std::shared_ptr<BaseMeta> meta) {
    if (meta)
      _meta = meta;
    else
      LOG_WARNING("meta转换有误");
  };
};

#define MapReaderPlugin_iid "com.github.map_reader_plugin"

Q_DECLARE_INTERFACE(MapReaderPlugin, MapReaderPlugin_iid);

#endif // MMM_MAPREADERPLUGIN_H
