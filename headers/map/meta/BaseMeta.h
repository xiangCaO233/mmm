#ifndef MMM_BASEMETA_H
#define MMM_BASEMETA_H

#include "../../log/Logger.h"

#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

enum class MetaType {
  int_,
  float_,
  double_,
  string_,
  bool_,
  vector_ // 列表
};

// 歌曲元数据
class BaseMeta {
public:
  // 元数据信息
  struct MetaInfo {
    // 数据
    std::variant<
        int, float, double, bool, std::string,
        std::vector<std::variant<int, float, double, bool, std::string>>>
        value;
    // 数据类型
    MetaType type;
  };
  // 构造meta--注册通用属性表
  BaseMeta() {
    universalKeys = {"AudioFile",     "Title",   "TitleUnicode", "Artist",
                     "ArtistUnicode", "Creator", "Version",      "Tags"};
    registerMetaData("AudioFile", MetaType::string_, "audio.mp3");
    registerMetaData("Title", MetaType::string_, "mmm");
    registerMetaData("TitleUnicode", MetaType::string_, "mmm");
    registerMetaData("Artist", MetaType::string_, "Various Artists");
    registerMetaData("ArtistUnicode", MetaType::string_, "Various Artists");
    registerMetaData("Creator", MetaType::string_, "...");
    registerMetaData("Version", MetaType::string_, "Normal");
    std::vector<std::variant<int, float, double, bool, std::string>> tags = {
        "mmm", " "};
    registerMetaData("Tags", MetaType::vector_, tags);
  };
  // 析构meta--需要override
  virtual ~BaseMeta() = default;

  // 设置属性
  void setMetaData(
      std::string &&key, MetaType dataType,
      std::variant<
          int, float, double, bool, std::string,
          std::vector<std::variant<int, float, double, bool, std::string>>>
          value) {
    auto it = meta_data.find(key);
    if (it != meta_data.end()) {
      if (it->second.type == dataType) {
        it->second.value = value;
      } else {
        LOG_WARNING("设置类型失败,与注册类型不匹配");
      }
    } else {
      LOG_WARNING("key[" + key + "]不存在");
    }
  };
  // 获取属性，进行类型检查
  template <class T> T getMeta(std::string &&key, MetaType dataType) {
    auto it = meta_data.find(key);
    if (it != meta_data.end()) {
      try {
        return std::get<T>(it->second.value);
      } catch (const std::bad_variant_access &) {
        LOG_FATAL("meta类型错误");
        return T();
      }
    } else {
      LOG_WARNING("key[" + key + "]不存在");
      return T();
    }
  };

protected:
  // 通用键
  std::vector<std::string> universalKeys;
  // 全部属性
  std::unordered_map<std::string, MetaInfo> meta_data;
  // 注册属性
  void registerMetaData(
      std::string &&key, MetaType dataType,
      std::variant<
          int, float, double, bool, std::string,
          std::vector<std::variant<int, float, double, bool, std::string>>>
          value) {
    if (meta_data.find(key) != meta_data.end()) {
      LOG_WARNING("已覆盖key[" + key + "]");
    }
    meta_data[key] = {value, dataType};
  };
};

#endif // MMM_BASEMETA_H
