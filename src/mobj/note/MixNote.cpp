#include "../../../headers/mobj/note/MixNote.h"
#include "../../../headers/log/Logger.h"
#include "../../../headers/mobj/note/Hold.h"
#include "../../../headers/mobj/note/Slide.h"
#include <algorithm>
#include <memory>
#include <string>

MixNote::MixNote(int timestamp, int orbit) : Note(timestamp, orbit) {}

MixNote::~MixNote() {
  LOG_INFO("析构MixNote:" + std::to_string(_time) + "ms,orbit[" +
           std::to_string(_orbit) +
           "],child_notes size:" + std::to_string(child_notes.size()) + "");
}
std::string MixNote::type_name() const { return "组合键"; }

std::string MixNote::toString() const {
  std::ostringstream oss;
  oss << "\nMixNote {\n"
      << "\ttype: mix\n"
      << "\ttime: " << _time << ",\n"
      << "\tpos: " << _pos << ",\n"
      << "\torbited: " << (_orbited ? "true" : "false") << ",\n"
      << "\torbit: " << _orbit << ",\n"
      << "\tchild_notes: [\n";

  for (const auto &note : child_notes) {
    if (note) {
      oss << "\t\t" << note->toString() << ",\n";
    }
  }

  oss << "\t]\n"
      << "}";
  return oss.str();
}

void MixNote::put_note(std::shared_ptr<Note> note) {
  if (child_notes.size() != 0) {
    // 组合键的尾物件
    auto tailNote = child_notes[child_notes.size() - 1];
    // LOG_DEBUG("组合键尾:" + tailNote->toString());
    //  组合键尾物件的时间戳
    int tailNoteTime = tailNote->time();
    // 尾物件的轨道
    int tailOrbit = tailNote->orbit();
    // 设置尾物件时间
    int mixNoteTailTime = tailNoteTime;
    auto t = tailNote->type();
    if (t == NoteType::HOLD) {
      // LOG_INFO("检查到长键");
      // 长键,更新时间,不更新轨道
      mixNoteTailTime +=
          std::static_pointer_cast<Hold>(tailNote)->sustaintime();
    } else if (t == NoteType::SLIDE) {
      // LOG_INFO("检查到滑键");
      if (tailOrbit != -1) {
        // 滑键,更新轨道,不更新时间
        tailOrbit += std::static_pointer_cast<Slide>(tailNote)->slidelength();
      }
    } else {
      // 不允许向组合键添加滑键与长键之外的物件
      LOG_WARNING("🐍中不允许出现:[" +
                  std::string(t == NoteType::NOTE ? "单键" : "🐍") + "]");
      return;
    }
    bool check = true;
    // LOG_INFO("当前添加:" + note->toString());
    // LOG_INFO("蛇尾时间:" + std::to_string(mixNoteTailTime));
    // LOG_INFO("蛇尾轨道:" + std::to_string(tailOrbit));
    // 检查添加的物件是否合法
    if (note->time() != mixNoteTailTime) {
      LOG_ERROR("🐍更改失败,请检查时间戳");
      check = false;
    }
    if (note->orbit() != tailOrbit) {
      LOG_ERROR("🐍更改失败,请检查轨道");
      check = false;
    }
    if (!check)
      return;
  }
  child_notes.push_back(note);
}

void MixNote::erase_note(std::shared_ptr<Note> note) {
  auto it = std::find(child_notes.begin(), child_notes.end(), note);
  child_notes.erase(it, it);
}
