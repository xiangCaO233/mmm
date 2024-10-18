#include "../../headers/mobj/note/Hold.h"
#include "../../headers/plugin/MetaPlugin.h"
#include <QtPlugin>
#include <memory>
#include <string>
#include <vector>

class ImdMetaData : public MetaPlugin, public BaseMeta {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID MetaPlugin_iid)
  Q_INTERFACES(MetaPlugin)
public:
  ImdMetaData() : MetaPlugin(), BaseMeta() {
    // 注册imd特有属性
    registerMetaData("Tabrows", MetaType::int_, 0);
    // 注册imd特有属性
    registerMetaData("MapLength", MetaType::int_, 0);
  };

  // 扩展构造
  ImdMetaData(std::shared_ptr<BaseMeta> source, Mmap &map)
      : MetaPlugin(), BaseMeta(*(source.get())) {
    int tabrowsv = 0;
    int maplengthv = 0;

    int tail_time = 0;
    for (auto tm : map.note_timestamps()) {
      for (auto tmnote : map.note_map()[tm]) {
        tabrowsv++;
        // 更新结尾时间
        if (tmnote->type() == NoteType::HOLD) {
          auto note = tmnote;
          int temp = tmnote->time() +
                     std::static_pointer_cast<Hold>(note)->sustaintime();
          if (tail_time < temp) {
            tail_time = temp;
          }
        } else {
          if (tail_time < tmnote->time()) {
            tail_time = tmnote->time();
          }
        }
      }
    }
    registerMetaData("tabrows", MetaType::int_, tabrowsv);
    registerMetaData("maplength", MetaType::int_, maplengthv);
  };

  // 重写析构
  ~ImdMetaData() override{};
  // 生成完整数据
  std::shared_ptr<BaseMeta> generate(std::shared_ptr<BaseMeta> source,
                                     Mmap &map) override {
    return std::make_shared<ImdMetaData>(source, map);
  }
  // 获取imd中的特有key
  std::vector<std::string> keys() override { return {"tabrows", "maplength"}; }
  std::string file_type() override { return "imd"; };
};

#include "ImdMetaData.moc"
