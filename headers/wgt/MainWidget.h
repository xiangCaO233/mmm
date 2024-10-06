#ifndef MMM_MAINWIDGET_H
#define MMM_MAINWIDGET_H

#include "ContentsWgt.h"
#include "TopFuncsWgt.h"
#include <QLayout>
#include <QWidget>

class MainWidget : public QWidget {
  // 根布局-vbox
  QVBoxLayout *rootVboxLayout;
  // 顶部功能
  TopFuncsWgt *topFuncs;
  // 内容
  ContentsWgt *contents;
  // 初始化组件
  void initializeComponents();

public:
  MainWidget();
  ~MainWidget();

  void test_map(Mmap *map);
};

#endif // MMM_MAINWIDGET_H
