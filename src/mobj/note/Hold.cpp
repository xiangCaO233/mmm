#include "../../../headers/mobj/note/Hold.h"
#include "../../../headers/log/Logger.h"
#include <string>

Hold::Hold(int timestamp, double rPosition, double size, int sustaintimev)
    : Note(timestamp, rPosition, size), _sustaintime(sustaintimev) {
  Note::_info.end_pos = rPosition;
  Note::_info.end_orbit = -1;
  Note::_info.end_time = _time + _sustaintime;
}

Hold::Hold(int timestamp, int orbit, int sustaintimev)
    : Note(timestamp, orbit), _sustaintime(sustaintimev) {
  Note::_info.end_pos = -1;
  Note::_info.end_orbit = _orbit;
  Note::_info.end_time = _time + _sustaintime;
}

Hold::~Hold() {
  LOG_INFO("析构Hold:" + std::to_string(_time) + "ms,orbit[" +
           std::to_string(_orbit) + "],hold[" + std::to_string(_sustaintime) +
           "ms]");
};

std::string Hold::type_name() const { return "长键"; }

std::string Hold::toString() const {
  std::ostringstream oss;
  oss << "\nHold {\n"
      << "\ttype: hold\n"
      << "\ttime: " << _time << ",\n"
      << "\tpos: " << _pos << ",\n"
      << "\torbited: " << (_orbited ? "true" : "false") << ",\n"
      << "\torbit: " << _orbit << ",\n"
      << "\tsustaintime: " << _sustaintime << "\n"
      << "}";
  return oss.str();
}
