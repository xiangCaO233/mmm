#include "../../headers/wgt/Canvas.h"
#include "../../headers/log/Logger.h"
#include "plugin/ICanvasRender.h"
#include <algorithm>
#include <string>

Canvas::Canvas(Mmap *map) : currentMap(map) {
  // 加载插件
  QDir pluginsDir = QDir::current();
  pluginsDir.cd("plugin");
  LOG_INFO("当前插件目录:" + pluginsDir.path().toStdString());
  // 在 pluginsDir 中查找并加载插件
  foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
    QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
    QObject *plugin = loader.instance();
    if (plugin) {
      // 插件加载成功
      LOG_SUCCESS("插件[" + fileName.toStdString() + "]加载成功");
      ICanvasRender *renderPlugin = qobject_cast<ICanvasRender *>(plugin);
      if (renderPlugin) {
        // 加载渲染插件成功
        string dot = ".";
        string name = fileName.toStdString();
        auto pos =
            std::find_end(name.begin(), name.end(), dot.begin(), dot.end());
        if (pos != name.end()) {
          name.erase(pos, name.end());
        }
        LOG_SUCCESS("成功读取渲染插件[" + name + "]");
        plugins.push_back(renderPlugin); // 将插件添加到列表中
        renderPlugin->sendCanvas(this);
      } else {
        LOG_ERROR("非法渲染插件[" + fileName.toStdString() + "]");
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
