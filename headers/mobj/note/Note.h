#ifndef MMM_NOTE_H
#define MMM_NOTE_H

#include "../MapObject.h"

enum class NoteType { NOTE, HOLD, SLIDE, MIX };
struct NoteInfo {
  int end_time;
  int end_orbit;
  double end_pos;
};
class Note : public MapObject {
protected:
  // 按键信息
  NoteInfo _info;
  // 是否有轨
  bool _orbited;
  // 轨道位置
  int _orbit;
  // 大小
  double _size;

public:
  // 无轨构造
  Note(int timestamp, double rPosition, double size);
  // 有轨构造
  Note(int timestamp, int orbit);
  virtual ~Note();
  // note类型
  virtual inline NoteType type() const { return NoteType::NOTE; };
  // 是否有轨
  inline bool orbited() const { return _orbited; };
  // 轨道位置
  inline int orbit() const { return _orbit; };
  // 物件更多信息
  virtual inline NoteInfo info() { return _info; }
  virtual std::string type_name() const { return "单键"; };
  virtual std::string toString() const override;
};

#endif // MMM_NOTE_H
