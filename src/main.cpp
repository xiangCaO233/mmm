#include "../headers/log/Logger.h"
#include "../headers/map/Mmap.h"
#include "../headers/mobj/note/Hold.h"
#include "../headers/mobj/note/MixNote.h"
#include "../headers/wgt/MainWidget.h"
#include "mobj/note/Note.h"
#include "mobj/note/Slide.h"
#include "mobj/timing/Timing.h"
#include <QApplication>
#include <memory>

using namespace Logger;
int main(int argc, char *argv[]) {
  LOG_INFO("Qt App 加载中...");
  QApplication app(argc, argv);
  LOG_INFO("Qt App加载完成");

  auto w = new MainWidget;
  w->show();
#if 0
  auto map = Mmap();
  map.set_length(120000);
  map.put_timing(std::make_shared<Timing>(46, 100, true));
  map.put_note(std::make_shared<Note>(146, 3));
  map.put_note(std::make_shared<Note>(1246, 2));
  map.put_note(std::make_shared<Note>(1546, 4));
  map.put_note(std::make_shared<Note>(1846, 2));
  map.put_note(std::make_shared<Note>(1546, 2));
  map.put_note(std::make_shared<Note>(1846, 1));
  map.put_note(std::make_shared<Note>(2146, 3));
  map.put_note(std::make_shared<Hold>(50, 3, 1200));
  map.put_timing(std::make_shared<Timing>(100, 100, false));
  map.put_note(std::make_shared<Slide>(100, 1, 2));

  auto mix1 = std::make_shared<MixNote>(50, 4);

  auto hold11 = std::make_shared<Hold>(50, 4, 500);
  auto slide12 = std::make_shared<Slide>(550, 4, -2);

  auto slidet = std::make_shared<Slide>(2050, 2, 1);
  auto holdt = std::make_shared<Hold>(3000, 4, 200);

  mix1->put_note(hold11);
  mix1->put_note(slide12);
  mix1->put_note(slidet);
  mix1->put_note(holdt);
  mix1->put_note(map.note_map()[100][0]);
  mix1->put_note(mix1);

  map.put_note(mix1);

  w->test_map(&map);
#endif
  QDir current = QDir::current();
  current.cd("../");
  current.cd("resources/");
  current.cd("testmap/");
  w->test_map((current.absolutePath() + "/Contrapasso -paradiso-/t+pazolite - "
                                        "Contrapasso -paradiso-_4k_hd.imd")
                  .toStdString());

  return app.exec();
}
