#ifndef MMM_NOTE_H
#define MMM_NOTE_H

#include "../MapObject.h"

enum class NoteType { NOTE, HOLD, SLIDE, MIX };
class Note : public MapObject {
protected:
  // 是否有轨
  bool _orbited;
  // 轨道位置
  int _orbit;

public:
  // 无轨构造
  Note(int timestamp, double rPosition);
  // 有轨构造
  Note(int timestamp, int orbit);
  virtual ~Note();
  // note类型
  virtual inline NoteType type() const { return NoteType::NOTE; };
  // 是否有轨
  inline bool orbited() const { return _orbited; };
  // 轨道位置
  inline int orbit() const { return _orbit; };
  virtual std::string toString() const override;
};

#endif // MMM_NOTE_H
