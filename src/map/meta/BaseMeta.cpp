#include "../../headers/map/meta/BaseMeta.h"
#include "../../headers/log/Logger.h"

BaseMeta::BaseMeta() {
  universalKeys = {"AudioFile",     "Title",   "TitleUnicode", "Artist",
                   "ArtistUnicode", "Creator", "Version",      "Tags"};
  registerMetaData("AudioFile", QMetaType::QString, "audio.mp3");
  registerMetaData("Title", QMetaType::QString, "mmm");
  registerMetaData("TitleUnicode", QMetaType::QString, "mmm");
  registerMetaData("Artist", QMetaType::QString, "Various Artists");
  registerMetaData("ArtistUnicode", QMetaType::QString, "Various Artists");
  registerMetaData("Creator", QMetaType::QString, "...");
  registerMetaData("Version", QMetaType::QString, "Normal");
  registerMetaData("Tags", QMetaType::QString, "mmm,");
}

void BaseMeta::registerMetaData(QString &&key, QMetaType::Type dataType) {
  if (meta_data.contains(key)) {
    LOG_WARNING("key[" + key.toStdString() + "]注册失败,已包含此key");
  } else {
    meta_data[key] = {QVariant(), dataType};
  }
}

void BaseMeta::setMetaData(QString &&key, QMetaType::Type dataType,
                           QVariant value) {
  if (meta_data.contains(key)) {
    const MetaInfo &info = meta_data[key];
    if (info.type == dataType) {
      meta_data[key] = {value, dataType};
    } else {
      LOG_ERROR("类型不匹配");
    }
  } else {
    LOG_WARNING("key[" + key.toStdString() + "]不存在");
  }
}

void BaseMeta::registerMetaData(QString &&key, QMetaType::Type dataType,
                                QVariant value) {

  if (meta_data.contains(key))
    LOG_WARNING("已覆盖存在的key[" + key.toStdString() + "]");
  meta_data[key] = {value, dataType};
}

QVariant BaseMeta::getMeta(QString &&key, QMetaType::Type dataType) {
  if (meta_data.contains(key)) {
    const MetaInfo &info = meta_data[key];
    if (info.type == dataType) {
      return info.value;
    } else {
      LOG_ERROR("类型不匹配");
      return {};
    }
  } else {
    LOG_WARNING("key[" + key.toStdString() + "]不存在");
    return {};
  }
}
