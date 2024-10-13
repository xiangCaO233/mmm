#include "../../headers/mobj/note/Hold.h"
#include "../../headers/mobj/note/MixNote.h"
#include "../../headers/mobj/note/Slide.h"
#include "../../headers/plugin/RenderPlugin.h"
#include "../../headers/wgt/Canvas.h"
#include <QDir>
#include <QImage>
#include <QPainter>
#include <QString>
#include <QtPlugin>
#include <memory>
#include <string>
#include <vector>

// 主渲染插件
class CanvasPrimaryRender : public RenderPluin {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID RenderPluin_iid)
  Q_INTERFACES(RenderPluin)
protected:
  Canvas *canvas;

public:
  int data;
  // 绘制起点
  int draw_start;
  // 时间线缩放
  double time_line_zoom;
  //  页总时间
  int page_time;
  //  绘制结束时间
  int draw_end;
  // 判定线偏移
  double judge_line_offset;
  //  每拍时间
  double beat_time;
  //  每毫秒像素
  double pixel_per_ms;

  QImage note_image;
  QImage long_body_image;
  QImage long_body_rotated_image;
  QImage left_arrow_image;
  QImage right_arrow_image;
  QImage node_image;
  CanvasPrimaryRender() : RenderPluin(), canvas(nullptr) {
    QDir cur = QDir::current();
    cur.cd("../");
    cur.cd("resources/");
    LOG_INFO("路径:" + (cur.absolutePath() + "/4l.png").toStdString());
    note_image.load(cur.absolutePath() + "/note.png");
    if (note_image.isNull()) {
      LOG_ERROR((cur.absolutePath() + "/note.png").toStdString() +
                "\t加载失败");
    }
    long_body_rotated_image.load(cur.absolutePath() + "/l_horizon.png");
    if (long_body_rotated_image.isNull()) {
      LOG_ERROR((cur.absolutePath() + "/l_horizon.png").toStdString() +
                "\t加载失败");
    }
    long_body_image.load(cur.absolutePath() + "/l_vertical.png");
    if (long_body_image.isNull()) {
      LOG_ERROR((cur.absolutePath() + "/l_vertical.png").toStdString() +
                "\t加载失败");
    }
    left_arrow_image.load(cur.absolutePath() + "/l_arrow.png");
    if (left_arrow_image.isNull()) {
      LOG_ERROR((cur.absolutePath() + "/l_arrow.png").toStdString() +
                "\t加载失败");
    }
    right_arrow_image.load(cur.absolutePath() + "/r_arrow.png");
    if (right_arrow_image.isNull()) {
      LOG_ERROR((cur.absolutePath() + "/r_arrow.png").toStdString() +
                "\t加载失败");
    }
    node_image.load(cur.absolutePath() + "/node.png");
    if (node_image.isNull()) {
      LOG_ERROR((cur.absolutePath() + "/node.png").toStdString() +
                "\t加载失败");
    }
  };
  ~CanvasPrimaryRender() override{};
  void sendCanvas(Canvas *c) override { canvas = c; };
  void render(QPainter *p) override {
    if (p->isActive()) {
      // 设置平滑缩放
      p->setRenderHints(QPainter::RenderHint::SmoothPixmapTransform);
      // 设置合成模式，启用 Alpha 混合
      p->setCompositionMode(QPainter::CompositionMode_SourceOver);
      // 物件数据
      // auto nmap = canvas->currentMap->note_map();
      // auto ntimestamplist = canvas->currentMap->note_timestamps();
      // auto tmap = canvas->currentMap->timing_map();
      // auto ttimestamplist = canvas->currentMap->timing_timestamps();
      // 轨道数据
      // int max_orbit = canvas->currentMap->maxOrbit() + 1;
      // double w = canvas->width();
      // double h = canvas->height();
      //  绘制起始时间
      draw_start = canvas->start_time;
      time_line_zoom = canvas->time_line_zoom;
      //  页总时间
      page_time = (int)(600 / time_line_zoom);
      // LOG_DEBUG("页时间:" + std::to_string(page_time) + "ms");
      //  绘制结束时间
      draw_end = draw_start + page_time;
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
      judge_line_offset = canvas->judge_line_offset;
      // drawBeatLine(p, w, h, draw_start, draw_end, page_time, basebpm,
      // basetime,
      //              split_policy, nmap, ntimestamplist, tmap, ttimestamplist,
      //              time_line_zoom, max_orbit, judge_line_offset);
      //  LOG_DEBUG("起始绘制时间:" + std::to_string(draw_start) + "ms");
      //  每拍时间
      beat_time = 60000 / basebpm;
      // LOG_DEBUG("每拍时间:" + std::to_string(beat_time) + "ms");
      // LOG_DEBUG("画布长宽:[" + std::to_string(w) + "," + std::to_string(h) +
      //           "]");
      //  每毫秒像素
      pixel_per_ms = h / page_time;
      // LOG_DEBUG("每毫秒像素:" + std::to_string(pixel_per_ms) + "ms");
      //  基准到绘制起点的拍距

      // 绘制节拍线
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
      p->drawLine(0, h * (1 - judge_line_offset), w,
                  h * (1 - judge_line_offset));

      // 绘制物件
      // drawMapObjects(p, w, h, draw_start, draw_end, actual_start, actual_end,
      //                pixel_per_ms, time_line_zoom, nmap, ntimestamplist,
      //                tmap, ttimestamplist, max_orbit, judge_line_offset);
      QPen pen(Qt::red, 4);
      p->setPen(pen);
      for (int ntime : *ntimestamplist) {
        for (auto note : (*nmap)[ntime]) {
          switch (note->type()) {
          case NoteType::NOTE: {
            // 绘制普通物件
            draw_note(p, note);
            break;
          }
          case NoteType::HOLD: {
            draw_hold_note(p, std::static_pointer_cast<Hold>(note), true, true);
            break;
          }
          case NoteType::SLIDE: {
            bool a;
            draw_slide_note(p, std::static_pointer_cast<Slide>(note), true,
                            true, a);
            break;
          }
          case NoteType::MIX:
            draw_mix_note(p, std::static_pointer_cast<MixNote>(note));
            break;
          }
        }
      }
      for (int ttime : *ttimestamplist) {
        for (auto timing : (*tmap)[ttime]) {
          // 绘制timing
          // LOG_INFO("此处绘制:" + timing->toString());
        }
      }
    }
    p->end();
  };
  void draw_note(QPainter *p, std::shared_ptr<Note> n) {
    double ny = (draw_end - n->time()) * pixel_per_ms - judge_line_offset * h;
    double nx = (n->orbited() ? w * ((double)n->orbit() - 1) / (double)max_orbit
                              : w * n->pos());
    double nh = 600 / time_line_zoom / 15 * pixel_per_ms;
    double nw = 2.5 * nh;
    int rny = ny - nh / 2.0;
    int rnx = nx - nw / 2.0 + w / max_orbit / 2.0;
    QRectF nrec = QRectF(rnx, rny, nw, nh);
    // p->drawRect(nrec);
    p->drawImage(nrec.x(), nrec.y(),
                 note_image.scaled(nrec.width(), nrec.height(),
                                   Qt::IgnoreAspectRatio,
                                   Qt::SmoothTransformation));
  }
  QRectF draw_hold_note(QPainter *p, std::shared_ptr<Hold> hold, bool drawnote,
                        bool drawnode) {
    double ny =
        (draw_end - hold->time()) * pixel_per_ms - judge_line_offset * h;
    double nx =
        (hold->orbited() ? w * ((double)hold->orbit() - 1) / (double)max_orbit
                         : w * hold->pos());
    double nh = 600 / time_line_zoom / 15 * pixel_per_ms;
    double nw = 2.5 * nh;
    double hold_time = hold->sustaintime();
    double ey = (draw_end - hold->time() - hold_time) * pixel_per_ms -
                judge_line_offset * h;
    double hh = ny - ey;
    double hw = nw / 6.0;
    double hx = nx - hw / 2 + w / max_orbit / 2.0;
    QRectF lrec = QRectF(hx, ey, hw, hh);
    // p->drawRect(lrec);
    p->drawImage(lrec.x(), lrec.y(), fit_long_body_size(long_body_image, lrec));
    QRectF noderect((hx - hw / 2), (ey - hw), hw * 2, hw * 2);
    if (drawnode) {
      p->drawImage(noderect.x(), noderect.y(),
                   node_image.scaled(noderect.width(), noderect.height(),
                                     Qt::IgnoreAspectRatio,
                                     Qt::SmoothTransformation));
    }
    // p->drawRect(noderect);

    if (drawnote)
      draw_note(p, hold);
    return noderect;
  }
  QRectF draw_slide_note(QPainter *p, std::shared_ptr<Slide> slide,
                         bool drawnote, bool drawarrow, bool &dir) {
    double ny =
        (draw_end - slide->time()) * pixel_per_ms - judge_line_offset * h;
    double nx =
        (slide->orbited() ? w * ((double)slide->orbit() - 1) / (double)max_orbit
                          : w * slide->pos());
    double nh = 600 / time_line_zoom / 15 * pixel_per_ms;
    double nw = 2.5 * nh;
    double slidelength = slide->slidelength();
    if (slidelength != 0) {
      double sw = slidelength * w / max_orbit;
      double sh = nw / 6.0;
      double sx =
          nx + sw - w / max_orbit / 2.0 - (slidelength - 1) * w / max_orbit;
      if (slidelength < 0)
        sx -= std::abs(sw);
      double sy = ny - sh / 2.0;

      QRectF srec = QRectF(sx, sy, std::abs(sw), sh);
      // p->drawRect(srec);

      p->drawImage(srec.x(), srec.y(),
                   fit_rotated_body_size(long_body_rotated_image, srec));

      QRectF arrowrect;
      if (slidelength > 0)
        arrowrect = QRectF(sx - 2 * sh + sw, sy - 3 * sh / 2, sh * 4, sh * 4);
      else
        arrowrect = QRectF(sx - 2 * sh, sy - 3 * sh / 2, sh * 4, sh * 4);
      dir = (slidelength > 0);
      // p->drawRect(arrowrect);
      p->drawImage(arrowrect.x(), arrowrect.y(),
                   (slidelength > 0 ? right_arrow_image : left_arrow_image)
                       .scaled(arrowrect.width(), arrowrect.height(),
                               Qt::KeepAspectRatio, Qt::SmoothTransformation));

      if (drawnote)
        draw_note(p, slide);
      return arrowrect;
    }
    return {};
  }
  void draw_mix_note(QPainter *p, std::shared_ptr<MixNote> mixnote) {
    int index = 0;
    int size = mixnote->childnotes().size();
    auto nodelist = std::vector<QRectF>();
    QRectF tailarrow;
    bool dir;
    for (auto childnote : mixnote->childnotes()) {
      // LOG_DEBUG("绘制:" + childnote->toString());
      switch (childnote->type()) {
      case NoteType::HOLD: {
        auto hold = std::static_pointer_cast<Hold>(childnote);
        nodelist.push_back(draw_hold_note(p, hold, false, false));
        break;
      };
      case NoteType::SLIDE: {
        auto slide = std::static_pointer_cast<Slide>(childnote);
        QRectF arrowrect = draw_slide_note(p, slide, false, false, dir);
        if (index + 1 == size) {
          tailarrow = arrowrect;
        }
        break;
      }
      default: {
        break;
      }
      }
      index++;
    }
    for (auto r : nodelist) {
      p->drawImage(r.x(), r.y(),
                   node_image.scaled(r.width(), r.height(), Qt::KeepAspectRatio,
                                     Qt::SmoothTransformation));
    }
    if (!tailarrow.isEmpty())
      p->drawImage(tailarrow.x(), tailarrow.y(),
                   (dir ? right_arrow_image : left_arrow_image)
                       .scaled(tailarrow.width(), tailarrow.height(),
                               Qt::KeepAspectRatio, Qt::SmoothTransformation));
    draw_note(p, mixnote);
  }
  QImage fit_long_body_size(QImage body, QRectF size) {
    // 初始化缓存图片
    QImage res(size.width(), size.height(), QImage::Format_ARGB32);
    res.fill(Qt::transparent);

    // 裁剪源图
    QImage image;
    if (body.height() > body.width())
      image = body.copy(0, 0, body.width(), body.width());
    else if (body.width() > body.height())
      image = body.copy((body.width() - body.height() / 2), 0, body.height(),
                        body.height());
    else
      image = body.copy();
    // 缩放至目标大小(以宽度为基准)
    image = image.scaled(size.width(), size.width());

    // 开始绘制
    QPainter p(&res);
    int currenty = 0;
    while (res.height() - currenty >= image.height()) {
      p.drawImage(0, currenty, image);
      currenty += image.height();
      // LOG_DEBUG("目标高度:[" + std::to_string(size.height()) +
      //           "]\t当前绘制到[" + std::to_string(currenty) + "]");
    }

    // 补充不完整剩余部分
    image = image.copy(0, 0, image.width(), res.height() - currenty);
    p.drawImage(0, currenty, image);
    // LOG_DEBUG("目标高度:[" + std::to_string(size.height()) + "]\t当前绘制到["
    // +
    //           std::to_string(currenty) + "]");

    return res;
  }
  // 复制横向body
  QImage fit_rotated_body_size(QImage body, QRectF size) {
    // 初始化缓存图片
    QImage res(size.width(), size.height(), QImage::Format_ARGB32);
    res.fill(Qt::transparent);

    // 裁剪源图
    QImage image;
    if (body.height() > body.width())
      image = body.copy(0, (body.height() - body.width()) / 2, body.width(),
                        body.width());
    else if (body.width() > body.height())
      image = body.copy(0, 0, body.height(), body.height());
    else
      image = body.copy();
    // 缩放至目标大小(以宽度为基准)
    image = image.scaled(size.height(), size.height());

    // 开始绘制
    QPainter p(&res);
    int currentx = 0;
    while (res.width() - currentx >= image.width()) {
      p.drawImage(currentx, 0, image);
      currentx += image.width();
      // LOG_DEBUG("目标高度:[" + std::to_string(size.height()) +
      //           "]\t当前绘制到[" + std::to_string(currenty) + "]");
    }

    // 补充不完整剩余部分
    image = image.copy(0, 0, image.width() - currentx, res.height());
    p.drawImage(currentx, 0, image);
    // LOG_DEBUG("目标高度:[" + std::to_string(size.height()) + "]\t当前绘制到["
    // +
    //           std::to_string(currenty) + "]");

    return res;
  }
  void drawSelectObjectsEffect(QPainter *p) {}
  bool double_mod(double a, double b) { return std::fabs(fmod(a, b)) < 1e-6; }
};

#include "CanvasPrimaryRender.moc"
