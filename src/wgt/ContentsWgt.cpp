#include "../headers/wgt/ContentsWgt.h"
#include "../headers/log/Logger.h"

ContentsWgt::ContentsWgt(QWidget *parent) : QWidget(parent) {
  LOG_DEBUG("初始化内容容器中...");

  rootLayout = new QHBoxLayout;
  rootLayout->setSpacing(0);
  rootLayout->setContentsMargins(0, 0, 0, 0);

  splitter = new QSplitter;
  fileSystemWgt = new FileSystemWgt(this);
  workSpace = new WorkSpace(this);
  splitter->addWidget(fileSystemWgt);
  splitter->addWidget(workSpace);

  rootLayout->addWidget(splitter);

  setLayout(rootLayout);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  LOG_DEBUG("初始化内容容器完成");
}

ContentsWgt::~ContentsWgt() {}
