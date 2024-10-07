#include "../../headers/plugin/MetaPlugin.h"
#include <string>

class ImdMetaData : public MetaPlugin {
  Q_OBJECT
public:
  ImdMetaData() : MetaPlugin(){};
  ~ImdMetaData() override{};
  BaseMeta *createMeta() override { return nullptr; };
  std::string file_type() override { return "imd"; };
};

#include "ImdMetaData.moc"
