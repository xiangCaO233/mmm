#include "posreceiver.h"

void posreceiver::receive(std::string &audio_name, long pos) {
  emit update_pos(audio_name, pos);
}

#include "moc_posreceiver.moc"
