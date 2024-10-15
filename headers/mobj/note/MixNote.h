#ifndef MMM_MIXNOTE_H
#define MMM_MIXNOTE_H

#include "../../../headers/log/Logger.h"
#include "Hold.h"
#include "Note.h"
#include "Slide.h"
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
  virtual inline NoteInfo info() override {
    if (child_notes.size() == 0) {
      return {_time, _orbit, _pos};
    }
    int et, eo, ep;
    auto tnote = child_notes[child_notes.size() - 1];
    if (tnote->type() == NoteType::HOLD) {
      auto hnote = std::static_pointer_cast<Hold>(tnote);
      return {hnote->time() + hnote->sustaintime(), hnote->orbit(),
              hnote->pos()};
    } else if (tnote->type() == NoteType::SLIDE) {
      auto snote = std::static_pointer_cast<Slide>(tnote);
      return {snote->time(), snote->orbit() + snote->slidelength(),
              snote->pos()};
    }
    LOG_ERROR("MixNote出错:" + toString());
    return {};
  };
};

#endif // MMM_MIXNOTE_H
