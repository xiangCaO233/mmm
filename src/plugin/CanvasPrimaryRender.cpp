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
  // 绘制起点
  double draw_start;
  // 时间线缩放
  double time_line_zoom;
  //  页总时间
  double page_time;
  //  绘制结束时间
  double draw_end;
  // 判定线偏移
  double judge_line_offset;
  //  每拍时间
  double beat_time;
  //  每毫秒像素
  double pixel_per_ms;

  QImage src_note_image;
  QImage src_long_body_image;
  QImage src_long_body_rotated_image;
  QImage src_left_arrow_image;
  QImage src_right_arrow_image;
  QImage src_node_image;

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
    src_note_image.load(cur.absolutePath() + "/note.png");
    if (src_note_image.isNull()) {
      LOG_ERROR((cur.absolutePath() + "/note.png").toStdString() +
                "\t加载失败");
    }
    src_long_body_rotated_image.load(cur.absolutePath() + "/l_horizon.png");
    if (src_long_body_rotated_image.isNull()) {
      LOG_ERROR((cur.absolutePath() + "/l_horizon.png").toStdString() +
                "\t加载失败");
    }
    src_long_body_image.load(cur.absolutePath() + "/l_vertical.png");
    if (src_long_body_image.isNull()) {
      LOG_ERROR((cur.absolutePath() + "/l_vertical.png").toStdString() +
                "\t加载失败");
    }
    src_left_arrow_image.load(cur.absolutePath() + "/l_arrow.png");
    if (src_left_arrow_image.isNull()) {
      LOG_ERROR((cur.absolutePath() + "/l_arrow.png").toStdString() +
                "\t加载失败");
    }
    src_right_arrow_image.load(cur.absolutePath() + "/r_arrow.png");
    if (src_right_arrow_image.isNull()) {
      LOG_ERROR((cur.absolutePath() + "/r_arrow.png").toStdString() +
                "\t加载失败");
    }
    src_node_image.load(cur.absolutePath() + "/node.png");
    if (src_node_image.isNull()) {
      LOG_ERROR((cur.absolutePath() + "/node.png").toStdString() +
                "\t加载失败");
    }
  };
  ~CanvasPrimaryRender() override{};
  void sendCanvas(Canvas *c) override { canvas = c; };
  void render(QImage *image) override {
    QPainter p(image);
    // 设置平滑缩放
    // p.setRenderHints(QPainter::RenderHint::SmoothPixmapTransform);
    // 设置合成模式，启用 Alpha 混合
    // p.setCompositionMode(QPainter::CompositionMode_SourceOver);
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
    // LOG_DEBUG("页时间:" + std::to_string(page_time) + "ms");
    //  绘制结束时间
    draw_end = draw_start + page_time;
    // LOG_DEBUG("结束绘制时间:" + std::to_string(draw_end) + "ms");
    double split_policy = canvas->split_policy;
    // 上一个基准timing
    auto base = canvas->currentMap->base_timing(canvas->start_time);
    double basebpm = -1;
    double basetime = 0;
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
    //  LOG_DEBUG("画布长宽:[" + std::to_string(w) + "," + std::to_string(h) +
    //            "]");
    // LOG_DEBUG("每毫秒像素:" + std::to_string(pixel_per_ms) + "ms");
    //   基准到绘制起点的拍距

    // 绘制节拍线
    int beat_distance_start = (draw_start - basetime) / beat_time;
    // LOG_DEBUG("start距离base拍数:" + std::to_string(beat_distance_start) +
    //"拍");
    // 实际绘制起点(start之前的一拍)
    double actual_start = basetime + (beat_distance_start - 2) * beat_time;
    if (double_mod(draw_start - basetime, beat_time)) {
      // 起点处于整拍位置(向前延伸2拍)
      // actual_start = basetime + (beat_distance_start - 2) * beat_time;
    } else {
      // 起点处于非整拍位置(由于向下取整,向前延伸一拍)
      // actual_start = basetime + (beat_distance_start - 1) * beat_time;
    }
    // LOG_DEBUG("实际起始时间:" + std::to_string(actual_start) + "ms");

    // 基准到绘制终点的拍距
    double beat_distance_end = (draw_start + page_time - basetime) / beat_time;
    // LOG_DEBUG("end距离base拍数:" +
    // std::to_string(beat_distance_end) + "拍");
    //  实际绘制结束点(页尾时间后一拍)
    double actual_end = basetime + (beat_distance_end + 3) * beat_time;
    if (double_mod(draw_start + page_time - basetime, beat_time)) {
      // 整拍位置(向后延伸2拍)
      // actual_end = basetime + (beat_distance_end + 2) * beat_time;
    } else {
      // 非整拍位置(由于向下取整,向后延伸3拍)
      // actual_end = basetime + (beat_distance_end + 3) * beat_time;
    }

    // LOG_DEBUG("实际结束时间:" + std::to_string(actual_end) + "ms");

    //  计算拍线位置
    int line_index = 0;
    double beat_line_time = actual_start;
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
      p.setPen(sPen);
      for (; split_index < split_policy; split_index++) {
        // 分拍线y轴位置
        double sly = ly - split_index * split_time * pixel_per_ms;
        // 分拍线精确时间位置
        double split_time_pos = beat_line_time + split_index * split_time;
        if (split_time_pos >= basetime)
          p.drawLine(0, sly, w, sly);

        // p.drawText(150, sly - 4,
        //             QString::number(split_time_pos) + "::pix[" +
        //                 QString::number(sly) + "]");
      }
      // 拍线样式
      QPen pen(Qt::white, 6);
      p.setPen(pen);
      // 绘制拍线
      if (ly <= h && ly >= 0)
        p.drawLine(0, ly, w, ly);
      // p.drawText(0, ly - 4,
      //             QString::number(beat_line_time) + "::pix[" +
      //                 QString::number(ly) + "]");

      line_index++;
    }
    // 绘制判定线
    QPen jPen(Qt::yellow, 8);
    p.setPen(jPen);
    p.drawLine(0, h * (1 - judge_line_offset), w, h * (1 - judge_line_offset));
    // 绘制分轨线
    QPen oPen(Qt::red, 6);
    p.setPen(oPen);
    for (int i = 1; i < max_orbit; i++) {
      p.drawLine(w / 4 * i, 0, w / 4 * i, h);
    }

    // 绘制物件
    // drawMapObjects(p, w, h, draw_start, draw_end, actual_start, actual_end,
    //                pixel_per_ms, time_line_zoom, nmap, ntimestamplist,
    //                tmap, ttimestamplist, max_orbit, judge_line_offset);
    QPen pen(Qt::red, 4);
    p.setPen(pen);
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
          QRectF b;
          draw_slide_note(p, std::static_pointer_cast<Slide>(note), true, true,
                          a, b);
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
  };
  void draw_note(QPainter &p, std::shared_ptr<Note> n) {
    auto c = get_note_center(n);
    double nh = 600.0 / time_line_zoom / 15.0 * pixel_per_ms;
    double nw = 2.5 * nh;
    double rny = c.y() - nh / 2.0;
    double rnx = c.x() - nw / 2.0 + w / max_orbit / 2.0;
    QRectF nrec = QRectF(rnx, rny, nw, nh);
    // p.drawRect(nrec);
    if (rny >= 0 && rny < h)
      p.drawImage(nrec.x(), nrec.y(), note_image);
  }
  QRectF draw_hold_note(QPainter &p, std::shared_ptr<Hold> hold, bool drawnote,
                        bool drawnode) {
    auto c = get_note_center(hold);
    double nh = 600.0 / time_line_zoom / 15.0 * pixel_per_ms;
    double nw = 2.5 * nh;
    double rny = c.y() - nh / 2.0;
    double rnx = c.x() - nw / 2.0 + w / max_orbit / 2.0;

    double hold_time = hold->sustaintime();
    double ey = (draw_end - hold->time() - hold_time) * pixel_per_ms -
                judge_line_offset * h;
    double hh = c.y() - ey;
    double hw = nw / 6.0;
    double hx = c.x() - hw / 2 + w / max_orbit / 2.0;
    QRectF lrec = QRectF(hx, ey, hw, hh);
    // p.drawRect(lrec);

    QRectF noderect((hx - hw / 2), (ey - hw), hw * 2, hw * 2);
    if (c.y() < 0 || get_time_y(hold->info().end_time) > h)
      return noderect;
    p.drawImage(lrec.x(), lrec.y(), fit_long_body_size(lrec));
    // p.drawImage(lrec.x(), lrec.y(), long_body_image);
    if (drawnode) {
      p.drawImage(noderect.x(), noderect.y(), node_image);
      if (drawnote)
        draw_note(p, hold);
    }
    // p.drawRect(noderect);

    return noderect;
  }
  QRectF draw_slide_note(QPainter &p, std::shared_ptr<Slide> slide,
                         bool drawnote, bool drawarrow, bool &dir,
                         QRectF &noderect) {
    auto c = get_note_center(slide);
    double nh = 600 / time_line_zoom / 15 * pixel_per_ms;
    double nw = 2.5 * nh;
    double rny = c.y() - nh / 2.0;
    double rnx = c.x() - nw / 2.0 + w / max_orbit / 2.0;

    double slidelength = slide->slidelength();
    if (slidelength != 0) {
      double sw = slidelength * w / max_orbit;
      double sh = nw / 6.0;
      double sx =
          c.x() + sw - w / max_orbit / 2.0 - (slidelength - 1) * w / max_orbit;
      if (slidelength < 0)
        sx -= std::abs(sw);
      double sy = c.y() - sh / 2.0;

      QRectF srec = QRectF(sx, sy, std::abs(sw), sh);
      // p.drawRect(srec);

      QRectF arrowrect;
      if (slidelength > 0) {

        arrowrect = QRectF(sx - 2 * sh + sw, sy - 3 * sh / 2, sh * 4, sh * 4);
        noderect = QRectF(sx - sh + sw, sy - sh / 2, sh * 2, sh * 2);
      } else {

        arrowrect = QRectF(sx - 2 * sh, sy - 3 * sh / 2, sh * 4, sh * 4);
        noderect = QRectF(sx - sh, sy - sh / 2, sh * 2, sh * 2);
      }

      dir = (slidelength > 0);
      if (rny < 0 || rny > h)
        return arrowrect;
      // p.drawRect(arrowrect);
      p.drawImage(srec.x(), srec.y(), fit_rotated_body_size(srec));
      // 是否绘制箭头
      if (drawarrow)
        p.drawImage(arrowrect.x(), arrowrect.y(),
                    (slidelength > 0 ? right_arrow_image : left_arrow_image));

      // 是否绘制头
      if (drawnote)
        draw_note(p, slide);
      return arrowrect;
    }
    return {};
  }
  void draw_mix_note(QPainter &p, std::shared_ptr<MixNote> mixnote) {
    auto c = get_note_center(mixnote);
    double nh = 600 / time_line_zoom / 15 * pixel_per_ms;
    double nw = 2.5 * nh;
    double rny = c.y() - nh / 2.0;
    double rnx = c.x() - nw / 2.0 + w / max_orbit / 2.0;

    if (c.y() < 0 || get_time_y(mixnote->info().end_time) > h)
      return;

    // LOG_DEBUG("当前组合键:" + mixnote->toString());
    // LOG_DEBUG("当前组合键end:" + std::to_string(mixnote->info().end_time));
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
        QRectF noderect;
        QRectF arrowrect =
            draw_slide_note(p, slide, false, false, dir, noderect);
        if (index + 1 == size) {
          tailarrow = arrowrect;
        } else {

          nodelist.push_back(noderect);
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
      p.drawImage(r.x(), r.y(), node_image);
    }
    if (!tailarrow.isEmpty())
      p.drawImage(tailarrow.x(), tailarrow.y(),
                  (dir ? right_arrow_image : left_arrow_image));
    draw_note(p, mixnote);
  }
  QImage fit_long_body_size(QRectF size) {
    // 初始化缓存图片
    QImage res(size.width(), size.height(), QImage::Format_ARGB32);
    res.fill(Qt::transparent);

    // 开始绘制
    QPainter p(&res);
    int currenty = 0;
    while (res.height() - currenty >= long_body_image.height()) {
      p.drawImage(0, currenty, long_body_image);
      currenty += long_body_image.height();
      // LOG_DEBUG("目标高度:[" + std::to_string(size.height()) +
      //           "]\t当前绘制到[" + std::to_string(currenty) + "]");
    }

    // LOG_DEBUG("long_body_image_size:[" + to_string(long_body_image.width()) +
    //           "," + to_string(long_body_image.height()) + "]");
    //  补充不完整剩余部分
    //  long_body_image = long_body_image.copy(0, 0, long_body_image.width(),
    //                                        res.height() - currenty);
    p.drawImage(0, currenty, long_body_image);
    // LOG_DEBUG("目标高度:[" + std::to_string(size.height()) + "]\t当前绘制到["
    // +
    //           std::to_string(currenty) + "]");

    return res;
  }
  // 复制横向body
  QImage fit_rotated_body_size(QRectF size) {
    // 初始化缓存图片
    QImage res(size.width(), size.height(), QImage::Format_ARGB32);
    res.fill(Qt::transparent);

    // 开始绘制
    QPainter p(&res);
    int currentx = 0;
    while (res.width() - currentx >= long_body_rotated_image.width()) {
      p.drawImage(currentx, 0, long_body_rotated_image);
      currentx += long_body_rotated_image.width();
      // LOG_DEBUG("目标高度:[" + std::to_string(size.height()) +
      //           "]\t当前绘制到[" + std::to_string(currenty) + "]");
    }

    // 补充不完整剩余部分
    // long_body_rotated_image = long_body_rotated_image.copy(
    //    0, 0, long_body_rotated_image.width() - currentx, res.height());
    p.drawImage(currentx, 0, long_body_rotated_image);
    // LOG_DEBUG("目标高度:[" + std::to_string(size.height()) + "]\t当前绘制到["
    // +
    //           std::to_string(currenty) + "]");

    return res;
  }

  QPointF get_note_center(std::shared_ptr<Note> note) {
    return {(note->orbited()
                 ? w * ((double)note->orbit() - 1) / (double)max_orbit
                 : w * note->pos()),
            get_time_y(note->time())};
  }

  // 获取y对应像素位置
  qreal get_time_y(int time) {
    return (draw_end - time) * pixel_per_ms - judge_line_offset * h;
  }
  void update_size(int wv, int hv) override {
    w = wv;
    h = hv;
    time_line_zoom = canvas->time_line_zoom;
    //  页总时间
    page_time = (600.0 / time_line_zoom);
    //   每毫秒像素
    pixel_per_ms = h / page_time;
    double nh = 600.0 / time_line_zoom / 15.0 * pixel_per_ms;
    double nw = 2.5 * nh;
    double nodew = nw / 6.0;
    note_image = src_note_image.scaled(nw, nh, Qt::IgnoreAspectRatio,
                                       Qt::SmoothTransformation);
    // 节点为两倍节点宽大小
    node_image = src_node_image.scaled(nodew * 2, nodew * 2);
    // 箭头为四倍节点宽大小
    left_arrow_image = src_left_arrow_image.scaled(nodew * 4, nodew * 4);
    right_arrow_image = src_right_arrow_image.scaled(nodew * 4, nodew * 4);
    // 长条图片
    // 裁剪源图
    QImage temp_long_image;
    if (src_long_body_image.height() > src_long_body_image.width())
      temp_long_image = src_long_body_image.copy(
          0, 0, src_long_body_image.width(), src_long_body_image.width());
    else if (src_long_body_image.width() > src_long_body_image.height())
      temp_long_image = src_long_body_image.copy(
          (src_long_body_image.width() - src_long_body_image.height() / 2), 0,
          src_long_body_image.height(), src_long_body_image.height());
    else
      temp_long_image = src_long_body_image.copy();
    // 缩放至目标大小(以宽度为基准)
    long_body_image = temp_long_image.scaled(nodew, nodew);
    // 横向
    // 裁剪源图
    QImage temp_rotated_long_image;
    if (src_long_body_rotated_image.height() >
        src_long_body_rotated_image.width())
      temp_rotated_long_image = src_long_body_rotated_image.copy(
          0,
          (src_long_body_rotated_image.height() -
           src_long_body_rotated_image.width()) /
              2,
          src_long_body_rotated_image.width(),
          src_long_body_rotated_image.width());
    else if (src_long_body_rotated_image.width() >
             src_long_body_rotated_image.height())
      temp_rotated_long_image = src_long_body_rotated_image.copy(
          0, 0, src_long_body_rotated_image.height(),
          src_long_body_rotated_image.height());
    else
      temp_rotated_long_image = src_long_body_rotated_image.copy();
    // 缩放至目标大小(以宽度为基准)
    long_body_rotated_image = temp_rotated_long_image.scaled(nodew, nodew);
  }
  void drawSelectObjectsEffect(QPainter *p) {}
  bool double_mod(double a, double b) { return std::fabs(fmod(a, b)) < 1e-6; }
};

#include "CanvasPrimaryRender.moc"
