#ifndef MMM_TIMING_H
#define MMM_TIMING_H

#include "../MapObject.h"

class Timing : public MapObject {
  double _bpm;
  double _rate;
  double _bpm_rate;

  bool _is_base;

public:
  Timing(int timestampv, double bpm, bool base);
  ~Timing() override;

  // 隐藏位置函数
  int pos() = delete;

  // 基准bpm值
  inline double bpm() { return _bpm; }
  // 相对倍速
  inline double rate() { return _rate; }
  // bpm速度
  inline double bpm_rate() { return _bpm_rate; }
  // 是否为基准timing
  inline bool is_base() { return _is_base; }

  std::string toString() const override;

  // 设置相对速度
  void set_rate(double rate);
  // 设置bpm速度
  void set_bpm_rate(double bpm_rate);
};

#endif // MMM_TIMING_H
