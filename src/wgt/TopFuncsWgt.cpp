#include "../../headers/wgt/TopFuncsWgt.h"
#include "../headers/log/Logger.h"

TopFuncsWgt::TopFuncsWgt(QWidget *parent) : QWidget(parent) {
  LOG_DEBUG("初始化顶部功能栏中...");
  // 初始化根布局
  rootLayout = new QHBoxLayout;
  rootLayout->setSpacing(2);
  rootLayout->setContentsMargins(2, 2, 2, 2);

  // 初始化搜索框
  searcher = new QLineEdit;
  searcher->setPlaceholderText("搜索");

  rootLayout->addWidget(searcher);

  setLayout(rootLayout);
  setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  LOG_DEBUG("初始化顶部功能栏完成");
};

TopFuncsWgt::~TopFuncsWgt() {}
