#ifndef MMM_MAPREADERPLUGIN_H
#define MMM_MAPREADERPLUGIN_H

#include "../map/Mmap.h"
#include <QObject>
#include <QtPlugin>
#include <string>

class MapReaderPlugin : public QObject {
  Q_OBJECT
public:
  MapReaderPlugin(){};
  virtual ~MapReaderPlugin() = default;
  virtual Mmap *read(std::string file_dir) = 0;
  virtual std::string suffix() = 0;
};

#define MapReaderPlugin_iid "com.github.map_reader_plugin"

Q_DECLARE_INTERFACE(MapReaderPlugin, MapReaderPlugin_iid);

#endif // MMM_MAPREADERPLUGIN_H
