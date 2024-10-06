#ifndef MMM_FILESYSTEMWGT_H
#define MMM_FILESYSTEMWGT_H

#include <QFileSystemModel>
#include <QLayout>
#include <QTreeView>
#include <QWidget>

class FileSystemWgt : public QWidget {
  // 根布局
  QHBoxLayout *rootLayout;
  // 文件系统模型
  QFileSystemModel *model;
  // 文件夹树状视图
  QTreeView *treeView;

public:
  FileSystemWgt(QWidget *parent);
  ~FileSystemWgt();
};

#endif // MMM_FILESYSTEMWGT_H
