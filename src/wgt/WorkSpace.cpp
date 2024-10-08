#include "../../headers/wgt/WorkSpace.h"
#include "../../headers/log/Logger.h"
#include "../../headers/wgt/Canvas.h"
#include <QDir>
#include <QJsonDocument>
#include <QPluginLoader>

WorkSpace::WorkSpace(QWidget *parent) : QWidget(parent) {
  LOG_DEBUG("初始化工作区中...");
  rootLayout = new QVBoxLayout;
  rootLayout->setSpacing(2);
  rootLayout->setContentsMargins(0, 0, 0, 0);
  tabWgt = new QTabWidget;
  rootLayout->addWidget(tabWgt);
  setLayout(rootLayout);
  setMinimumSize(600, 450);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  LOG_INFO("加载元数据插件中");
  // 加载插件
  QDir pluginsDir = QDir::current();
  pluginsDir.cd("plugin");
  pluginsDir.cd("MetaPlugin");
  LOG_INFO("meta插件目录:" + pluginsDir.path().toStdString());
  // 在 pluginsDir 中查找并加载插件
  foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
    QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
    QObject *plugin = loader.instance();
    QFileInfo fileInfo(fileName);
    QString name = pluginsDir.absoluteFilePath(fileInfo.baseName());
    if (plugin) {
      // 插件加载成功
      LOG_SUCCESS("插件[" + fileName.toStdString() + "]加载成功");
      MetaPlugin *metaPlugin = qobject_cast<MetaPlugin *>(plugin);
      if (metaPlugin) {
        // 将插件添加到列表中
        plugins.push_back(metaPlugin);
        // 读取插件信息
        QString jsonFilePath = pluginsDir.absoluteFilePath(
            "json/" + fileInfo.baseName() + ".json");
        QFile jsonFile(jsonFilePath);
        if (jsonFile.open(QIODevice::ReadOnly)) {
          QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonFile.readAll());
          QJsonObject jsonObj = jsonDoc.object();
          QString pluginName = jsonObj["name"].toString();
          QString pluginVersion = jsonObj["version"].toString();
          // 初始化插件信息
          std::unordered_map<std::string, std::string> plugInfo;
          plugInfo["name"] = pluginName.toStdString();
          plugInfo["version"] = pluginVersion.toStdString();
          metaPluginInfos.push_back(plugInfo);
          // 成功加载插件
          LOG_SUCCESS("成功加载插件:[" + pluginName.toStdString() + " v" +
                      pluginVersion.toStdString() + "]");
        } else {
          LOG_ERROR(jsonFilePath.toStdString() + "读取失败");
        }
      } else {
        LOG_ERROR("非法渲染插件[" + name.toStdString() + "]");
      }
    } else {
      // 插件加载失败
      LOG_WARNING("插件[" + fileName.toStdString() + "]加载失败");
    }
  }
  LOG_DEBUG("初始化工作区完成");
}

WorkSpace::~WorkSpace() {}

void WorkSpace::put_map(Mmap *map) {
  mapList.push_back(map);
  tabWgt->addTab(new Canvas(map), "map1");
}
