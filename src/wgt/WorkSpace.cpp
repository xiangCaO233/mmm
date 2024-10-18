#include "../../headers/wgt/WorkSpace.h"
#include "../../headers/log/Logger.h"
#include "../../headers/wgt/Canvas.h"
#include "map/meta/BaseMeta.h"
#include "plugin/MapReaderPlugin.h"
#include <QDir>
#include <QJsonDocument>
#include <QPluginLoader>
#include <memory>
#include <string>

WorkSpace::WorkSpace(QWidget *parent) : QWidget(parent) {
  LOG_DEBUG("初始化工作区中...");
  rootLayout = new QVBoxLayout;
  rootLayout->setSpacing(2);
  rootLayout->setContentsMargins(0, 0, 0, 0);
  tabWgt = new QTabWidget;
  rootLayout->addWidget(tabWgt);
  setLayout(rootLayout);
  setMinimumSize(450, 600);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  LOG_INFO("加载map相关插件中");
  // 切换插件目录
  QDir pluginsDir = QDir::current();
  pluginsDir.cd("plugin");

  load_plugin(pluginsDir, "MetaPlugin");
  pluginsDir.cd("../");
  load_plugin(pluginsDir, "MapReaderPlugin");

  for (auto meta_plugin : meta_plugins) {
    for (auto reader_plugin : reader_plugins) {
      if (reader_plugin->suffix() == meta_plugin->file_type()) {
        std::shared_ptr<BaseMeta> meta(dynamic_cast<BaseMeta *>(meta_plugin));
        reader_plugin->place_meta(meta);
      }
    }
  }

  LOG_DEBUG("初始化工作区完成");
}

WorkSpace::~WorkSpace() {}

void WorkSpace::put_map(Mmap *map) {
  mapList.push_back(map);

  tabWgt->addTab(new Canvas(map), "map1");
}
void WorkSpace::read_file(std::string file) {
  LOG_INFO("尝试读取:" + file);
  for (auto reader : reader_plugins) {
    std::string suffix;
    // 查找最后一个点的位置
    size_t dotIndex = file.rfind('.');
    if (dotIndex != std::string::npos && dotIndex != 0) {
      suffix = file.substr(dotIndex + 1); // 提取后缀
    }
    if (suffix == reader->suffix()) {
      auto map = reader->read(file);
      if (map) {
        put_map(map);
      } else {
        LOG_SUCCESS("读取未完成");
      }
      break;
    }
    LOG_WARNING("不存在能够读取[" + suffix + "]文件的谱面读取插件");
  }
}
void WorkSpace::load_plugin(QDir &pluginsDir, std::string ppath) {
  pluginsDir.cd(QString::fromStdString(ppath));
  LOG_INFO("当前插件目录:" + pluginsDir.path().toStdString());
  // 在 pluginsDir 中查找并加载插件
  foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
    QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
    QObject *plugin = loader.instance();
    QFileInfo fileInfo(fileName);
    QString name = pluginsDir.absoluteFilePath(fileInfo.baseName());
    if (plugin) {
      // 插件加载成功
      LOG_SUCCESS("插件[" + fileName.toStdString() + "]加载成功");

      // 初始化插件信息
      std::unordered_map<std::string, std::string> plugInfo;
      // 初始化插件
      MetaPlugin *metaPlugin = nullptr;
      MapReaderPlugin *readerPlugin = nullptr;

      // 读取插件信息
      QString jsonFilePath =
          pluginsDir.absoluteFilePath("json/" + fileInfo.baseName() + ".json");
      QFile jsonFile(jsonFilePath);
      if (jsonFile.open(QIODevice::ReadOnly)) {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonFile.readAll());
        QJsonObject jsonObj = jsonDoc.object();
        QString pluginName = jsonObj["name"].toString();
        QString pluginVersion = jsonObj["version"].toString();
        plugInfo["name"] = pluginName.toStdString();
        plugInfo["version"] = pluginVersion.toStdString();
        // 成功加载插件
        LOG_SUCCESS("成功加载插件:[" + pluginName.toStdString() + " v" +
                    pluginVersion.toStdString() + "]");
      } else {
        LOG_ERROR(jsonFilePath.toStdString() + "读取失败");
      }

      if ("MetaPlugin" == ppath) {
        LOG_SUCCESS("加载元数据插件");
        metaPlugin = qobject_cast<MetaPlugin *>(plugin);
      } else if ("MapReaderPlugin" == ppath) {
        LOG_SUCCESS("加载读取插件");
        readerPlugin = qobject_cast<MapReaderPlugin *>(plugin);
      }

      if (metaPlugin) {
        // 将插件添加到列表中
        meta_plugins.push_back(metaPlugin);
        LOG_SUCCESS("添加元数据插件");
        metaPluginInfos.push_back(plugInfo);
      } else if (readerPlugin) {
        reader_plugins.push_back(readerPlugin);
        LOG_SUCCESS("添加读取插件");
        readerPluginInfos.push_back(plugInfo);
      } else {
        LOG_ERROR("非法插件[" + name.toStdString() + "]");
      }
    } else {
      // 插件加载失败
      LOG_WARNING("插件[" + fileName.toStdString() + "]加载失败");
    }
  }
}
