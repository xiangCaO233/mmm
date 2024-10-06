#include "../../../headers/wgt/file/FileSystemWgt.h"
#include "../../../headers/log/Logger.h"

FileSystemWgt::FileSystemWgt(QWidget *parent) : QWidget(parent) {
  rootLayout = new QHBoxLayout;
  rootLayout->setContentsMargins(0, 0, 0, 0);
  rootLayout->setSpacing(0);
  model = new QFileSystemModel;
  // 设置根目录为系统根目录
  model->setRootPath(QDir(QDir::currentPath()).filePath("files"));

  treeView = new QTreeView;
  treeView->setModel(model);
  treeView->setRootIndex(model->index(model->rootPath()));
  treeView->setColumnWidth(0, 250);

  // 在 treeView 中选择文件夹时，listView 显示该文件夹的内容
  connect(treeView, &QTreeView::clicked, [this](const QModelIndex &index) {
    LOG_DEBUG("当前目录:" + model->filePath(index).toStdString());
  });
  rootLayout->addWidget(treeView);
  setLayout(rootLayout);
}

FileSystemWgt::~FileSystemWgt() {}
