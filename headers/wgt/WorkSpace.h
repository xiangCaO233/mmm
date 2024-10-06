#ifndef MMM_WORKSPACE_H
#define MMM_WORKSPACE_H

#include "../map/Mmap.h"
#include <QLayout>
#include <QTabWidget>
#include <QWidget>
#include <vector>

class WorkSpace : public QWidget {
  // 根布局
  QVBoxLayout *rootLayout;
  // 标签组件
  QTabWidget *tabWgt;
  // 图列表
  std::vector<Mmap *> mapList;

public:
  WorkSpace(QWidget *parent);
  ~WorkSpace();

  // 设置map
  void put_map(Mmap *map);
};

#endif // MMM_WORKSPACE_H
