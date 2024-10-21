#ifndef MMM_RENDERPLUGIN_H
#define MMM_RENDERPLUGIN_H

#include "../map/meta/BaseMeta.h"
#include "../mobj/note/Note.h"
#include "../mobj/timing/Timing.h"
#include <QObject>
#include <QPainter>
#include <QtPlugin>

class Canvas;

class RenderPluin : public QObject {
  Q_OBJECT
protected:
  volatile double w;
  volatile double h;
  // 图元数据
  BaseMeta *mapMetaData;
  // 物件映射
  std::unordered_map<int, std::vector<std::shared_ptr<Note>>> *nmap;
  // 物件时间戳列表
  std::vector<int> *ntimestamplist;
  // timing映射
  std::unordered_map<int, std::vector<std::shared_ptr<Timing>>> *tmap;
  // timing时间戳列表
  std::vector<int> *ttimestamplist;
  // 绝对bpm区间头:bpm
  std::map<int, std::shared_ptr<Timing>> *abs_timings;
  // 轨道数
  volatile int max_orbit;
  // 歌曲长度
  volatile int length;

public:
  RenderPluin(){};
  ~RenderPluin() override = default;
  // 传递canvas
  virtual void sendCanvas(Canvas *canvas) = 0;
  // 绘制接口
  virtual void render(QImage *paiter) = 0;

  // 更新数据
  virtual void update_size(int wv, int hv) {
    w = wv;
    h = hv;
  }

private:
  void update_mapdata(

      BaseMeta mapMetaDatav,
      std::unordered_map<int, std::vector<std::shared_ptr<Note>>> &nmapv,
      std::vector<int> &ntimestamplistv,
      std::unordered_map<int, std::vector<std::shared_ptr<Timing>>> &tmapv,
      std::vector<int> &ttimestamplistv,
      std::map<int, std::shared_ptr<Timing>> &abs_timingsv, int max_orbitv,
      int lengthv) {
    nmap = &nmapv;
    ntimestamplist = &ntimestamplistv;
    tmap = &tmapv;
    ttimestamplist = &ttimestamplistv;
    abs_timings = &abs_timingsv;
    max_orbit = max_orbitv;
    length = lengthv;
  }
  friend Canvas;
};

#define RenderPluin_iid "com.github.render_plugin"

Q_DECLARE_INTERFACE(RenderPluin, RenderPluin_iid);

#endif // MMM_RENDERPLUGIN_H
