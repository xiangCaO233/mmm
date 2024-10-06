#ifndef MMM_MIXNOTE_H
#define MMM_MIXNOTE_H

#include "Note.h"
#include <memory>
#include <vector>

class MixNote : public Note {
  std::vector<std::shared_ptr<Note>> child_notes;

public:
  MixNote(int timestamp, int orbit);
  ~MixNote();

  // 物件类型
  virtual inline NoteType type() const override { return NoteType::MIX; };
  // 返回children
  inline std::vector<std::shared_ptr<Note>> childnotes() const {
    return child_notes;
  };
  // 添加物件
  void putnote(std::shared_ptr<Note> child);
  // 移除物件
  void erasenote(std::shared_ptr<Note> note);
  virtual std::string toString() const override;
};

#endif // MMM_MIXNOTE_H
