#ifndef MMM_CONTENTSWGT_H
#define MMM_CONTENTSWGT_H

#include "WorkSpace.h"
#include "file/FileSystemWgt.h"
#include <QLayout>
#include <QSplitter>
#include <QWidget>

class MainWidget;
class ContentsWgt : public QWidget {
  // 内容根布局
  QHBoxLayout *rootLayout;

  // 分割栏
  QSplitter *splitter;

  // 文件管理
  FileSystemWgt *fileSystemWgt;

  // 工作区
  WorkSpace *workSpace;

  friend MainWidget;

public:
  ContentsWgt(QWidget *parent);
  ~ContentsWgt();
};

#endif // MMM_CONTENTSWGT_H
