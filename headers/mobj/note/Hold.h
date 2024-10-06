#ifndef MMM_HOLD_H
#define MMM_HOLD_H

#include "Note.h"
class Hold : public Note {
  // 持续时间
  int _sustaintime;

public:
  // 无轨构造
  Hold(int timestamp, double rPosition, int sustaintimev);
  // 有轨构造
  Hold(int timestamp, int orbit, int sustaintime);
  ~Hold();

  // 物件类型
  inline NoteType type() const override { return NoteType::HOLD; };
  // 持续时间
  inline int sustaintime() const { return _sustaintime; };
  virtual std::string toString() const override;
};
#endif // MMM_HOLD_H
