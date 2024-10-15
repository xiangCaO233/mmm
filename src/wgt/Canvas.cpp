#include "../../headers/wgt/Canvas.h"
#include "../../headers/log/Logger.h"
#include "plugin/RenderPlugin.h"
#include <QJsonDocument>
#include <QWheelEvent>
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
        renderPlugin->update_mapdata(
            currentMap->meta(), currentMap->note_map(),
            currentMap->note_timestamps(), currentMap->timing_map(),
            currentMap->timing_timestamps(), currentMap->abs_timings(),
            currentMap->maxOrbit(), currentMap->length());
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
  // 清除为透明背景
  p->setCompositionMode(QPainter::CompositionMode_Source);
  // p->fillRect(rect(), Qt::transparent);
  //  LOG_INFO("插件列表长度:" + to_string((int)plugins.size()));
  for (auto plugin : plugins) {
    // 遍历插件进行渲染
    plugin->render(p);
  }
}

void Canvas::wheelEvent(QWheelEvent *event) {
  // LOG_DEBUG("滚动事件触发" + std::to_string(event->angleDelta().ry()));
  //  查找最近的timing
  int dy;
  dy += event->pixelDelta().rx();
  dy += event->angleDelta().ry();
  start_time += dy;
  if (start_time <= 0)
    start_time = 0;
  if (start_time >= currentMap->length() + 1000)
    start_time = currentMap->length() + 1000;
  repaint();
}

void Canvas::mousePressEvent(QMouseEvent *event) {}

void Canvas::mouseMoveEvent(QMouseEvent *event) {}

void Canvas::resizeEvent(QResizeEvent *event) {
  for (auto p : plugins)
    p->update_size(width(), height());
}
