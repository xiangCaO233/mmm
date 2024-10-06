#include "../../../headers/mobj/note/Slide.h"
#include "../../../headers/log/Logger.h"
#include <cmath>

Slide::Slide(int timestamp, double rPosition, double direction, double distance)
    : Note(timestamp, rPosition), _slidelengthv(-1),
      _slidevector({(int)std::floor(std::cos(direction) * distance + 1e-9),
                    (int)std::floor(std::sin(direction) * distance + 1e-9),
                    direction, distance}) {}

Slide::Slide(int timestamp, double rPosition, int dx, int dy)
    : Note(timestamp, rPosition), _slidelengthv(-1),
      _slidevector({dx, dy, std::atan((double)dy / (double)dx),
                    std::sqrt(dx * dx + dy * dy)}) {}

Slide::Slide(int timestamp, int orbit, int slidelength)
    : Note(timestamp, orbit), _slidelengthv(slidelength),
      _slidevector({-1, -1, slidelength > 0 ? 0 : 2 * M_PI, -1}) {}

Slide::~Slide() {
  LOG_INFO(
      "析构slide:" + std::to_string(_time) + "ms,orbit[" +
      std::to_string(_orbit) + "],slidearg[" + std::to_string(_slidelengthv) +
      "],slidedata[" + std::to_string(_slidevector.dx) + "," +
      std::to_string(_slidevector.dy) + "," + std::to_string(_slidevector.rad) +
      "," + std::to_string(_slidevector.distance) + "]");
}

std::string Slide::toString() const {
  std::ostringstream oss;
  oss << "\nSlide {\n"
      << "\ttime: " << _time << ",\n"
      << "\tpos: " << _pos << ",\n"
      << "\torbited: " << (_orbited ? "true" : "false") << ",\n"
      << "\torbit: " << _orbit << ",\n"
      << "\tslidelength: " << _slidelengthv << ",\n"
      << "\tslidevector: {\n"
      << "\t\tdx: " << _slidevector.dx << ",\n"
      << "\t\tdy: " << _slidevector.dy << ",\n"
      << "\t\trad: " << _slidevector.rad << ",\n"
      << "\t\tdistance: " << _slidevector.distance << "\n"
      << "\t}\n"
      << "}";
  return oss.str();
}