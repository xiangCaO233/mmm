#ifndef MMM_CANVAS_H
#define MMM_CANVAS_H

#include "../audio/posreceiver.h"
#include "../map/Mmap.h"
#include "../plugin/RenderPlugin.h"
#include "WorkSpace.h"
#include <QDir>
#include <QPainter>
#include <QPluginLoader>
#include <QTimer>
#include <QWidget>
#include <vector>

class Canvas : public QWidget {
public:
  Canvas(Mmap *map, WorkSpace *workSpace);
  ~Canvas();
  // 当前工作区
  WorkSpace *current_work_space;
  // 当前canvas绑定的map
  Mmap *currentMap;

  // 时间线缩放比例
  double time_line_zoom{0.7};

  // 起始绘制时间
  double start_time{0.0};

  // 基准bpm
  double base_bpm;

  // 分拍策略
  int split_policy{4};

  // 最大轨道数
  int maxOrbit;

  // 判定线偏移比例
  double judge_line_offset{0.25};

  // 画板插件列表
  std::vector<RenderPluin *> plugins;

  // 插件信息列表
  std::vector<std::unordered_map<std::string, std::string>> pluginInfos;

  QTimer *refresh_timer;
  QTimer *sync_timer;

  posreceiver *preceiver;

public slots:
  void update_map();
  void update_start_time();

protected:
  // 绘制
  void paintEvent(QPaintEvent *event) override;
  // 滚动时间
  void wheelEvent(QWheelEvent *event) override;
  // 鼠标按下事件
  void mousePressEvent(QMouseEvent *event) override;
  // 鼠标移动事件
  void mouseMoveEvent(QMouseEvent *event) override;
  // 大小改变事件
  void resizeEvent(QResizeEvent *event) override;
};

#endif // MMM_CANVAS_H
