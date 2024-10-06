#include "../../headers/wgt/WorkSpace.h"
#include "../../headers/log/Logger.h"
#include "../../headers/wgt/Canvas.h"

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
  LOG_DEBUG("初始化工作区完成");
}

WorkSpace::~WorkSpace() {}

void WorkSpace::put_map(Mmap *map) {
  mapList.push_back(map);
  tabWgt->addTab(new Canvas(map), "map1");
}
