#include "../../../headers/mobj/timing/Timing.h"
#include "../../../headers/log/Logger.h"

Timing::Timing(int timestampv, double bpm, bool base)
    : MapObject(timestampv, -1), _bpm(bpm), _is_base(base) {
  _bpm_rate = bpm;
  _rate = 1.0;
};

Timing::~Timing() {

  LOG_INFO("析构Timing:" + std::to_string(_time) + "ms,bpm[" +
           std::to_string(_bpm) + "]");
}

void Timing::set_rate(double ratev) {
  _rate = ratev;
  _bpm_rate *= ratev;
}

void Timing::set_bpm_rate(double _bpm_ratev) {
  _rate = _bpm_ratev / _bpm_rate;
  _bpm_rate = _bpm_ratev;
}

std::string Timing::toString() const {
  std::ostringstream oss;
  oss << "\nTiming {\n"
      << "\ttime: " << _time << ",\n"
      << "\tbpm: " << _bpm << ",\n"
      << "\tbase: " << _is_base << "\n"
      << "\trate: " << _rate << ",\n"
      << "\t_bpm_rate: " << _bpm_rate << ",\n"
      << "}";
  return oss.str();
}
