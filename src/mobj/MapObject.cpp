#include "../../headers/mobj/MapObject.h"

MapObject::MapObject(int _timev, double _posv) : _time(_timev), _pos(_posv) {}

MapObject::~MapObject() {}

std::string MapObject::toString() const {
  std::ostringstream oss;
  oss << "\nMapObject {\n"
      << "\ttime: " << _time << ",\n"
      << "\tpos: " << _pos << "\n"
      << "}";
  return oss.str();
}
