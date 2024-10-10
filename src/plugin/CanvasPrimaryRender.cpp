#include "../../headers/plugin/RenderPlugin.h"
#include "../../headers/wgt/Canvas.h"
#include <QDir>
#include <QImage>
#include <QPainter>
#include <QString>
#include <QtPlugin>
#include <string>

// 主渲染插件
class CanvasPrimaryRender : public RenderPluin {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID RenderPluin_iid)
  Q_INTERFACES(RenderPluin)
protected:
  Canvas *canvas;

public:
  int data;
  QImage note_image;
  CanvasPrimaryRender() : RenderPluin(), canvas(nullptr) {
    note_image.load(QDir::current().absolutePath() + "/note.png");
  };
  ~CanvasPrimaryRender() override{};
  void sendCanvas(Canvas *c) override { canvas = c; };
  void render(QPainter *p) override {
    if (p->isActive()) {
      // 物件数据
      auto nmap = canvas->currentMap->note_map();
      auto ntimestamplist = canvas->currentMap->note_timestamps();
      auto tmap = canvas->currentMap->timing_map();
      auto ttimestamplist = canvas->currentMap->timing_timestamps();
      // 轨道数据
      int max_orbit = canvas->currentMap->maxOrbit() + 1;
      //  画布信息
      double w = canvas->width();
      double h = canvas->height();
      // 绘制起始时间
      int draw_start = canvas->start_time;
      double time_line_zoom = canvas->time_line_zoom;
      //  页总时间
      int page_time = (int)(600 / time_line_zoom);
      // LOG_DEBUG("页时间:" + std::to_string(page_time) + "ms");
      //  绘制结束时间
      int draw_end = draw_start + page_time;
      // LOG_DEBUG("结束绘制时间:" + std::to_string(draw_end) + "ms");
      int split_policy = canvas->split_policy;
      // 上一个基准timing
      auto base = canvas->currentMap->base_timing(canvas->start_time);
      double basebpm = -1;
      int basetime = 0;
      if (base) {
        basebpm = base->bpm();
        basetime = base->time();
      }
      // 判定线偏移
      double judge_line_offset = canvas->judge_line_offset;

      drawBeatLine(p, w, h, draw_start, draw_end, page_time, basebpm, basetime,
                   split_policy, nmap, ntimestamplist, tmap, ttimestamplist,
                   time_line_zoom, max_orbit, judge_line_offset);
    }
    p->end();
  };
  // 绘制节拍线
  void drawBeatLine(
      QPainter *p, const double &w, const double &h, const int &draw_start,
      const int &draw_end, const int &page_time, const double &basebpm,
      const int &basetime, const int &split_policy,
      std::unordered_map<int, std::vector<std::shared_ptr<Note>>> &nmap,
      std::vector<int> &ntimestamplist,
      std::unordered_map<int, std::vector<std::shared_ptr<Timing>>> &tmap,
      std::vector<int> &ttimestamplist, const double &time_line_zoom,
      const int &max_orbit, const double &judge_line_offset) {
    // LOG_DEBUG("起始绘制时间:" + std::to_string(draw_start) + "ms");
    // 每拍时间
    double beat_time = 60000 / basebpm;
    // LOG_DEBUG("每拍时间:" + std::to_string(beat_time) + "ms");
    // LOG_DEBUG("画布长宽:[" + std::to_string(w) + "," + std::to_string(h) +
    //           "]");
    //  每毫秒像素
    double pixel_per_ms = h / page_time;
    // LOG_DEBUG("每毫秒像素:" + std::to_string(pixel_per_ms) + "ms");
    //  基准到绘制起点的拍距
    int beat_distance_start = ((double)(draw_start)-basetime) / beat_time;
    // LOG_DEBUG("start距离base拍数:" + std::to_string(beat_distance_start) +
    //"拍");
    // 实际绘制起点(start之前的一拍)
    int actual_start;
    if (double_mod((double)(draw_start - basetime), beat_time)) {
      // 起点处于整拍位置(向前延伸2拍)
      actual_start = basetime + (beat_distance_start - 2) * beat_time;
    } else {
      // 起点处于非整拍位置(由于向下取整,向前延伸一拍)
      actual_start = basetime + (beat_distance_start - 1) * beat_time;
    }
    // LOG_DEBUG("实际起始时间:" + std::to_string(actual_start) + "ms");

    // 基准到绘制终点的拍距
    int beat_distance_end =
        ((double)(draw_start + page_time) - basetime) / beat_time;
    // LOG_DEBUG("end距离base拍数:" +
    // std::to_string(beat_distance_end) + "拍");
    //  实际绘制结束点(页尾时间后一拍)
    int actual_end;
    if (double_mod((double)(draw_start + page_time - basetime), beat_time)) {
      // 整拍位置(向后延伸2拍)
      actual_end = basetime + (beat_distance_end + 2) * beat_time;
    } else {
      // 非整拍位置(由于向下取整,向后延伸3拍)
      actual_end = basetime + (beat_distance_end + 3) * beat_time;
    }

    // LOG_DEBUG("实际结束时间:" + std::to_string(actual_end) +
    // "ms");

    //  计算拍线位置
    int line_index = 0;
    int beat_line_time = actual_start;
    while (beat_line_time <= actual_end) {
      beat_line_time = actual_start + line_index * beat_time;
      // 拍线y轴位置
      double ly = (draw_end - beat_line_time) * pixel_per_ms -
                  judge_line_offset * h; // 判定线偏移

      // 绘制分拍线
      // 每分拍时间
      double split_time = beat_time / split_policy;
      int split_index = 1;
      // 分拍线样式
      QPen sPen(Qt::green, 2);
      p->setPen(sPen);
      for (; split_index < split_policy; split_index++) {
        // 分拍线y轴位置
        double sly = ly - split_index * split_time * pixel_per_ms;
        // 分拍线精确时间位置
        double split_time_pos = beat_line_time + split_index * split_time;
        if (split_time_pos >= basetime)
          p->drawLine(0, sly, w, sly);

        // p->drawText(150, sly - 4,
        //             QString::number(split_time_pos) + "::pix[" +
        //                 QString::number(sly) + "]");
      }
      // 拍线样式
      QPen pen(Qt::white, 6);
      p->setPen(pen);
      // 绘制拍线
      p->drawLine(0, ly, w, ly);
      // p->drawText(0, ly - 4,
      //             QString::number(beat_line_time) + "::pix[" +
      //                 QString::number(ly) + "]");

      line_index++;
    }
    // 绘制判定线
    QPen jPen(Qt::yellow, 8);
    p->setPen(jPen);
    p->drawLine(0, h * (1 - judge_line_offset), w, h * (1 - judge_line_offset));

    drawMapObjects(p, w, h, draw_start, draw_end, actual_start, actual_end,
                   pixel_per_ms, time_line_zoom, nmap, ntimestamplist, tmap,
                   ttimestamplist, max_orbit, judge_line_offset);
  }
  // 绘制分轨线
  void drawOrbitSplitLine(QPainter *p) {}
  // 绘制物件
  void drawMapObjects(
      QPainter *p, const double &w, const double &h, const double &draw_start,
      const double &draw_end, const double &actual_start,
      const double &actual_end, const double &pixel_per_ms,
      const double &time_line_zoom,
      std::unordered_map<int, std::vector<std::shared_ptr<Note>>> &nmap,
      std::vector<int> &ntimestamplist,
      std::unordered_map<int, std::vector<std::shared_ptr<Timing>>> &tmap,
      std::vector<int> &ttimestamplist, const int &max_orbit,
      const double &judge_line_offset) {
    for (int ntime : ntimestamplist) {
      for (auto note : nmap[ntime]) {
        // 绘制物件
        // LOG_INFO("此处绘制:" + note->toString());
        double ny =
            (draw_end - note->time()) * pixel_per_ms - judge_line_offset * h;
        double nx =
            (note->orbited() ? w * (double)note->orbit() / (double)max_orbit
                             : w * note->pos());
        // LOG_INFO("x:" + QString::number(nx).toStdString());

        double nh = 600 / time_line_zoom / 15 * pixel_per_ms;
        double nw = 2.5 * nh;

        ny -= nh / 2.0;
        nx = nx - nw / 2.0 + w / max_orbit / 2.0;
        QRectF nrec = QRectF(nx, ny, nw, nh);
        // 设置平滑缩放
        p->setRenderHints(QPainter::RenderHint::SmoothPixmapTransform);

        // QPen pen(Qt::gray, 2);
        // p->setPen(pen);
        // p->setBrush(QColor::fromRgb(235, 182, 193));
        //  绘制图片
        p->drawImage(nrec.x(), nrec.y(),
                     note_image.scaled(nrec.width(), nrec.height()));
        // p->drawRect(nrec);
        // p->drawText(nx, ny,
        //            QString::number(note->time()) + "::ypix[" +
        //                QString::number(ny) + "]");
      }
    }
    for (int ttime : ttimestamplist) {
      for (auto timing : tmap[ttime]) {
        // 绘制timing
        // LOG_INFO("此处绘制:" + timing->toString());
      }
    }
  }
  // 绘制选中的物件效果
  void drawSelectObjectsEffect(QPainter *p) {}
  bool double_mod(double a, double b) { return std::fabs(fmod(a, b)) < 1e-6; }
};

#include "CanvasPrimaryRender.moc"
