#ifndef MMM_SLIDE_H
#define MMM_SLIDE_H

#include "Note.h"

class Slide : public Note {
  struct slidevector {
    // 结束点相对x坐标
    int dx;
    // 结束点相对y坐标
    int dy;
    // 滑动方向(弧度制)
    double rad;
    // 滑动距离
    double distance;
  };
  // 有轨滑动(滑动长度<0朝左,>0朝右)
  int _slidelengthv;
  // 无轨滑动(数组长度为2,值分别为[1]:滑动弧度(角度,单位rad),[2]:滑动距离)
  slidevector _slidevector;

public:
  // 无轨使用方向+距离构造
  Slide(int timestamp, double rPosition, double size, double direction,
        double distance);
  // 无轨使用相对位置构造
  Slide(int timestamp, double rPosition, double size, int dx, int dy);
  // 有轨构造
  Slide(int timestamp, int orbit, int slidelength);
  ~Slide();

  inline NoteType type() const override { return NoteType::SLIDE; };
  // 有轨滑动(滑动长度<0朝左,>0朝右)
  inline int slidelength() const { return _slidelengthv; };
  // 无轨滑动(数组长度为1,值分别为[1]:滑动弧度(角度,单位rad),[2]:滑动距离)
  inline slidevector slidedata() const { return _slidevector; };
  virtual std::string toString() const override;
};

#endif // MMM_SLIDE_H
