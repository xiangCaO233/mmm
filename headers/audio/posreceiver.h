#ifndef MMM_POSRECEIVER_H
#define MMM_POSRECEIVER_H

#include <QObject>

class posreceiver : public QObject {
  Q_OBJECT
public:
  void receive(std::string &audio_name, long pos);

signals:
  void update_pos(std::string &audio_name, long pos);
};

#endif // MMM_POSRECEIVER_H
