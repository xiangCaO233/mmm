#ifndef MMM_TOPFUNCSWGT_H
#define MMM_TOPFUNCSWGT_H

#include <QLayout>
#include <QLineEdit>
#include <QWidget>

class TopFuncsWgt : public QWidget {
  // 根布局
  QHBoxLayout *rootLayout;

  // 搜索器
  QLineEdit *searcher;

public:
  TopFuncsWgt(QWidget *parent);
  ~TopFuncsWgt();
};

#endif // MMM_TOPFUNCSWGT_H
