#include "../../headers/wgt/Canvas.h"
#include "../../headers/log/Logger.h"
#include "plugin/RenderPlugin.h"
#include <QJsonDocument>
#include <string>
#include <unordered_map>

Canvas::Canvas(Mmap *map) : currentMap(map) {
  // 加载插件
  QDir pluginsDir = QDir::current();
  pluginsDir.cd("plugin");
  pluginsDir.cd("RenderPlugin");
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
      RenderPluin *renderPlugin = qobject_cast<RenderPluin *>(plugin);
      if (renderPlugin) {
        // 将插件添加到列表中
        plugins.push_back(renderPlugin);
        // 传递指针
        renderPlugin->sendCanvas(this);
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
          pluginInfos.push_back(plugInfo);
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
}

Canvas::~Canvas() {}

void Canvas::paintEvent(QPaintEvent *event) {
  auto p = new QPainter(this);
  // LOG_INFO("插件列表长度:" + to_string((int)plugins.size()));
  for (auto plugin : plugins) {
    // 遍历插件进行渲染
    // LOG_INFO("调用插件:" + plugin->name());
    plugin->render(p);
  }
}
