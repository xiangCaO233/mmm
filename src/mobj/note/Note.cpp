#include "../../../headers/mobj/note/Note.h"
#include "../../../headers/log/Logger.h"
#include <string>

Note::Note(int timestamp, double rPosition, double size)
    : MapObject(timestamp, rPosition), _size(size), _orbit(-1),
      _orbited(false) {
  _info.end_pos = -1;
  _info.end_orbit = _orbit;
  _info.end_time = _time;
}

Note::Note(int timestamp, int orbit)
    : MapObject(timestamp, -1), _orbit(orbit), _orbited(true) {
  _info.end_pos = -1;
  _info.end_orbit = _orbit;
  _info.end_time = _time;
}

Note::~Note() {
  LOG_INFO("析构Note:" + std::to_string(_time) + "ms,orbit[" +
           std::to_string(_orbit) + "]");
};

std::string Note::toString() const {
  std::ostringstream oss;
  oss << "\nNote {\n"
      << "\ttype: note\n"
      << "\ttime: " << _time << ",\n"
      << "\tpos: " << _pos << ",\n"
      << "\torbited: " << (_orbited ? "true" : "false") << ",\n"
      << "\torbit: " << _orbit << "\n"
      << "}";
  return oss.str();
}
