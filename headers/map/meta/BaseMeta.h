#ifndef MMM_BASEMETA_H
#define MMM_BASEMETA_H

#include <QMap>
#include <QVariant>

// 歌曲元数据
class BaseMeta {
public:
  // 元数据信息
  struct MetaInfo {
    // 数据
    QVariant value;
    // 数据类型
    QMetaType::Type type;
  };
  // 构造meta--注册通用属性表
  BaseMeta();
  // 析构meta--需要override
  virtual ~BaseMeta() = default;

  // 设置属性
  void setMetaData(QString &&key, QMetaType::Type dataType, QVariant value);
  // 获取属性，进行类型检查
  QVariant getMeta(QString &&key, QMetaType::Type dataType);

protected:
  // 通用键
  QList<QString> universalKeys;
  // 全部属性
  QMap<QString, MetaInfo> meta_data;
  // 注册属性
  void registerMetaData(QString &&key, QMetaType::Type dataType);
  void registerMetaData(QString &&key, QMetaType::Type dataType,
                        QVariant value);
};

#endif // MMM_BASEMETA_H
