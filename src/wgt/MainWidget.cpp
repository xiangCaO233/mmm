#include "../../headers/wgt/MainWidget.h"
#include "../../headers/log/Logger.h"
#include <QPluginLoader>

MainWidget::MainWidget() : QWidget(nullptr) { initializeComponents(); }

MainWidget::~MainWidget() {}

void MainWidget::initializeComponents() {
  LOG_INFO("初始化组件...");
  // 根布局设置
  rootVboxLayout = new QVBoxLayout;
  rootVboxLayout->setSpacing(4);
  rootVboxLayout->setContentsMargins(4, 4, 4, 4);

  // 顶部功能
  topFuncs = new TopFuncsWgt(this);
  // 内容布局设置
  contents = new ContentsWgt(this);

  rootVboxLayout->addWidget(topFuncs);
  rootVboxLayout->addWidget(contents);
  // 应用布局
  setLayout(rootVboxLayout);
  resize(800, 600);

  LOG_INFO("初始化组件完成");
}

void MainWidget::test_map(Mmap *map) { contents->workSpace->put_map(map); }
