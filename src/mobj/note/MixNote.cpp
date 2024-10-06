#include "../../../headers/mobj/note/MixNote.h"
#include "mobj/note/Note.h"
#include <algorithm>
#include <memory>

MixNote::MixNote(int timestamp, int orbit) : Note(timestamp, orbit) {}

MixNote::~MixNote() {}

std::string MixNote::toString() const {
  std::ostringstream oss;
  oss << "\nMixNote {\n"
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

void MixNote::putnote(std::shared_ptr<Note> note) {
  child_notes.push_back(note);
}

void MixNote::erasenote(std::shared_ptr<Note> note) {
  auto it = std::find(child_notes.begin(), child_notes.end(), note);
  child_notes.erase(it, it);
}
