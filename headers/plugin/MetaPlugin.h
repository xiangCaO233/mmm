#ifndef MMM_METAPLUGIN_H
#define MMM_METAPLUGIN_H

#include "../map/meta/BaseMeta.h"
#include <QObject>
#include <QtPlugin>
#include <string>

class MetaPlugin : public QObject {
  Q_OBJECT
public:
  MetaPlugin(){};
  virtual ~MetaPlugin() = default;
  virtual BaseMeta *createMeta() = 0;

  virtual std::string file_type() = 0;
};

#define MetaPlugin_iid "com.github.meta_plugin"

Q_DECLARE_INTERFACE(MetaPlugin, MetaPlugin_iid);

#endif // MMM_METAPLUGIN_H
