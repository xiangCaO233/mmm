#ifndef MMM_MIXNOTE_H
#define MMM_MIXNOTE_H

#include "Note.h"
#include <memory>
#include <vector>

class MixNote : public Note {
  std::vector<std::shared_ptr<Note>> child_notes;

public:
  MixNote(int timestamp, int orbit);
  ~MixNote() override;

  // 物件类型
  virtual inline NoteType type() const override { return NoteType::MIX; };
  // 返回children
  inline std::vector<std::shared_ptr<Note>> childnotes() const {
    return child_notes;
  };
  // 添加物件
  void put_note(std::shared_ptr<Note> child);
  // 移除物件
  void erase_note(std::shared_ptr<Note> note);
  virtual std::string type_name() const override;
  virtual std::string toString() const override;
};

#endif // MMM_MIXNOTE_H
